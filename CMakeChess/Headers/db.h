#ifndef DB_H
#define DB_H

#include <QDebug>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <QString>
#include <QDebug>
#include <string>
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace std;
class DB
{
private:
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database db = client["chess"];
    mongocxx::collection collection;
public:
    DB()
    {
    }
    ~DB(){

    }

    QString intToChar(int i)
    {
        QString s;
        switch (i) {
        case 1:
            s='a';
            break;
        case 2:
            s='b';
            break;
        case 3:
            s='c';
            break;
        case 4:
            s='d';
            break;
        case 5:
            s='e';
            break;
        case 6:
            s='f';
            break;
        case 7:
            s='g';
            break;
        case 8:
            s='h';
            break;
        default:
            s='no';
            break;
        }
        return s;
    }

    void createTable(const QString &name)
    {

        collection = db[name.toStdString()];
        //qDebug() << name <<"cT";
    }
    QStringList tableList(const QString &ct)
    {

        mongocxx::cursor curs= db.list_collections();
         QStringList list; QString s;
        for(auto coll : curs) {
          s=QString::fromStdString(bsoncxx::to_json(coll));
          s=s.split(",").first().split(":").last().remove('"').remove(' ');
          if(s!=ct)list<<s;

        }
        qDebug() <<list;
        return list;
    }
    bool containsTable(const QString &name)
    {
        qDebug() <<"conT";
        return tableList("1").contains(name);
    }
    QStringList deleteList(){
     QStringList  sl=  tableList("1");
     qDebug() <<"delL";
     qDebug() << sl;
        foreach (QString s, sl) {
            if(recordCount(s)<=0)
               bool b=sl.removeOne(s);
        }

        return sl;
    }

    int recordCount(const QString &name)
    {
        createTable(name);
        qDebug() <<"cou";
        mongocxx::cursor cursor = collection.find(document{} << finalize);
        int i=0;
        for(auto doc : cursor) {
          i++;
        }
        qDebug() <<"i";
        return i;
    }
    void deleteTable(const QString &name)
    {
         createTable(name);
         qDebug() <<"del "<<name;
         collection.drop();
    }
    void writeMove(const QString &table,QPoint from, QPoint to)
    {
        createTable(table);
        auto builder = bsoncxx::builder::stream::document{};
        bsoncxx::document::value doc_value = builder
          << "from1" << from.x()
          << "from2" << from.y()
          << "to1" << to.x()
          << "to2" << to.y()
          << finalize;
        bsoncxx::document::view view = doc_value.view();
        qDebug() <<"wr1";
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
         collection.insert_one(view);
        qDebug() <<"wr2";
    }
    QVector<QPoint> readMove(const QString &table, int i)
    {
        QStringList list; QString s;

        createTable(table);

        QVector<QPoint> moves;
        mongocxx::cursor cursor = collection.find(document{} << finalize);
       // qDebug() <<"rM";
        int k=0;
        for(auto doc : cursor) {
            s= QString::fromStdString(bsoncxx::to_json(doc));

            list=s.split(",");

           // qDebug() <<list[4].split(":")<<i;
            if(i==k)
            {
                 moves<<QPoint(list[1].split(":").last().remove(" ").toInt(),list[2].split(":").last().remove(" ").toInt());
                 moves<<QPoint(list[3].split(":").last().remove(" ").toInt(),list[4].split(":").last().remove(" ").remove("}").toInt());
            }
            k++;
        }
        //qDebug() << moves;
        return moves;
    }
    QStringList readMovesS(const QString &name)
    {
        QVector<QPoint> moves;
        QStringList sl;
        for(int i=0; i<recordCount(name); i++)
        {
            moves=readMove(name,i);
            sl<<QString::number(moves[0].x())<<QString::number(moves[0].y())<<QString::number(moves[1].x())<<QString::number(moves[1].y());
            //QTextStream(&s)<<intToChar(moves[0].x())<<'-'<<moves[0].y()<<" -> "<<intToChar(moves[1].x())<<'-'<<moves[1].y()<<'\n';
        }
        return sl;
    }


    QString searchDebute(QVector<QPoint> &deb, const QString &name)
    {
        if(deb.count()<6) return "false";
        QVector<QPoint> moves;
        QStringList list=tableList(name);
        int k=0,i;
        for(i =0;i<list.count();i++ )
        {
            QString table = list[i];
            for(int j=0;j<deb.count()/2;j++  ){
                moves<<readMove(table,j);
            }
            qSort(moves.begin(),moves.end(),lessThan);
            qSort(deb.begin(),deb.end(),lessThan);
            //qDebug() <<moves;
            if(moves==deb) break;
            moves.clear();
        }
        qDebug() <<list[i];
        if(moves==deb)  return list[i];
        return "false";
    }

    QStringList filterDebute(QVector<QPoint> &deb, const QString &name)
    {
        QVector<QPoint> moves;
        QStringList list=tableList(name),res;
        int k=0,i;
        for(i =0;i<list.count();i++ )
        {
            QString table = list[i];
            for(int j=0;j<deb.count()/2;j++  ){
                moves<<readMove(table,j);
            }
            qSort(moves.begin(),moves.end(),lessThan);
            qSort(deb.begin(),deb.end(),lessThan);
            //qDebug() <<moves;
            if(moves==deb) res<<list[i];
            moves.clear();
        }
        return res;
    }

    static bool lessThan( const QPoint & e1, const QPoint & e2 )
    {
        int d1=e1.x()*e1.x()+e1.y()*e1.y();
        int d2=e2.x()*e2.x()+e2.y()*e2.y();
        if(d1==d2) return e1.x()<e2.x();
        return d1<d2;
    }

    void al23_barbo(){
        if(!containsTable("Al_barbo")){
            writeMove("Al_barbo",QPoint(5,2),QPoint(5,4));
            writeMove("Al_barbo",QPoint(5,7),QPoint(5,5));
            writeMove("Al_barbo",QPoint(7,1),QPoint(6,3));
            writeMove("Al_barbo",QPoint(2,8),QPoint(3,6));
            writeMove("Al_barbo",QPoint(6,1),QPoint(3,4));
            writeMove("Al_barbo",QPoint(6,8),QPoint(3,5));
            writeMove("Al_barbo",QPoint(5,1),QPoint(7,1));//rok
            writeMove("Al_barbo",QPoint(8,1),QPoint(6,1));
            writeMove("Al_barbo",QPoint(4,7),QPoint(4,6));
            writeMove("Al_barbo",QPoint(3,2),QPoint(3,3));
            writeMove("Al_barbo",QPoint(4,8),QPoint(6,6));
            writeMove("Al_barbo",QPoint(4,2),QPoint(4,3));
            writeMove("Al_barbo",QPoint(8,7),QPoint(8,6));
            writeMove("Al_barbo",QPoint(3,1),QPoint(5,3));
            writeMove("Al_barbo",QPoint(3,5),QPoint(2,6));
            writeMove("Al_barbo",QPoint(5,3),QPoint(2,6));
            writeMove("Al_barbo",QPoint(1,7),QPoint(2,6));
            writeMove("Al_barbo",QPoint(8,2),QPoint(8,3));
            writeMove("Al_barbo",QPoint(7,8),QPoint(5,7));
            writeMove("Al_barbo",QPoint(6,3),QPoint(8,2));
            writeMove("Al_barbo",QPoint(5,7),QPoint(7,6));
            writeMove("Al_barbo",QPoint(2,1),QPoint(4,2));
            writeMove("Al_barbo",QPoint(7,6),QPoint(6,4));
            writeMove("Al_barbo",QPoint(4,2),QPoint(6,3));
            writeMove("Al_barbo",QPoint(6,6),QPoint(7,6));
            writeMove("Al_barbo",QPoint(7,2),QPoint(7,3));
            writeMove("Al_barbo",QPoint(6,4),QPoint(8,3));
            writeMove("Al_barbo",QPoint(7,1),QPoint(7,2));
            writeMove("Al_barbo",QPoint(8,3),QPoint(6,4));
            writeMove("Al_barbo",QPoint(7,2),QPoint(8,1));
            writeMove("Al_barbo",QPoint(3,8),QPoint(8,3));
            writeMove("Al_barbo",QPoint(6,1),QPoint(7,1));
            writeMove("Al_barbo",QPoint(7,6),QPoint(8,5));
            writeMove("Al_barbo",QPoint(7,3),QPoint(6,4));
            writeMove("Al_barbo",QPoint(5,5),QPoint(6,4));
            writeMove("Al_barbo",QPoint(8,2),QPoint(7,4));
            writeMove("Al_barbo",QPoint(8,3),QPoint(7,4));
            writeMove("Al_barbo",QPoint(8,1),QPoint(7,2));
            writeMove("Al_barbo",QPoint(8,5),QPoint(8,3));
        }
            //QVector <QPoint> v;
            //v<<QPoint(5,2)<<QPoint(5,4)<<QPoint(5,7)<<QPoint(5,5)<<QPoint(7,1)<<QPoint(6,3)<<
                   // QPoint(2,8)<< QPoint(3,6)<<QPoint(6,1)<<QPoint(3,4)<<QPoint(6,8)<<QPoint(3,5);
            qDebug()<<"searchDebute(v)";
    }

    void Murphy(){
        if(!containsTable("ChessMorphy")) {
            writeMove("ChessMorphy",QPoint(5,2),QPoint(5,4));
            writeMove("ChessMorphy",QPoint(5,7),QPoint(5,5));
            writeMove("ChessMorphy",QPoint(7,1),QPoint(6,3));
            writeMove("ChessMorphy",QPoint(4,7),QPoint(4,6));
            writeMove("ChessMorphy",QPoint(4,2),QPoint(4,4));
            writeMove("ChessMorphy",QPoint(3,8),QPoint(7,4));
            writeMove("ChessMorphy",QPoint(4,4),QPoint(5,5));
            writeMove("ChessMorphy",QPoint(7,4),QPoint(6,3));
            writeMove("ChessMorphy",QPoint(4,1),QPoint(6,3));
            writeMove("ChessMorphy",QPoint(4,6),QPoint(5,5));
            writeMove("ChessMorphy",QPoint(6,1),QPoint(3,4));
            writeMove("ChessMorphy",QPoint(7,8),QPoint(6,6));
            writeMove("ChessMorphy",QPoint(6,3),QPoint(2,3));
            writeMove("ChessMorphy",QPoint(4,8),QPoint(5,7));
            writeMove("ChessMorphy",QPoint(2,1),QPoint(3,3));
            writeMove("ChessMorphy",QPoint(3,7),QPoint(3,6));
            writeMove("ChessMorphy",QPoint(3,1),QPoint(7,5));
            writeMove("ChessMorphy",QPoint(2,7),QPoint(2,5));
            writeMove("ChessMorphy",QPoint(3,3),QPoint(2,5));
            writeMove("ChessMorphy",QPoint(3,6),QPoint(2,5));
            writeMove("ChessMorphy",QPoint(3,4),QPoint(2,5));
            writeMove("ChessMorphy",QPoint(2,8),QPoint(4,7));
            writeMove("ChessMorphy",QPoint(5,1),QPoint(3,1)); //СЂРѕРєРёСЂРѕРІРєР°
            writeMove("ChessMorphy",QPoint(1,1),QPoint(4,1));
            writeMove("ChessMorphy",QPoint(1,8),QPoint(4,8));
            writeMove("ChessMorphy",QPoint(4,1),QPoint(4,7));
            writeMove("ChessMorphy",QPoint(4,8),QPoint(4,7));
            writeMove("ChessMorphy",QPoint(8,1),QPoint(4,1));
            writeMove("ChessMorphy",QPoint(5,7),QPoint(5,6));
            writeMove("ChessMorphy",QPoint(2,5),QPoint(4,7));
            writeMove("ChessMorphy",QPoint(6,6),QPoint(4,7));
            writeMove("ChessMorphy",QPoint(4,4),QPoint(5,5));
            writeMove("ChessMorphy",QPoint(2,3),QPoint(2,8));
            writeMove("ChessMorphy",QPoint(4,7),QPoint(2,8));
            writeMove("ChessMorphy",QPoint(4,1),QPoint(4,8));
           // QPoint m[12]={QPoint(5,2),QPoint(5,4),QPoint(5,7),QPoint(5,5),QPoint(7,1),QPoint(6,3),
                  //  QPoint(4,7), QPoint(4,6),QPoint(4,2),QPoint(4,4),QPoint(3,8),QPoint(7,4)};
        }
    }
};


#endif // DB_H
