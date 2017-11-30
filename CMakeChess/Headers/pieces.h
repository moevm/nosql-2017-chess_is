#ifndef PIECES_H
#define PIECES_H
#include <QPoint>
#include <QList>


class Piece
{
public:
    enum PieceType {K, Q, R, B, N, P, E}; //king, queen, rook, bishop, knight, pawn, empty piece
    enum PieceState {Moved, NonMoved};
    virtual ~Piece();
    static Piece *CreatePiece(bool isGraphic,PieceType type, QPoint pos = QPoint(), PieceState state = NonMoved);
    QPoint getPosition();
    void setPosition(QPoint pos);
    void setState(PieceState state);
    PieceState getState();
    PieceType getType();
    bool isEmpty();
    bool isUnderAttack(Piece* attackingPiece, const QList<Piece *> &allPieces);
    virtual bool MovePattern(QPoint newPos, const QList <Piece*> &pieces) = 0;
private:
    //Factory methods
    static Piece *CreateSimplePiece(PieceType type, QPoint pos = QPoint(), PieceState state = NonMoved);
    // Empty graphic factory method
    static Piece *CreateGraphicPiece(PieceType type, QPoint pos = QPoint(), PieceState state = NonMoved);
protected:
    bool isSamePosition(QPoint pos);
    int sign(int n);
    QPoint pos;
    PieceType type;
    PieceState state;
};

class King : public Piece
{
    public:
    King(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~King();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Queen : public Piece
{
    public:
    Queen(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~Queen();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Rook : public Piece
{
    public:
    Rook(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~Rook();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Bishop : public Piece
{
    public:
    Bishop(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~Bishop();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Knight : public Piece
{
    public:
    Knight(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~Knight();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Pawn : public Piece
{
    public:
    Pawn(QPoint pos = QPoint(), PieceState state = NonMoved);
    ~Pawn();
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces);
};

class Empty : public Piece{
    public:
    Empty(){type = E;}
    ~Empty(){}
    bool MovePattern(QPoint newPos, const QList <Piece*> &pieces){return false;}
};

#endif // PIECES_H
