#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->label->setText("TCP Client/Server Utility\n"
                       "Created by: Bharat Sabne\n"
                       "Email: bharatsabne1991@gmail.com\n"
                       "Version: 1.0");

}

AboutDialog::~AboutDialog()
{
    delete ui;
}
