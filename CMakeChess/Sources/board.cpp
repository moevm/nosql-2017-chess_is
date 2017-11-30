#include "Headers/board.h"

const QList <Piece *> Board::getPieces()
{
    QList <Piece*> temp;
    for(int i=0; i<players.length(); i++){
        temp.append(players[i]->getPieces());
    }
    return temp;
}

const QList<Player *> Board::getPlayers()
{
    return players;
}

bool Board::isPlayerIndexCorrect(const int &playerIndex)
{
    return playerIndex >= 0 && playerIndex < players.length();
}

bool Board::isOnBoard(QPoint pos)
{
    return  (1 <= pos.y() && pos.y() <= 8 &&
             a <= pos.x() && pos.x() <= h);
}

Board *Board::CreateSimpleBoard(bool boardType){
    Board* b  = new Board();
    b->viewType = boardType;
    return b;
}

Board *Board::CreateGraphicBoard(bool boardType)
{
}

Board *Board::CreateBoard(bool isGraphic)
{
    //if(isGraphic)
        //return CreateGraphicBoard(isGraphic);
    //else
    return CreateSimpleBoard(isGraphic);
}

bool Board::getViewType()
{
    return viewType;
}

unsigned Board::getPlayersAmount()
{
    return players.length();
}

void Board::addPlayer()
{
    players.append(new Player());
}

bool Board::deletePlayer(const int &playerIndex)
{
    if(!isPlayerIndexCorrect(playerIndex)){
        return false;
    }
    delete players[playerIndex];
    players.removeAt(playerIndex);
    return true;
}

bool Board::addPlayerPiece(const int &playerIndex, Piece::PieceType p, QPoint pos, Piece::PieceState state)
{
    if(!isPlayerIndexCorrect(playerIndex) || !isOnBoard(pos)){
        return false;
    }
    for(int i=0; i<players.length(); i++){
       if(!players[i]->getPiece(pos)->isEmpty())
               return false;
    }
    players[playerIndex]->addPiece(viewType, p, pos, state);
    return true;
}

Piece* Board::getPlayerPiece(const int &playerIndex, const QPoint &pos)
{
    return players[playerIndex]->getPiece(pos);
}

bool Board::changePlayerPiecePosition(const int &playerIndex, const QPoint &from, const QPoint &to)
{
    for(int i=0; i<players.length(); i++){
       if(!players[i]->getPiece(to)->isEmpty())
               return false;
    }
    Piece* p = getPlayerPiece(playerIndex, from);
    if(!p->isEmpty() && from!=to && isOnBoard(to) && p->MovePattern(to,getPieces()))
    {
        p->setPosition(to);
        p->setState(Piece::Moved);
        return true;
    }
    return false;
}

bool Board::deletePlayerPiece(const int &playerIndex, const QPoint &pos)
{
    return players[playerIndex]->deletePiece(pos);
}

/*bool Board::isPlayerPieceUnderAttack(const int &playerIndex, const QPoint &pos)
{
    Piece* pieceUnderAttack = getPlayerPiece( playerIndex, pos);
    QList <Piece * > enemyPieces;
    for(int i=0; i<players.length(); i++){
        if(i!=playerIndex){
            enemyPieces.append(players[i]->getPieces());
        }
    }
    return pieceUnderAttack->isUnderAttack(enemyPieces, getPieces());
}*/

int Board::getPiecePlayerIndex(const QPoint &pos)
{
    for(int i=0; i<players.length(); i++){
       if(!players[i]->getPiece(pos)->isEmpty())
               return i;
    }
    return -1;
}

Piece *Board::createCopy(const QPoint &pos)
{
    Piece* temp = NULL;
    for(int i=0; i<players.length(); i++){
        delete temp;
        temp = getPlayerPiece(i, pos);
        if(!temp->isEmpty())
        {
            return Piece::CreatePiece(viewType, temp->getType(), temp->getPosition(), temp->getState());
        }
    }
    return temp;
}

Board::~Board()
{
    for(int i=0 ;i<players.length(); i++){
        delete players[i];
    }
}

