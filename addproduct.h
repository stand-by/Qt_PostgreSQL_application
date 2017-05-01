#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class addproduct;
}

//class for adding new product and product type
class addproduct : public QDialog {
    Q_OBJECT

public:
    explicit addproduct(QWidget *parent, QSqlDatabase db);
    ~addproduct();
    bool is_valid();

private slots:
    void on_button_box_accepted();
    void on_button_add_type_clicked();

private:
    Ui::addproduct *ui;

    bool insert_new_type;
    bool is_filled;
    int type_id;

    QSqlQueryModel *model_names;
    QSqlQueryModel *model_ids;
    QSqlDatabase db;

    void prompt_error(QString text, bool exit_flag = false);
    void switch_modes();
    void perform_type_insert();
    void perform_product_insert();
};

#endif // ADDPRODUCT_H
