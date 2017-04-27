#include "productpicker.h"
#include "ui_productpicker.h"

ProductPicker::ProductPicker(QWidget *parent): QDialog(parent), ui(new Ui::ProductPicker) {
    ui->setupUi(this);
}

ProductPicker::~ProductPicker() {
    delete ui;
}
