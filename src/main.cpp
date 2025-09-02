#include "TcpClientDialog.h"
#include "TcpServerDialog.h"
#include "aboutdialog.h"
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainWin;
    mainWin.setWindowTitle("TCP Client & Server");

    QTabWidget *tabs = new QTabWidget(&mainWin);
    TcpClientDialog *client = new TcpClientDialog;
    TcpServerDialog *server = new TcpServerDialog;

    tabs->addTab(client, "Client");
    tabs->addTab(server, "Server");
    mainWin.setCentralWidget(tabs);

    QMenu *helpMenu = mainWin.menuBar()->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About Me");

    QObject::connect(aboutAction, &QAction::triggered, [&]() {
            AboutDialog dlg(&mainWin);
            dlg.exec();
        });

    mainWin.resize(800, 600);
    mainWin.show();
    return a.exec();
}
