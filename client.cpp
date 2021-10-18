#include <QtWidgets>
#include <QtNetwork>

#include "client.h"


Client::Client(QWidget *parent)
    : QDialog(parent)
    , playerCombo(new QComboBox)
    , gameCombo(new QComboBox)
    , rowCombo(new QComboBox)
    , colCombo(new QComboBox)
    , CreatePlayer(new QPushButton(tr("Create new Player")))
    , gameMove(new QPushButton(tr("Make A Move")))
    , CreateNewGame(new QPushButton(tr("Create New Game")))    
    , pollingTimer(new QTimer())

{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    pollingTimer->setInterval(1000);

    playerCombo->setEditable(false);
    gameCombo->setEditable(false);
    rowCombo->setEditable(false);
    colCombo->setEditable(false);
    rowCombo->addItem("1");
    rowCombo->addItem("2");
    rowCombo->addItem("3");
    colCombo->addItem("1");
    colCombo->addItem("2");
    colCombo->addItem("3");

    listofplayers = new QTreeWidget(this);
    QTreeWidgetItem *HeaderItem = new QTreeWidgetItem();
    HeaderItem->setText(0,"Player Id");
    HeaderItem->setText(1,"Games Won");
    listofplayers->setHeaderItem(HeaderItem);

    gameBoardsList = new QTreeWidget(this);
    QTreeWidgetItem *HeaderItemBoard = new QTreeWidgetItem();
    HeaderItemBoard->setText(0,"Game Id");
    HeaderItemBoard->setText(1,"Game Board");
    gameBoardsList->setHeaderItem(HeaderItemBoard);
    gameBoardsList->setStyleSheet("QTreeWidget::item {  border-right: 1px solid black; border-bottom: 1px solid black;}");

    gameBoardstatus = new QTreeWidget(this);
    HeaderItemBoard = new QTreeWidgetItem();
    HeaderItemBoard->setText(0,"Game Id");
    HeaderItemBoard->setText(1,"Players");
    HeaderItemBoard->setText(2,"Last Played");
    HeaderItemBoard->setText(3,"Status");
    gameBoardstatus->setHeaderItem(HeaderItemBoard);
    gameBoardstatus->setStyleSheet("QTreeWidget::item {  border-right: 1px solid black; border-bottom: 1px solid black;}");


    auto playerID = new QLabel(tr("&Player ID:"));
    auto gameID = new QLabel(tr("&Game ID:"));
    auto row_label = new QLabel(tr("&Row Index:"));
    auto col_label = new QLabel(tr("&Column Index:"));
    playerID->setBuddy(playerCombo);
    gameID->setBuddy(gameCombo);
    row_label->setBuddy(rowCombo);
    col_label->setBuddy(colCombo);

    statusLabel = new QLabel(tr("Responses will be displayed here."));

    gameMove->setDefault(true);
    gameMove->setEnabled(true);

    CreateNewGame->setDefault(true);
    CreateNewGame->setEnabled(true);

    CreatePlayer->setDefault(true);
    CreatePlayer->setEnabled(true);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(CreatePlayer, QDialogButtonBox::ActionRole);
    buttonBox->addButton(CreateNewGame, QDialogButtonBox::ActionRole);
    buttonBox->addButton(gameMove, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    addPlayer = new QNetworkAccessManager(this);
    addGame = new QNetworkAccessManager(this);
    PlayerFetcher = new QNetworkAccessManager(this);
    GameFetcher = new QNetworkAccessManager(this);
    GameStatusFetcher = new QNetworkAccessManager(this);
    connect(addPlayer, &QNetworkAccessManager::finished, this, &Client::response_addplayer);
    connect(PlayerFetcher, &QNetworkAccessManager::finished, this, &Client::response_list_of_Players);
    connect(addGame, &QNetworkAccessManager::finished, this, &Client::response_addgame);
    connect(GameFetcher, &QNetworkAccessManager::finished, this, &Client::response_list_of_Games);
    connect(GameStatusFetcher, &QNetworkAccessManager::finished, this, &Client::response_status_of_Games);
    connect(pollingTimer, &QTimer::timeout, this, &Client::GetAllGames);
    connect(pollingTimer, &QTimer::timeout, this, &Client::GetAllPlayers);
    connect(pollingTimer, &QTimer::timeout, this, &Client::GetAllGameStatus);



    connect(gameMove, &QAbstractButton::clicked, this, &Client::makeMove);
    connect(CreatePlayer, &QAbstractButton::clicked, this, &Client::CreatePlayerHandler);
    connect(CreateNewGame, &QAbstractButton::clicked, this, &Client::CreateGameHander);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(playerID, 0, 0);
    mainLayout->addWidget(playerCombo, 0, 1);
    mainLayout->addWidget(gameID, 1, 0);
    mainLayout->addWidget(gameCombo, 1, 1);
    mainLayout->addWidget(row_label,2,0);
    mainLayout->addWidget(rowCombo,2,1);
    mainLayout->addWidget(col_label,3,0);
    mainLayout->addWidget(colCombo,3,1);
    mainLayout->addWidget(gameBoardsList,5,1,10,1);
    mainLayout->addWidget(gameBoardstatus,5,0,10,1);
    mainLayout->addWidget(statusLabel, 16, 0);//, 2, 2);
    mainLayout->addWidget(buttonBox,17, 0);//, 1, 2);
    mainLayout->addWidget(listofplayers,0,2,4,2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    pollingTimer->start();
}

QString Client::BoardCharacter(int asci_value)
{
    QString returnchar;
    if(asci_value==46)
        returnchar = QString("   ");
    if(asci_value==88)
        returnchar = QString("X");
    if(asci_value==79)
        returnchar = QString("O");
    return returnchar;
}

QString Client::getPlayerIdStrings(int number1,int number2)
{
    QString returnchar;
    if(number1 != -1)
        returnchar = QString::number(number1);
    if(number1 != -1 && number2 != -1)
        returnchar += " , "+QString::number(number2);
    else if(number1 == -1 && number2 != -1)
        returnchar = QString::number(number2);

    return returnchar;
}

QString Client::getPlayerId(int number)
{
    QString returnchar;
    if(number != -1)
        returnchar = QString::number(number);
    return returnchar;
}

/* APIs and handler to create players and fetch their responses */

void Client::CreatePlayerHandler()
{
  QByteArray data= "";
  QNetworkRequest request(QUrl(serverpath+"newplayer"));
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  addPlayer->post(request,data);
}

void Client::response_addplayer(QNetworkReply* reply)
{
  QByteArray response =  reply->readAll();
  statusLabel->setText(QJsonDocument::fromJson(response).object().value("result").toString());
  GetAllPlayers();
}

void Client::response_list_of_Players(QNetworkReply* reply)
{
    QByteArray response =  reply->readAll();
    QJsonObject document = QJsonDocument::fromJson(response).object();
    QJsonValue data = document.value("result");
    QJsonObject data_Players = data.toObject();
    QStringList list_of_Players = data_Players.keys();
    int current = playerCombo->currentIndex();
    int n = list_of_Players.length();
    playerCombo->setEditable(false);
    playerCombo->clear();
    listofplayers->clear();
    for(int i = 0; i<n;++i)
    {
      playerCombo->addItem(list_of_Players[i]);
      QTreeWidgetItem* playerRow = new QTreeWidgetItem();
      int win_score = data_Players.value(list_of_Players[i]).toInt() ;
      playerRow->setText(0,list_of_Players[i]);
      playerRow->setText(1,QString::number(win_score));
      listofplayers->insertTopLevelItem(0,playerRow);
    }
    listofplayers->sortByColumn(1,Qt::DescendingOrder);
    playerCombo->setCurrentIndex(current);
}

void Client::GetAllPlayers()
{
    QNetworkRequest request(QUrl(serverpath+"getplayerinfo"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    PlayerFetcher->get(request);
}



/* APIs and handler to create games and fetch their responses */
void Client::response_list_of_Games(QNetworkReply* reply)
{
    QByteArray response =  reply->readAll();
    QJsonObject document = QJsonDocument::fromJson(response).object();
    QJsonValue data = document.value("result");
    QJsonObject gameMap = data.toObject();
    QStringList list_of_games = gameMap.keys();
    int current = gameCombo->currentIndex();
    int n = list_of_games.length();
    gameCombo->setEditable(false);
    gameCombo->clear();
    gameBoardsList->clear();
    for(int i = 0; i<n;++i)
    {
      gameCombo->addItem(list_of_games[i]);
      QTreeWidgetItem* gameboardRow = new QTreeWidgetItem();
      QJsonArray board = gameMap.value(list_of_games[i]).toArray();
      QJsonArray r1 = board[0].toArray();
      QJsonArray r2 = board[1].toArray();
      QJsonArray r3 = board[2].toArray();
      QString linemid("-------------");
      QString line1 = BoardCharacter(r1[0].toInt()) + " | " +  BoardCharacter(r1[1].toInt()) + " | " +  BoardCharacter(r1[2].toInt());
      QString line2 = BoardCharacter(r2[0].toInt()) + " | " +  BoardCharacter(r2[1].toInt()) + " | " +  BoardCharacter(r2[2].toInt());
      QString line3 = BoardCharacter(r3[0].toInt()) + " | " +  BoardCharacter(r3[1].toInt()) + " | " +  BoardCharacter(r3[2].toInt());
      gameboardRow->setText(0,QString(list_of_games[i]));
      gameboardRow->setText(1,QString("\n"+line1+"\n"+linemid+"\n"+line2+"\n"+linemid+"\n"+line3+"\n"));
      gameBoardsList->insertTopLevelItem(0,gameboardRow);
    }
    gameCombo->setCurrentIndex(current);
}

void Client::GetAllGames()
{
    QNetworkRequest request(QUrl(serverpath+"getallgames"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    GameFetcher->get(request);
}


void Client::response_status_of_Games(QNetworkReply* reply)
{
    QByteArray response =  reply->readAll();
    QJsonObject document = QJsonDocument::fromJson(response).object();
    QJsonValue data = document.value("result");
    QJsonObject gameMap = data.toObject();
    gameBoardstatus->clear();
    QStringList list_of_games = gameMap.keys();
    int n = list_of_games.length();
    for(int i = 0; i<n;++i)
    {
      QTreeWidgetItem* gameRow = new QTreeWidgetItem();
      QJsonArray board = gameMap.value(list_of_games[i]).toArray();
      QString players = getPlayerIdStrings(board[0].toInt() ,board[1].toInt());
      gameRow->setText(0,QString(list_of_games[i]));
      gameRow->setText(1,players);
      gameRow->setText(2,getPlayerId(board[2].toInt()));
      if( board[3].toInt() == -1)
          gameRow->setText(3,QString("ONgoing"));
      else if( board[3].toInt() == 2)
          gameRow->setText(3,QString("Over,Draw"));
      else if( board[3].toInt() == 0 || board[3].toInt() == 1)
          gameRow->setText(3,QString("Over,Winner : ") + getPlayerId(board[board[3].toInt()].toInt()));
      gameBoardstatus->insertTopLevelItem(0,gameRow);
    }
}

void Client::GetAllGameStatus()
{
    QNetworkRequest request(QUrl(serverpath+"getallgamesstatus"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    GameStatusFetcher->get(request);
}

void Client::response_addgame(QNetworkReply* reply)
{
  QByteArray response =  reply->readAll();
  statusLabel->setText(QJsonDocument::fromJson(response).object().value("result").toString());
  GetAllGames();
}

void Client::CreateGameHander()
{
  QByteArray data = "";
  QString full_api = serverpath+"createnewgame";
  QNetworkRequest request(QUrl(serverpath+"createnewgame"));
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  addGame->post(request,data);
}

void Client::makeMove()
{
  QByteArray data = "";
  QString api = serverpath+"makeamove";
  QString player(playerCombo->currentText());
  QString gameid(gameCombo->currentText());
  QString rowIdx(rowCombo->currentText());
  QString colIds(colCombo->currentText());
  const QString full_api = api + "/" + gameid + "/" + player + "/" + rowIdx + "/" + colIds;
  QUrl path(full_api);
  QNetworkRequest request(path);
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  addGame->post(request,data);
}

