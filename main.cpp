
#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("TicTacToe Client"));
    Client client;
    client.show();
    return app.exec();
}
