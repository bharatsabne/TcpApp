#include "TcpClientDialog.h"
#include "ui_TcpClientDialog.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>

TcpClientDialog::TcpClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClientDialog),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(socket, &QTcpSocket::connected, this, &TcpClientDialog::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClientDialog::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClientDialog::onReadyRead);
    connect(ui->lineEditMessage, &QLineEdit::returnPressed,
            this, &TcpClientDialog::on_sendButton_clicked);
}

TcpClientDialog::~TcpClientDialog()
{
    delete ui;
}
bool TcpClientDialog::ValidateConnect()
{
    QString host = ui->lineEditHost->text().trimmed();
    QString portStr = ui->lineEditPort->text().trimmed();

    // Check if host/IP is empty
    if (host.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid host/IP address.");
        return false;
    }

    // Validate host/IP format
    QHostAddress address;
    if (!address.setAddress(host)) {
        QMessageBox::warning(this, "Input Error", "Invalid IP address or hostname.");
        return false;
    }

    // Validate port
    bool ok;
    quint16 port = portStr.toUShort(&ok);
    if (!ok || port == 0 || port > 65535) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid port number (1-65535).");
        return false;
    }
    return true;
}
void TcpClientDialog::on_connectButton_clicked()
{
    if(ValidateConnect())
    {
        socket->connectToHost(ui->lineEditHost->text().trimmed(), ui->lineEditPort->text().toUShort());
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    }
}
void TcpClientDialog::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QMessageBox::critical(this, "Connection Failed",
                          "Cannot connect to the server.\n"
                          "Please make sure the server is running and IP/port are correct.");
}

void TcpClientDialog::on_disconnectButton_clicked()
{
    if (!socket) return; // safety check

    if (socket->state() == QAbstractSocket::ConnectedState ||
            socket->state() == QAbstractSocket::ConnectingState)
    {
        ui->textEditIncoming->append("Disconnecting...");
        socket->disconnectFromHost();
    }
    else
    {
        QMessageBox::information(this, "Disconnect", "Socket is not connected.");
    }
}

void TcpClientDialog::on_sendButton_clicked()
{
    if (!socket) return; // safety check

    // Check connection
    if (socket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "Send Error", "Not connected to server.");
        return;
    }

    QString message = ui->lineEditMessage->text().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Send Error", "Please enter a message to send.");
        return;
    }

    // Send message
    qint64 bytesSent = socket->write(message.toUtf8());
    if (bytesSent == -1) {
        QMessageBox::warning(this, "Send Error", "Failed to send the message.");
    } else {
        ui->textEditIncoming->append("You: " + message);
        ui->lineEditMessage->clear();
    }
}

void TcpClientDialog::on_clearButton_clicked()
{
    ui->textEditIncoming->clear();
    ui->lineEditMessage->clear();
}

void TcpClientDialog::onConnected()
{
    ui->textEditIncoming->append("Connected to server.");
}

void TcpClientDialog::onDisconnected()
{
    ui->textEditIncoming->append("Disconnected.");
}

void TcpClientDialog::onReadyRead()
{
    QByteArray data = socket->readAll();
    ui->textEditIncoming->append("Server: " + QString::fromUtf8(data));
}
