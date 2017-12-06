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
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
class DB
{
private:
    mongocxx::database db;
    mongocxx::collection collection;
public:
    DB()
    {   mongocxx::instance instance{}; // This should be done only once.
        mongocxx::client client{mongocxx::uri{}};
        db = client["chess"];
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
    }
    QStringList tableList()
    {
        //qDebug() << db.tables();
        mongocxx::cursor curs= db.list_collections();
         QStringList list; QString s;
        for(auto coll : curs) {
          s=QString::fromStdString(bsoncxx::to_json(coll));
          list<<s.split(",").first().split(":").last();
        }
        return list;
    }
    bool containsTable(const QString &name)
    {
        return tableList().contains(name);
    }
    QStringList deleteList(){
     QStringList  sl=  tableList();
        foreach (QString s, sl) {
            if(recordCount(s)<=0)
               bool b=sl.removeOne(s);
        }
        return sl;
    }

    int recordCount(const QString &name)
    {
        createTable(name);
        mongocxx::cursor cursor = collection.find(document{} << finalize);
        int i=0;
        for(auto doc : cursor) {
          i++;
        }
        return i;
    }
    void deleteTable(const QString &name)
    {
         createTable(name);
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
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
         collection.insert_one(view);
    }
    QVector<QPoint> readMove(const QString &table, int i)
    {
        QStringList list; QString s;

        createTable(table);
        QVector<QPoint> moves;
        mongocxx::cursor cursor = collection.find(document{} << finalize);
        for(auto doc : cursor) {
            s= QString::fromStdString(bsoncxx::to_json(doc));
            list<<s.split(",").first();
            int k=0;
            if(i==k)
            {
                 moves[0]=QPoint(list[1].split(":").last().toInt(),list[2].split(":").last().toInt());
                 moves[1]=QPoint(list[3].split(":").last().toInt(),list[4].split(":").last().toInt());
            }
            k++;
        }
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


    QString searchDebute(QVector<QPoint> &deb)
    {
        if(deb.count()<6) return "false";
        QVector<QPoint> moves;
        QStringList list=tableList();
        int k=0,i;
        for(i =0;i<list.count();i++ )
        {
            QString table = list[i];
            for(int j=0;j<deb.count();j++  ){
                moves<<readMove(table,i);
            }
            qSort(moves.begin(),moves.end(),lessThan);
            qSort(deb.begin(),deb.end(),lessThan);
            //qDebug() << p << deb;
            if(moves==deb) break;
            moves.clear();
        }
        if(moves==deb)  return list[i];
        return "false";
    }

    static bool lessThan( const QPoint & e1, const QPoint & e2 )
    {
        int d1=e1.x()*e1.x()+e1.y()*e1.y();
        int d2=e2.x()*e2.x()+e2.y()*e2.y();
        if(d1==d2) return e1.x()<e2.x();
        return d1<d2;
    }

    void al23_barbo(){
            deleteTable("Al_barbo");
            createTable("Al_barbo");
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
            //QVector <QPoint> v;
            //v<<QPoint(5,2)<<QPoint(5,4)<<QPoint(5,7)<<QPoint(5,5)<<QPoint(7,1)<<QPoint(6,3)<<
                   // QPoint(2,8)<< QPoint(3,6)<<QPoint(6,1)<<QPoint(3,4)<<QPoint(6,8)<<QPoint(3,5);
            //qDebug()<<searchDebute(v);
    }

    void Murphy(){
        //DB *db = new DB();
            deleteTable("ChessMorphy");
            createTable("ChessMorphy");
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
            writeMove("ChessMorphy",QPoint(5,1),QPoint(3,1)); //рокировка
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
};


#endif // DB_H