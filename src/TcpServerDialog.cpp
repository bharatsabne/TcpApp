#include "TcpServerDialog.h"
#include "ui_TcpServerDialog.h"

#include <QMessageBox>

TcpServerDialog::TcpServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpServerDialog),
    server(new QTcpServer(this))
{
    ui->setupUi(this);
    connect(server, &QTcpServer::newConnection, this, &TcpServerDialog::onNewConnection);

    connect(ui->lineEditMessage, &QLineEdit::returnPressed,
            this, &TcpServerDialog::on_sendButton_clicked);
}

TcpServerDialog::~TcpServerDialog()
{
    delete ui;
}

void TcpServerDialog::on_startButton_clicked()
{
    QString portStr = ui->lineEditPort->text().trimmed();

    bool ok;
    quint16 port = portStr.toUShort(&ok);
    if (!ok || port == 0 || port > 65535)
    {
        QMessageBox::warning(this, "Input Error", "Please enter a valid port number (1-65535).");
        return;
    }

    if (server->isListening())
    {
        QMessageBox::information(this, "Server", "Server is already running.");
        return;
    }

    if (server->listen(QHostAddress::Any, port))
    {
        ui->textEditIncoming->append("Server started...");
    }
    else
    {
        QMessageBox::warning(this, "Server Error", "Failed to start server. Port may be in use.");
    }
}

void TcpServerDialog::on_stopButton_clicked()
{
    if (!server->isListening())
    {
        QMessageBox::information(this, "Server", "Server is not running.");
        return;
    }

    server->close();

    for (QTcpSocket *client : clients)
    {
        client->disconnectFromHost();
    }
    clients.clear();

    ui->textEditIncoming->append("Server stopped.");
}

void TcpServerDialog::on_sendButton_clicked()
{
    QString msg = ui->lineEditMessage->text().trimmed();
    if (msg.isEmpty())
    {
        QMessageBox::warning(this, "Send Error", "Please enter a message to send.");
        return;
    }

    if (clients.isEmpty())
    {
        QMessageBox::information(this, "Send", "No clients connected to send the message.");
        return;
    }

    for (QTcpSocket *client : clients)
    {
        if (client->state() == QAbstractSocket::ConnectedState)
        {
            client->write(msg.toUtf8());
        }
    }

    ui->textEditIncoming->append("Server: " + msg);
    ui->lineEditMessage->clear();
}

void TcpServerDialog::on_clearButton_clicked()
{
    ui->textEditIncoming->clear();
}

void TcpServerDialog::onNewConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *client = server->nextPendingConnection();
        clients << client;
        connect(client, &QTcpSocket::readyRead, this, &TcpServerDialog::onReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &TcpServerDialog::onClientDisconnected);
        ui->textEditIncoming->append("Client connected.");
    }
}

void TcpServerDialog::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client)
    {
        clients.removeAll(client);
        client->deleteLater();
        ui->textEditIncoming->append("Client disconnected.");
    }
}

void TcpServerDialog::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client)
    {
        QByteArray data = client->readAll();
        ui->textEditIncoming->append("Client: " + QString::fromUtf8(data));
    }
}
