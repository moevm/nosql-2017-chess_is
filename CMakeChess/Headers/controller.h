#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "board.h"
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    //enum PawnDirection {Down, Up};
public:
    Controller();
    ~Controller();
    void initializeGame(bool viewType);
    void refreshGame();
    void destroyGame();

private:
    Board* board;
    QList <Piece* > kings;
    QList <Piece*> ElPassantPieces;
    QList <unsigned> ElPassantTTLs;
    unsigned currPlayerIndex;
    void moveTransmission();
    void moveBackTransmission();
    void createWhitePlayer();
    void createBlackPlayer();
    bool isPlayerKingUnderAttack(unsigned int playerIndex, QPoint kingPosition);
    bool isCorrectDirectionPawnMovement(QPoint to, QPoint from, unsigned int playerIndex);
    bool isPawnReachedEndOfBoard(QPoint pos, unsigned int playerIndex);
    void decreaseElPassantTTLs();
    void increaseElPassantTTLs();
    bool isElPassantPieceMatch(Piece* ptr);
public slots:
    void makeMove(const QPoint &from, const QPoint &to);

signals:
    void moveMade(QList <Player* > players, unsigned int currentPlayerIndex, bool isChecked);
};

#endif // CONTROLLER_H
