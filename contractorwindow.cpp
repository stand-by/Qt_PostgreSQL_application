#include "contractorwindow.h"
#include "ui_contractorwindow.h"

ContractorWindow::ContractorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContractorWindow)
{
    ui->setupUi(this);
}

ContractorWindow::~ContractorWindow()
{
    delete ui;
}
