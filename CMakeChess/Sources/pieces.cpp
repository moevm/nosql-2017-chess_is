#include "Headers/pieces.h"

Piece *Piece::CreateSimplePiece(PieceType type, QPoint pos, PieceState state)
{
    switch(type){
    case K:
        return new King(pos, state);
        break;
    case Q:
        return new Queen(pos, state);
        break;
    case R:
        return new Rook(pos, state);
        break;
    case B:
        return new Bishop(pos, state);
        break;
    case N:
        return new Knight(pos, state);
        break;
    case P:
        return new Pawn(pos, state);
        break;
    default: return new Empty();
    }
}

Piece::~Piece()
{

}

Piece *Piece::CreatePiece(bool isGraphic, PieceType type, QPoint pos, PieceState state)
{
    //if(isGraphic)
        //return CreateGraphicPiece(type, pos);
    //else
        return CreateSimplePiece(type, pos, state);
}

bool Piece::isSamePosition(QPoint pos){
    return Piece::pos == pos;
}

bool Piece::isUnderAttack(Piece* attackingPiece, const QList<Piece*> &allPieces)
{
        if(attackingPiece->MovePattern(pos,allPieces)){
            return true;
        }
    return false;
}

QPoint Piece::getPosition(){
    return pos;
}

void Piece::setPosition(QPoint pos){
    if(!isSamePosition(pos)){
        Piece::pos = pos;
    }
}

void Piece::setState(PieceState state)
{
    Piece::state = state;
}

Piece::PieceState Piece::getState()
{
    return state;
}

Piece:: PieceType Piece::getType(){
    return type;
}

bool Piece::isEmpty(){
    if(type == Piece::E)
        return true;
    else
        return false;
}

int Piece::sign(int n)
{
    return (n < 0) ? -1 : (n > 0);
}

King::King(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = K;
    Piece::state = state;
}

King::~King()
{

}

bool King::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = pos.x() -  newPos.x();
    int diffY = pos.y() - newPos.y();
    if(!isSamePosition(newPos)){
        if(abs(diffX) <= 1 && abs(diffY) <= 1)
            return true;
        else
        {
            if(abs(diffX)==2 && diffY == 0 && state == NonMoved)
            {
                for(int i=0 ; i<pieces.length(); i++){
                    if(pieces[i]->getType() == R && pieces[i]->MovePattern(pos,pieces) && pieces[i]->getState()== NonMoved)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Queen::Queen(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = Q;
    Piece::state = state;
}

Queen::~Queen()
{

}

bool Queen::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = pos.x() -  newPos.x();
    int diffY = pos.y() - newPos.y();
    if(!isSamePosition(newPos) && (abs(diffY) == 0 || abs(diffX) == 0 ||  abs(diffY) == abs(diffX)))
         {
                for(int i =0; i< pieces.size(); i++){
                    int diffIntersectsX = pieces[i]->getPosition().x() -  newPos.x();
                    int diffIntersectsY = pieces[i]->getPosition().y() -  newPos.y();
                    if((diffX ==0 && diffIntersectsX ==0 && abs(diffIntersectsY) < abs(diffY) && sign(diffIntersectsY) == sign(diffY)) ||
                            (diffY ==0 && diffIntersectsY ==0 && abs(diffIntersectsX) < abs(diffX) && sign(diffIntersectsX) == sign(diffX)) ||
                            (abs(diffIntersectsX) == abs(diffIntersectsY) && abs(diffIntersectsX) < abs(diffX) && sign(diffIntersectsY) == sign(diffY) && sign(diffIntersectsX) == sign(diffX))){
                        return false;
                    }
                }
                return true;
    } else {
        return false;
    }
}

Rook::Rook(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = R;
    Piece::state = state;
}

Rook::~Rook()
{

}

bool Rook::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = pos.x() -  newPos.x();
    int diffY = pos.y() - newPos.y();
    if(!isSamePosition(newPos) && (abs(diffY) == 0 || abs(diffX) == 0))
    {
           for(int i =0; i<pieces.length(); i++){
               int diffIntersectsX = pieces[i]->getPosition().x() -  newPos.x();
               int diffIntersectsY = pieces[i]->getPosition().y() -  newPos.y();
               if((diffX ==0 && abs(diffIntersectsY) < abs(diffY) && diffIntersectsX ==0 && sign(diffIntersectsY) == sign(diffY)) ||
                       (diffY ==0 && diffIntersectsY ==0 && abs(diffIntersectsX) < abs(diffX) && sign(diffIntersectsX) == sign(diffX)))
                   return false;
           }
           return true;
       }
       else
           return false;
}

Bishop::Bishop(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = B;
    Piece::state = state;
}

Bishop::~Bishop()
{

}

bool Bishop::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = pos.x() -  newPos.x();
    int diffY = pos.y() - newPos.y();
    if(!isSamePosition(newPos) && abs(diffY) == abs(diffX) )
    {
           for(int i =0; i<pieces.length(); i++){
               int diffIntersectsX = pieces[i]->getPosition().x() -  newPos.x();
               int diffIntersectsY = pieces[i]->getPosition().y() -  newPos.y();
               if(abs(diffIntersectsX) == abs(diffIntersectsY) && abs(diffIntersectsX) < abs(diffX) && sign(diffIntersectsY) == sign(diffY) && sign(diffIntersectsX) == sign(diffX))
                   return false;
           }
           return true;
       }
       else
           return false;
}

Knight::Knight(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = N;
    Piece::state = state;
}

Knight::~Knight()
{

}

bool Knight::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = pos.x() -  newPos.x();
    int diffY = pos.y() - newPos.y();
    if(!isSamePosition(newPos) &&
            ((abs(diffY) == 2 &&  abs(diffX) == 1)
             || (abs(diffX) == 2 &&  abs(diffY) == 1)))
           return true;
       else
           return false;
}

Pawn::Pawn(QPoint pos, PieceState state){
    Piece::pos = pos;
    type = P;
    Piece::state = state;
}

Pawn::~Pawn()
{

}

bool Pawn::MovePattern(QPoint newPos, const QList <Piece*> &pieces)
{
    int diffX = newPos.x() - pos.x();
    int diffY = newPos.y() - pos.y();
    if (!isSamePosition(newPos))
    {
       if(((abs(diffY) == 1 || abs(diffY) == 2) && diffX == 0) || (abs(diffY) == 1 && abs(diffX) == 1))
           return true;
       else
           return false;
    }

       /*if((abs(diffY) == 1 || abs(diffY) == 2) && diffX == 0){
        for(int i =0; i<pieces.length(); i++){
            int diffIntersectsX = pieces[i]->getPosition().x() -  newPos.x();
            int diffIntersectsY = pieces[i]->getPosition().y() -  newPos.y();
            if(diffIntersectsX == 0 && diffIntersectsY == 0 && pieces[i]->getType()==P)
                return false;
        }
        return true;
       } else{
           if(abs(diffY) == 1 && abs(diffX) == 1)
           {
                for(int i =0; i<pieces.length(); i++)
                {
                    int diffIntersectsX = pieces[i]->getPosition().x() -  newPos.x();
                    int diffIntersectsY = pieces[i]->getPosition().y() -  newPos.y();
                    if((diffIntersectsX == 0 && diffIntersectsY == 0) ||
                            (abs(diffIntersectsX) == 0 && abs(diffIntersectsY) == 1))
                        return true;
                }
          }
           return false;
       }
   }
    return false;*/
}
