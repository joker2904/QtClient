#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QPushButton;
class QScrollArea;
class QTreeWidget;
class QString;

QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void CreatePlayerHandler();
    void CreateGameHander();
    void makeMove();

    QString BoardCharacter(int);
    QString getPlayerIdStrings(int,int);
    QString getPlayerId(int);
    void response_addplayer(QNetworkReply*);
    void response_list_of_Players(QNetworkReply*);
    void GetAllPlayers();

    void response_addgame(QNetworkReply*);
    void GetAllGames();
    void response_list_of_Games(QNetworkReply*);
    void GetAllGameStatus();
    void response_status_of_Games(QNetworkReply*);

private:
    QComboBox *playerCombo = nullptr;
    QComboBox *gameCombo = nullptr;
    QComboBox *rowCombo = nullptr;
    QComboBox *colCombo = nullptr;
    QLabel *statusLabel = nullptr;   
    QPushButton *gameMove = nullptr;
    QPushButton *CreatePlayer = nullptr;
    QPushButton *CreateNewGame = nullptr;
    QTreeWidget* listofplayers = nullptr;
    QTreeWidget* gameBoardsList = nullptr;
    QTreeWidget* gameBoardstatus = nullptr;

    QNetworkAccessManager* addPlayer = nullptr;
    QNetworkAccessManager* addGame = nullptr;
    QNetworkAccessManager* PlayerFetcher = nullptr;
    QNetworkAccessManager* GameFetcher = nullptr;
    QNetworkAccessManager* GameStatusFetcher = nullptr;
    QTimer* pollingTimer = nullptr;    
    const QString serverpath = "http://localhost:9098/tictacserver/";
};
#endif
