#include "Headers/boardvision.h"

BoardVision::BoardVision(QWidget *widget)
{
    int i,j,k=0,hor,ver;
    Border *border[4]={ NULL };
    baseWidget=widget;
    //borderDisplay
    {
    border[0]->outline(baseWidget,80,45,0);
    border[1]->outline(baseWidget,80,577,0);
    border[2]->outline(baseWidget,80,65,1);
    border[2]->outline(baseWidget,612,65,1);
    }
    kLabel = new QLabel("",baseWidget);
    kLabel->setGeometry(300,20,40,20);

    //Create 64 tiles (allocating memories to the objects of Tile class)
    ver=65;
    color= new Tile(baseWidget);
    color->tileColor=0;
    color->piece=0;
    color->row=0;
    color->col=0;
    color->tileNum=0;
    color->tileDisplay();
    color->setGeometry(30,620,50,50);
    QLabel *playerLabel = new QLabel("РРіСЂРѕРє:",baseWidget);
    playerLabel->setGeometry(30,590,60,20);
    for(i=7;i>=0;i--)
    {
        hor=100;
        for(j=0;j<8;j++)
        {
            tile[i][j] = new Tile(baseWidget);
            tile[i][j]->tileColor=(i+j)%2;
            tile[i][j]->piece=0;
            tile[i][j]->row=i+1;
            tile[i][j]->col=j+1;
            tile[i][j]->tileNum=k++;
            tile[i][j]->tileDisplay();
            tile[i][j]->setGeometry(hor,ver,64,64);
            connect(tile[i][j],SIGNAL( clicked(QPoint) ), this, SLOT( tileClicked(QPoint) ) );
            hor+=64;
        }
        ver+=64;
    }

    //b pawns
    c = new Controller();
    c->initializeGame(false);
    connect(this,SIGNAL(wantMove(QPoint,QPoint)),c,SLOT(makeMove(QPoint,QPoint)) );
    connect(c,SIGNAL(moveMade(QList<Player*>,unsigned int,bool)),this,SLOT(setupedMove(QList<Player*>,unsigned int,bool)));
}

BoardVision::~BoardVision(){
    db->deleteTable(currTable);
}

void BoardVision::buttons(){
    QLabel *partLabel = new QLabel("РџР°СЂС‚РёРё:",baseWidget);
    partLabel->setGeometry(930,570,80,20);
    listWgt = new QListWidget(baseWidget);
    listWgt->setGeometry(920,590,110,70);
    listWgt->addItems(db->tableList(currTable));
    connect( listWgt, SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onListClicked(QListWidgetItem*)));
    QDate* d=new QDate();
    currTable="Save_"+QString::number(d->currentDate().day())+'_'+QString::number(d->currentDate().month());
    if(db->containsTable(currTable))
    {
        db->deleteTable(currTable);
    }
    db->createTable(currTable);
    //qDebug() << db->tableList("1");

    QPushButton *dButton= new QPushButton("Р—Р°РіСЂСѓР·РёС‚СЊ РїР°СЂС‚РёСЋ",baseWidget);
    dButton->setGeometry(700,590,150,20);
    connect( dButton, SIGNAL(clicked()),this,SLOT(moveList()));

    //QPushButton *backButton= new QPushButton("<",baseWidget);
    //backButton->setGeometry(400,600,50,20);
   // connect( backButton, SIGNAL(clicked()),this,SLOT(backMove()));
    QPushButton *nextButton= new QPushButton(">",baseWidget);
    nextButton->setGeometry(550,600,50,20);
    connect( nextButton, SIGNAL(clicked()),this,SLOT(nextMove()));
    //if(movesTable==nullptr) nextButton->setEnabled(false);
    //else nextButton->setEnabled(true);

    QPushButton *clearButton= new QPushButton("РћС‡РёСЃС‚РёС‚СЊ",baseWidget);
    clearButton->setGeometry(455,600,90,20);
    connect( clearButton, SIGNAL(clicked()),this,SLOT(clearBoard()));

   QPushButton *saButton= new QPushButton("РЎРѕС…СЂР°РЅРёС‚СЊ РїР°СЂС‚РёСЋ",baseWidget);
    saButton->setGeometry(700,620,150,20);
    connect( saButton, SIGNAL(clicked()),this,SLOT(savedMoves()));
   QPushButton *seButton= new QPushButton("РЈРґР°Р»РёС‚СЊ РїР°СЂС‚РёСЋ",baseWidget);
   seButton->setGeometry(700,650,150,20);
   connect( seButton, SIGNAL(clicked()),this,SLOT(deletedMoves()));
   QPushButton *deButton= new QPushButton("РџРѕРёСЃРє РїРѕ РґРµР±СЋС‚Сѓ",baseWidget);
   deButton->setGeometry(700,680,150,20);
   connect( deButton, SIGNAL(clicked()),this,SLOT(findDeb()));
   //connect( this, SIGNAL(closeEvent(QCloseEvent*)),this,SLOT(deletedCurrTable()));
   QComboBox *fd =new QComboBox(baseWidget);
   fd->setGeometry(930,680,80,20);
   fd->addItem("РћС‚РєСЂС‹С‚С‹Рµ РґРµР±СЋС‚С‹");
   fd->addItem("Р—Р°РєСЂС‹С‚С‹Рµ РґРµР±СЋС‚С‹");
   connect( fd, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
       if(i==0){
           listWgt->clear();
           QVector<QPoint> deb;
           deb<<QPoint(5,2)<<QPoint(5,4)<<QPoint(5,7)<<QPoint(5,5);
           listWgt->addItems(db->filterDebute(deb, currTable)); }
       else if(i==1){
               listWgt->clear();
               QVector<QPoint> deb;
               deb<<QPoint(4,2)<<QPoint(4,4)<<QPoint(4,7)<<QPoint(4,5);
               listWgt->addItems(db->filterDebute(deb, currTable));}
   });
   fd->show();
}

void BoardVision::filteredDeb(){

}
void BoardVision::savedMoves(){
    listWgt->addItem(currTable);
    saveInd++;
    QDate* d=new QDate();
    currTable="Save_"+QString::number(d->currentDate().day())+'_'+QString::number(d->currentDate().month())+"_"+QString::number(saveInd);
    db->createTable(currTable);
}

void BoardVision::deletedMoves(){
    listWgt->clear();
    db->deleteTable(table);

    listWgt->addItems(db->tableList(currTable));
}

void BoardVision::deletedCurrTable(){
    db->deleteTable(currTable);

}

void BoardVision::findDeb(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("РџРѕРёСЃРє Р·Р°РІРµСЂС€РµРЅ");
    int n=db->recordCount(currTable);
        QVector<QPoint> moves;
    for(int i=0;i<n;i++)
        moves<<db->readMove(currTable,i);
    QString t=db->searchDebute(moves,currTable);
    if(t!="false"){
        qDebug() <<t;
        table=t;
        moveList();
        msgBox.setText(t);
    } else msgBox.setText("РќРµСѓРґР°С‡РЅРѕ");
    msgBox.exec();
}
void BoardVision::initBoard(){
    for(int i=7;i>=0;i--)
    {
        for(int j=0;j<8;j++)
        {
            tile[i][j]->piece=0;
            tile[i][j]->clear();
        }
     }
    for(int j=0;j<8;j++)
    {
        tile[1][j]->piece=1;
        tile[1][j]->pieceColor=1;
        tile[1][j]->display(Piece::P);
         //w pawns
        tile[6][j]->piece=1;
        tile[6][j]->pieceColor=0;
        tile[6][j]->display(Piece::P);
    }
    for(int j=0;j<8;j++)
    {
        tile[0][j]->piece=1;
        tile[0][j]->pieceColor=1;
        tile[7][j]->piece=1;
        tile[7][j]->pieceColor=0;
    }
    {
    tile[0][0]->display(Piece::R);
    tile[0][1]->display(Piece::N);
    tile[0][2]->display(Piece::B);
    tile[0][3]->display(Piece::Q);
    tile[0][4]->display(Piece::K);
    tile[0][5]->display(Piece::B);
    tile[0][6]->display(Piece::N);
    tile[0][7]->display(Piece::R);
    }

    {
    tile[7][0]->display(Piece::R);
    tile[7][1]->display(Piece::N);
    tile[7][2]->display(Piece::B);
    tile[7][3]->display(Piece::Q);
    tile[7][4]->display(Piece::K);
    tile[7][5]->display(Piece::B);
    tile[7][6]->display(Piece::N);
    tile[7][7]->display(Piece::R);
    }
}

void BoardVision::clearBoard(){
    c->refreshGame();
    index=-1;
    db->deleteTable(currTable);
    db->createTable(currTable);
}

void BoardVision::backMove(){
    if(index>-1){
        QVector<QPoint> moves ;
        index--;
        moves=db->readMove(table,index && table !="");
        movesTable->selectRow(index);
        emit wantMove(moves[1],moves[0]);
    }
}

void BoardVision::nextMove(){
    if(index<db->recordCount(table) && table !=""){
        QVector<QPoint> moves ;
    index++;
    moves=db->readMove(table,index);
    movesTable->selectRow(index);
    db->writeMove(currTable,moves[0],moves[1]);
    emit wantMove(moves[0],moves[1]);
    }
}
void BoardVision::setupedMove(QList<Player*> pl,unsigned int play, bool k){

    QFont font = kLabel->font();
    font.setPointSize(14);
    kLabel->setFont(font);
    kLabel->show();
    if(k)
       kLabel->setText("РЁР°С…!");
    else
       kLabel->clear();
    for(int i=7;i>=0;i--)
    {
        for(int j=0;j<8;j++)
        {
            tile[i][j]->piece=0;
            tile[i][j]->clear();
        }
     }
    color->tileColor=play;
    color->tileDisplay();
    for (int i=0; i<pl[0]->getPieces().length();i++){
    tile[pl[0]->getPieces()[i]->getPosition().y()-1][pl[0]->getPieces()[i]->getPosition().x()-1]->pieceColor=true;
    tile[pl[0]->getPieces()[i]->getPosition().y()-1][pl[0]->getPieces()[i]->getPosition().x()-1]->piece=1;
    tile[pl[0]->getPieces()[i]->getPosition().y()-1][pl[0]->getPieces()[i]->getPosition().x()-1]->display(pl[0]->getPieces()[i]->getType());
    }
    for (int i=0; i<pl[1]->getPieces().length();i++){
    tile[pl[1]->getPieces()[i]->getPosition().y()-1][pl[1]->getPieces()[i]->getPosition().x()-1]->pieceColor=false;
    tile[pl[1]->getPieces()[i]->getPosition().y()-1][pl[1]->getPieces()[i]->getPosition().x()-1]->piece=1;
    tile[pl[1]->getPieces()[i]->getPosition().y()-1][pl[1]->getPieces()[i]->getPosition().x()-1]->display(pl[1]->getPieces()[i]->getType());
    }
}

void BoardVision::tileClicked(QPoint p)
{
    fromto <<p;
    if(tile[fromto[0].y()-1][fromto[0].x()-1]->piece){
        tile[fromto[0].y()-1][fromto[0].x()-1]->checked=true;
        tile[fromto[0].y()-1][fromto[0].x()-1]->tileDisplay();
    }
    if(fromto.length()>0 && fromto[0]!=p){
        count++;
        if(count==2){
            count=1;
            if(tile[fromto[0].y()-1][fromto[0].x()-1]->piece)
            {
                tile[p.y()-1][p.x()-1]->checked=true;
                tile[p.y()-1][p.x()-1]->tileDisplay();
                db->writeMove(currTable,fromto[0],p);
                emit wantMove(fromto[0],p);
                tile[p.y()-1][p.x()-1]->checked=false;
                tile[p.y()-1][p.x()-1]->tileDisplay();
                tile[fromto[0].y()-1][fromto[0].x()-1]->checked=false;
                tile[fromto[0].y()-1][fromto[0].x()-1]->tileDisplay();
            }
            fromto.clear();
        }
    }
}

void BoardVision::moveList()
{
    if (table =="") return;
    movesTable = new QTableWidget(db->recordCount(table),2,baseWidget);
    movesTable->setGeometry(660,35,250,550);
    movesTable->setStyleSheet("QLabel {background-color: white;}");
    movesTable->setHorizontalHeaderLabels(QStringList() << "РћС‚РєСѓРґР°" << "РљСѓРґР°");
    QVector<QPoint> p ;
    for(int row=0; row!=movesTable->rowCount(); ++row){
        p=db->readMove(table,row);
        QTableWidgetItem *newItem = new QTableWidgetItem(db->intToChar(p[0].x())+QString::number(p[0].y()),QTableWidgetItem::Type);
        movesTable->setItem(row, 0, newItem);
        newItem = new QTableWidgetItem(db->intToChar(p[1].x())+QString::number(p[1].y()),QTableWidgetItem::Type);
        movesTable->setItem(row, 1, newItem);
    }
    movesTable->show();
}
void BoardVision::onListClicked(QListWidgetItem *item){
    table=item->text();
}
