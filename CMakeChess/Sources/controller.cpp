#include "Headers/controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
    delete board;
}

void Controller::initializeGame(bool viewType)
{
    board = Board::CreateBoard(viewType);
    createWhitePlayer();
    createBlackPlayer();
    currPlayerIndex = 0;
    for(int i=0; i<board->getPlayersAmount(); i++){
        ElPassantPieces.append(NULL);
        ElPassantTTLs.append(0);
    }
}

void Controller::refreshGame()
{
     bool prevt = board->getViewType();
     delete board;
     kings.clear();
     ElPassantPieces.clear();
     ElPassantTTLs.clear();
     initializeGame(prevt);
     /*board = Board::CreateBoard(prevt);
     createWhitePlayer();
     createBlackPlayer();
     currPlayerIndex = 0;*/
     emit moveMade(board->getPlayers(), currPlayerIndex, false);
}

void Controller::destroyGame()
{
    delete board;
    board = NULL;
}

void Controller::moveTransmission()
{
    if(currPlayerIndex + 1 < board->getPlayersAmount()){
        currPlayerIndex++;
    }
    else {
        currPlayerIndex = 0;
    }
}

void Controller::moveBackTransmission()
{
    if(currPlayerIndex < 1){
        currPlayerIndex = board->getPlayersAmount() - 1;
    }
    else {
        currPlayerIndex --;
    }
    increaseElPassantTTLs();
}

void Controller::createWhitePlayer()
{
    board->addPlayer();
    int playerIndex = board->getPlayersAmount() - 1;
    //king
    board->addPlayerPiece(playerIndex, Piece::K, QPoint(Board::e,1));
    kings.append(board->getPlayerPiece(playerIndex, QPoint(Board::e,1)));
    //queen
    board->addPlayerPiece(playerIndex, Piece::Q,QPoint(Board::d,1));
    //bishops
    board->addPlayerPiece(playerIndex, Piece::B,QPoint(Board::f,1));
    board->addPlayerPiece(playerIndex, Piece::B,QPoint(Board::c,1));
    //knights
    board->addPlayerPiece(playerIndex, Piece::N,QPoint(Board::g,1));
    board->addPlayerPiece(playerIndex, Piece::N,QPoint(Board::b,1));
    //rooks
    board->addPlayerPiece(playerIndex, Piece::R,QPoint(Board::h,1));
    board->addPlayerPiece(playerIndex, Piece::R,QPoint(Board::a,1));
    //pawns
    for(int i=Board::a; i<=Board::h; i++){
        board->addPlayerPiece(playerIndex, Piece::P,QPoint(i,2));
    }
}

void Controller::createBlackPlayer()
{
    board->addPlayer();
    int playerIndex = board->getPlayersAmount() - 1;
    //king
    board->addPlayerPiece(playerIndex, Piece::K, QPoint(Board::e,8));
    kings.append(board->getPlayerPiece(playerIndex, QPoint(Board::e,8)));
    //queen
    board->addPlayerPiece(playerIndex, Piece::Q,QPoint(Board::d,8));
    //bishops
    board->addPlayerPiece(playerIndex, Piece::B,QPoint(Board::f,8));
    board->addPlayerPiece(playerIndex, Piece::B,QPoint(Board::c,8));
    //knights
    board->addPlayerPiece(playerIndex, Piece::N,QPoint(Board::g,8));
    board->addPlayerPiece(playerIndex, Piece::N,QPoint(Board::b,8));
    //rooks
    board->addPlayerPiece(playerIndex, Piece::R,QPoint(Board::h,8));
    board->addPlayerPiece(playerIndex, Piece::R,QPoint(Board::a,8));
    //pawns
    for(int i=Board::a; i<=Board::h; i++){
        board->addPlayerPiece(playerIndex, Piece::P,QPoint(i,7));
    }
}

bool Controller::isPlayerKingUnderAttack(unsigned int playerIndex, QPoint kingPosition)
{
    Piece* pieceUnderAttack = board-> getPlayerPiece( playerIndex, kingPosition);
    //QList <Piece* > enemyPieces;
    QList <Player* >players = board->getPlayers();
    for(int i=0; i<players.length(); i++){
        if(i!=playerIndex){
            //enemyPieces.append(players[i]->getPieces());
            QList <Piece *>playerPieces = players[i]->getPieces();
            for(int j=0; j<playerPieces.length(); j++)
            {
                if(playerPieces[j]->getType()==Piece::P)
                {
                    QPoint pieceUnderAttackPosition = pieceUnderAttack->getPosition();
                    QPoint attackingPiecePosition = playerPieces[j]->getPosition();
                    int diffX = pieceUnderAttackPosition.x() - attackingPiecePosition.x();
                    int diffY = pieceUnderAttackPosition.y() - attackingPiecePosition.y();
                    if(isCorrectDirectionPawnMovement(pieceUnderAttackPosition,attackingPiecePosition, j) && abs(diffX) == 1 && abs(diffY) == 1){
                        return true;
                    }
                } else if(pieceUnderAttack->isUnderAttack(playerPieces[j], board->getPieces())){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Controller::isCorrectDirectionPawnMovement(QPoint to, QPoint from, unsigned int playerIndex)
{
    if((to.y()-from.y() < 0 && playerIndex == 0) || (to.y()-from.y()>0 && playerIndex == 1)){
        return false;
    }else{
        return true;
    }
}

bool Controller::isPawnReachedEndOfBoard(QPoint pos, unsigned int playerIndex)
{
    if((playerIndex == 0 && pos.y() == 8) || (playerIndex == 1 && pos.x() == 1))
        return true;
    else
        return false;
}

void Controller::decreaseElPassantTTLs()
{
    for(int i=0 ; i<ElPassantTTLs.length(); i++){
        if(ElPassantTTLs[i] == 0){
            ElPassantPieces[i] = NULL;
        }else{
            ElPassantTTLs[i]--;
        }
    }
}

void Controller::increaseElPassantTTLs()
{
    for(int i=0 ; i<ElPassantTTLs.length(); i++){
        ElPassantTTLs[i]++;
    }
}

bool Controller::isElPassantPieceMatch(Piece *ptr)
{
    unsigned index = board->getPiecePlayerIndex(ptr->getPosition());
    if(ElPassantPieces[index]->getPosition() == ptr->getPosition()){
        return true;
    }else{
        return false;
    }
}

//Добавить проверку шаха/мата! сделано
//Добавить удаление при сьедении фигуры! сделано
//Реализовать механизм рокировки. сделано
//Ограничить движение пешки только вперед для каждого из игроков. сделано
//Обозначить ситуацию el passant. сделано
//Добавить превращение пешки в фигуру после прохождения доски
void Controller::makeMove(const QPoint &from, const QPoint& to)
{
    if(board!=NULL)
    {
        QList <Piece*> temp = kings;
        Piece* pieceToMove = board->getPlayerPiece(currPlayerIndex, from);
        if(!pieceToMove->isEmpty()){ // "empty" piece movement block
            decreaseElPassantTTLs();
            Piece* pieceToMoveCopy = board->createCopy(from);
            Piece* pieceToEatCopy = board->createCopy(to);
            unsigned int pieceToEatCopyPlayerIndex = board->getPiecePlayerIndex(to);
            if(!pieceToEatCopy->isEmpty() && pieceToEatCopyPlayerIndex != currPlayerIndex && from!=to)
            {
                board->deletePlayerPiece(pieceToEatCopyPlayerIndex, to);
            }

            if(board->changePlayerPiecePosition(currPlayerIndex, from, to)){ //move
                QPoint kingPosition = kings[currPlayerIndex]->getPosition();
                int prevPlayerIndex = currPlayerIndex;
                moveTransmission();

                switch(pieceToMoveCopy->getType())
                {
                case Piece::K:
                    // Casteling condition
                    if(pieceToMoveCopy->getPosition().x() - pieceToMove->getPosition().x() == 2)// left casteling
                    {
                        Piece* kingCopy = board->createCopy(to);
                        board->deletePlayerPiece(prevPlayerIndex, to);
                        board->changePlayerPiecePosition(prevPlayerIndex, QPoint(Board::a, kingCopy->getPosition().y()), QPoint(kingCopy->getPosition().x()+1,kingCopy->getPosition().y()));
                        board->addPlayerPiece(prevPlayerIndex, kingCopy->getType(), kingCopy->getPosition(), kingCopy->getState());
                        kings[prevPlayerIndex] = board->getPlayerPiece(prevPlayerIndex, to);
                        delete kingCopy;
                    }
                    else if(pieceToMoveCopy->getPosition().x() - pieceToMove->getPosition().x() == -2)// right casteling
                    {
                        Piece* kingCopy = board->createCopy(to);
                        board->deletePlayerPiece(prevPlayerIndex, to);
                        board->changePlayerPiecePosition(prevPlayerIndex, QPoint(Board::h, kingCopy->getPosition().y()), QPoint(kingCopy->getPosition().x()-1,kingCopy->getPosition().y()));
                        board->addPlayerPiece(prevPlayerIndex, kingCopy->getType(), kingCopy->getPosition(), kingCopy->getState());
                        kings[prevPlayerIndex] = board->getPlayerPiece(prevPlayerIndex, to);
                        delete kingCopy;
                    }
                    break;
                case Piece::P:{
                    //Pawns movement processing
                    int diffX = to.x() - from.x();
                    int diffY = to.y() - from.y();
                    if((abs(diffY) == 1 && diffX == 0) || (abs(diffY) == 2 && diffX == 0 && pieceToMoveCopy->getState() == Piece::NonMoved)){
                         int diffIntersectsX = pieceToEatCopy->getPosition().x() -  to.x();
                         int diffIntersectsY = pieceToEatCopy->getPosition().y() -  to.y();
                         //back and forward movement processing
                         if( (diffIntersectsX == 0 && diffIntersectsY == 0) || !isCorrectDirectionPawnMovement(to, from, prevPlayerIndex))
                         {
                             if(!isPlayerKingUnderAttack(prevPlayerIndex, kingPosition))// if king is not attacked
                             {
                                 moveBackTransmission();
                                 board->changePlayerPiecePosition(currPlayerIndex, to, from);
                                 pieceToMove->setState(pieceToMoveCopy->getState());
                                 if(!pieceToEatCopy->isEmpty())
                                 {
                                     board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
                                 }
                             }
                         }else{
                             if(abs(diffY) == 2){
                                 ElPassantPieces[prevPlayerIndex] = pieceToMove;
                                 ElPassantTTLs[prevPlayerIndex] = 1;
                             }
                         }
                    } else{
                        if(abs(diffY) == 1 && abs(diffX) == 1)
                        {
                            bool ElPassant = false;
                                 if(pieceToEatCopy->isEmpty()){
                                     delete pieceToEatCopy;
                                     pieceToEatCopy = board->createCopy(QPoint(to.x(), from.y()));
                                     pieceToEatCopyPlayerIndex = board->getPiecePlayerIndex(QPoint(to.x(), from.y()));
                                     ElPassant = true;
                                     if(pieceToEatCopyPlayerIndex == prevPlayerIndex){
                                         ElPassant = false;
                                         pieceToEatCopy = new Empty();
                                         pieceToEatCopyPlayerIndex = UINT_MAX;
                                     }
                                 }
                                 int diffIntersectsX = pieceToEatCopy->getPosition().x()  -  to.x();
                                 int diffIntersectsY = pieceToEatCopy->getPosition().y() -  to.y();
                                 //back and forward pawn movement processing, el passant check
                                 if(!((diffIntersectsX == 0 && diffIntersectsY == 0) ||
                                         (abs(diffIntersectsX) == 0 && abs(diffIntersectsY) == 1)) || !isCorrectDirectionPawnMovement(to, from, prevPlayerIndex)) //  pieceToEatCopy->getType()==Piece::P
                                 {
                                     if(!isPlayerKingUnderAttack(prevPlayerIndex, kingPosition))// if king is not attacked
                                     {
                                         moveBackTransmission();
                                         board->changePlayerPiecePosition(currPlayerIndex, to, from);
                                         pieceToMove->setState(pieceToMoveCopy->getState());
                                         if(!pieceToEatCopy->isEmpty())
                                         {
                                             board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
                                         }
                                     }
                                 } else{
                                     if(ElPassant){
                                         if(isElPassantPieceMatch(pieceToEatCopy)){
                                             board->deletePlayerPiece(pieceToEatCopyPlayerIndex, QPoint(to.x(), from.y()));
                                         }else{
                                             moveBackTransmission();
                                             board->changePlayerPiecePosition(currPlayerIndex, to, from);
                                             pieceToMove->setState(pieceToMoveCopy->getState());
                                             if(!pieceToEatCopy->isEmpty())
                                             {
                                                 board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
                                             }
                                         }
                                     }

                                 }
                        }
                        else
                        {
                            moveBackTransmission();
                            board->changePlayerPiecePosition(currPlayerIndex, to, from);
                            pieceToMove->setState(pieceToMoveCopy->getState());
                            if(!pieceToEatCopy->isEmpty())
                            {
                                board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
                }

                if(isPlayerKingUnderAttack(prevPlayerIndex, kingPosition)){ //check condition
                    moveBackTransmission();
                    board->changePlayerPiecePosition(currPlayerIndex, to, from);
                    pieceToMove->setState(pieceToMoveCopy->getState());
                    if(!pieceToEatCopy->isEmpty())
                    {
                        board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
                    }
                }
            }
            else
            {
                board->addPlayerPiece(pieceToEatCopyPlayerIndex, pieceToEatCopy->getType(), pieceToEatCopy->getPosition(), pieceToEatCopy->getState());
            }
            delete pieceToMoveCopy;
            delete pieceToEatCopy;
            emit moveMade(board->getPlayers(), currPlayerIndex, isPlayerKingUnderAttack(currPlayerIndex, kings[currPlayerIndex]->getPosition()));
        } else {
            delete pieceToMove;
        }
    }
    else{
        qWarning("Game is not initialized!");
    }
}
