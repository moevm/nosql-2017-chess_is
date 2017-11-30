#ifndef PLAYER_H
#define PLAYER_H

#include <Headers/pieces.h>
#include <QObject>

class Player
{
    QList <Piece*> pieces;

public:
    QList <Piece*> getPieces();
    bool addPiece(bool isGraphic, Piece::PieceType p, QPoint pos, Piece::PieceState state = Piece::NonMoved);
    Piece* getPiece(QPoint pos);
    bool deletePiece(QPoint pos);
    Player();
    ~Player();
};

#endif // PLAYER_H

