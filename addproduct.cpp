#include "addproduct.h"
#include "ui_addproduct.h"

addproduct::addproduct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addproduct)
{
    ui->setupUi(this);
}

addproduct::~addproduct()
{
    delete ui;
}
