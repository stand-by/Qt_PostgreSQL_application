#ifndef PRODUCTPICKER_H
#define PRODUCTPICKER_H

#include <QDialog>
#include <QtSql>
#include "modalformbase.h"
#include "addproduct.h"

namespace Ui {
class ProductPicker;
}

//class for choosing product from product list
class ProductPicker : public QDialog, public ModalFormBase {
    Q_OBJECT

public:
    explicit ProductPicker(QWidget *parent, QSqlDatabase db);
    ~ProductPicker();

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

    bool is_searching;

    int product_id;
    QString product_name;
    QString product_type;

    void enable_search();
    void disable_search();
};

#endif // PRODUCTPICKER_H
