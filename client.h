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
    void response_addplayer(QNetworkReply*);
    void response_list_of_Players(QNetworkReply*);
    void GetAllPlayers();

    void response_addgame(QNetworkReply*);
    void GetAllGames();
    void response_list_of_Games(QNetworkReply*);

private:
    QComboBox *playerCombo = nullptr;
    QComboBox *gameCombo = nullptr;
    QComboBox *rowCombo = nullptr;
    QComboBox *colCombo = nullptr;
    QLabel *statusLabel = nullptr;
    QScrollArea* gameBoards = nullptr;
    QPushButton *gameMove = nullptr;
    QPushButton *CreatePlayer = nullptr;
    QPushButton *CreateNewGame = nullptr;
    QTreeWidget* listofplayers = nullptr;
    QTreeWidget* gameBoardsList = nullptr;

    QNetworkAccessManager* addPlayer = nullptr;
    QNetworkAccessManager* addGame = nullptr;
    QNetworkAccessManager* PlayerFetcher = nullptr;
    QNetworkAccessManager* GameFetcher = nullptr;
    QTimer* pollingTimer = nullptr;    
};
#endif
