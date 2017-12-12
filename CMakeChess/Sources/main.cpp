#include <QApplication>
#include "Headers/mainwindow.h"
#include "Headers/board.h"
#include "Headers/db.h"
#include "Headers/boardvision.h"
#include <unistd.h>
#include <iostream>


int main(int argc, char *argv[])
{
    if (!system(NULL)) exit (EXIT_FAILURE);

    pid_t pid = fork(); /* Create a child process */
    switch (pid) {
        case -1: /* Error */
            std::cerr << "Fork() failed.\n";
            exit(1);
        case 0: /* Child process */
           {
            system ("mongod"); /* run mongo server */
            exit(0);
    }
        default: /* Parent process */
        wait();
        while(system ("nc -zvv localhost 27017"));
        QApplication a(argc, argv);
        QWidget *myWidget = new QWidget();
        myWidget->setGeometry(50,50,1070,700);
        BoardVision *bv = new BoardVision(myWidget);
        bv->initBoard();
        bv->buttons();
        myWidget->show();
        int exitcode = a.exec();
        system ("mongod --shutdown"); //stop mongo server
        return exitcode;
    }
}


