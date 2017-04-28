#include "orderwindow.h"
#include "ui_orderwindow.h"

OrderWindow::OrderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderWindow)
{
    ui->setupUi(this);
}

OrderWindow::~OrderWindow()
{
    delete ui;
}
