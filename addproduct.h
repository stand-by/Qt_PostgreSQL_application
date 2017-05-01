#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QDialog>
#include <QtSql>
#include "modalformbase.h"

namespace Ui {
class addproduct;
}

//class for adding new product and product type
class addproduct : public QDialog, public ModalFormBase {
    Q_OBJECT

public:
    explicit addproduct(QWidget *parent, QSqlDatabase db);
    ~addproduct();

private slots:
    void on_button_box_accepted();
    void on_button_add_type_clicked();

private:
    Ui::addproduct *ui;

    bool insert_new_type;
    int type_id;

    QSqlQueryModel *model_names;
    QSqlQueryModel *model_ids;

    void switch_modes();
    void perform_type_insert();
    void perform_product_insert();
};

#endif // ADDPRODUCT_H
