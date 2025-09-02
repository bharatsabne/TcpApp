#ifndef TCPSERVERDIALOG_H
#define TCPSERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class TcpServerDialog;
}

class TcpServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpServerDialog(QWidget *parent = nullptr);
    ~TcpServerDialog();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_sendButton_clicked();
    void on_clearButton_clicked();

    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    Ui::TcpServerDialog *ui;
    QTcpServer *server;
    QList<QTcpSocket*> clients;
};

#endif // TCPSERVERDIALOG_H
