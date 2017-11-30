#include <QApplication>
#include "Headers/mainwindow.h"
#include "Headers/board.h"
//#include "db.h"
#include "Headers/boardvision.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget *myWidget = new QWidget();
    myWidget->setGeometry(50,50,1070,700);
    //DB *db = new DB();
    //db->openDB();
    //db->Murphy();
    //db->al23_barbo();
    BoardVision *bv = new BoardVision(myWidget);
    bv->initBoard();
    bv->buttons();
    myWidget->show();
    return a.exec();
    //db->closeDB();
}



