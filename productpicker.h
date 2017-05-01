#ifndef PRODUCTPICKER_H
#define PRODUCTPICKER_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include "addproduct.h"

namespace Ui {
class ProductPicker;
}

//class for choosing product from product list
class ProductPicker : public QDialog {
    Q_OBJECT

public:
    explicit ProductPicker(QWidget *parent, QSqlDatabase db);
    ~ProductPicker();

    bool is_valid();

    //methods for obtaining necessary info about selected product
    int pick_product_id();
    QString pick_product_name();
    QString pick_product_type();

private slots:
    void on_pushButton_addproduct_clicked();
    void on_buttonBox_accepted();
    void on_pushButton_search_clicked();

private:
    Ui::ProductPicker *ui;
    QSqlDatabase db;

    bool is_searching;
    bool is_filled;

    int product_id;
    QString product_name;
    QString product_type;

    void enable_search();
    void disable_search();
    void prompt_error(QString text, bool exit_flag = false);
};

#endif // PRODUCTPICKER_H
