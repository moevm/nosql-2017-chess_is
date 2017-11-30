#ifndef TILE_H
#define TILE_H
#include <QLabel>
#include <QDebug>
#include "pieces.h"
class Tile: public QLabel
{
     Q_OBJECT
public:
    //Fields
    int tileColor,piece,row,col,tileNum;
    bool checked=false,pieceColor;
    Piece::PieceType pieceName;

    //Constructors
    Tile(QWidget* pParent=0, Qt::WindowFlags f=0) : QLabel(pParent, f) {}
    Tile(const QString& text, QWidget* pParent = 0, Qt::WindowFlags f = 0) : QLabel(text, pParent, f){}

    //Methods
    void display(Piece::PieceType elem);
    void tileDisplay();
    void mousePressEvent(QMouseEvent* event) {
           emit clicked(QPoint(col,row));
        }
signals:
    void clicked(QPoint p);
public slots:
    void slotClicked()
{
    qDebug()<<row << col;
}

};
//
#endif // TILE_H
