#ifndef TCPCLIENTDIALOG_H
#define TCPCLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class TcpClientDialog;
}

class TcpClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpClientDialog(QWidget *parent = nullptr);
    ~TcpClientDialog();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_sendButton_clicked();
    void on_clearButton_clicked();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    Ui::TcpClientDialog *ui;
    QTcpSocket *socket;
    bool ValidateConnect();
};

#endif // TCPCLIENTDIALOG_H
