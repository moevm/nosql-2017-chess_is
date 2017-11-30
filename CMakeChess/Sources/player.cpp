#include "Headers/player.h"

QList<Piece *> Player::getPieces()
{
    return pieces;
}

Piece *Player::getPiece(QPoint pos)
{
    for(int i=0; i<pieces.length(); i++){
        if(pieces[i]->getPosition()==pos){
            return pieces[i];
        }
    }
    return new Empty();
}

bool Player::addPiece(bool isGraphic, Piece::PieceType p, QPoint pos, Piece::PieceState state)
{
    for(int i = 0; i<pieces.length(); i++){
        if(pieces[i]->getPosition()==pos){
            return false;
        }
    }
    pieces.append(Piece::CreatePiece(isGraphic, p, pos, state));
    return true;
}

bool Player::deletePiece(QPoint pos)
{
    for(int i=0; i<pieces.length(); i++){
        if(pieces[i]->getPosition()==pos){
            delete pieces[i];
            pieces.removeAt(i);
            return true;
        }
    }
    return false;
}

Player::Player()
{

}

Player::~Player()
{
    for(int i=0; i<pieces.length(); i++){
        delete pieces[i];
    }
}

