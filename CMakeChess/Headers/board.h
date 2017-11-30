#ifndef BOARD_H
#define BOARD_H

#include <Headers/player.h>

class Board
{
public:
    enum l_letters {a = 1, b, c, d, e, f, g, h};
    static Board* CreateBoard(bool isGraphic);
    bool getViewType();
    unsigned getPlayersAmount();

    Board(){}
    ~Board();
    const QList <Piece*> getPieces();
    const QList <Player* > getPlayers();
    void addPlayer();
    bool deletePlayer(const int &playerIndex);
    bool addPlayerPiece(const int &playerIndex, Piece::PieceType p, QPoint pos, Piece::PieceState state = Piece::NonMoved);
    Piece* getPlayerPiece(const int &playerIndex, const QPoint &pos);
    bool changePlayerPiecePosition(const int &playerIndex, const QPoint &from, const QPoint &to);
    bool deletePlayerPiece(const int &playerIndex, const QPoint &pos);
    //bool isPlayerPieceUnderAttack(const int &playerIndex, const QPoint &pos);
    int getPiecePlayerIndex(const QPoint &pos);
    Piece* createCopy(const QPoint &pos); //creates copy of board piece which not connected with board
private:
    QList <Player* > players;
    bool viewType; // false means simple(console) and true means graphic
    bool isPlayerIndexCorrect(const int &playerIndex);
    bool isOnBoard(QPoint pos);
    //Factory methods
    static Board *CreateSimpleBoard(bool boardType);
    // Empty graphic factory method
    static Board *CreateGraphicBoard(bool boardType);
};

#endif // BOARD_H
