#ifndef BOARDVISION_H
#define BOARDVISION_H
#include "tile.h"
//#include "db.h"
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <Headers/controller.h>

class BoardVision : public QObject
{
    Q_OBJECT 
    int count=1,index=-1, saveInd=0;
    QVector<QPoint> fromto;
    Tile *tile[8][8] = { { NULL } };
    Tile *color;
    QString table="", currTable="";
    QWidget *baseWidget;
    QListWidget *listWgt;
    QTableWidget *movesTable;
    Controller *c;
    //DB *db = new DB();
    QLabel *kLabel;
    //unsigned int player;
public:
    class Border
    {
    public:
        Border();
        void outline(QWidget *baseWidget, int xPos, int yPos, int Pos)
        {
             QLabel *outLabel = new QLabel(baseWidget);

            if(!Pos)
                outLabel->setGeometry(xPos,yPos,552,20);        //Horizontal Borders

            else
                outLabel->setGeometry(xPos,yPos,20,512);        //Vertical Borders

            outLabel->setStyleSheet("QLabel { background-color :rgb(170, 170, 127); color : black; }");
        }
    };
    BoardVision(QWidget *widget);
    ~BoardVision();
    void buttons();
    void initBoard();
    signals:
    void wantMove(QPoint from,QPoint to);
private slots:
    void onListClicked(QListWidgetItem* item );
    void moveList();
    void tileClicked(QPoint p);
    void setupedMove(QList<Player*> pl,unsigned int play,bool k);
    void backMove();
    void nextMove();
    void clearBoard();
    void savedMoves();
    void deletedMoves();
};

#endif // BOARDVISION_H
