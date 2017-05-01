#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSpinBox>
#include "contractorwindow.h"
#include "productpicker.h"

namespace Ui {
class OrderWindow;
}

//class for filling purchase type or sell type of order
class OrderWindow : public QDialog {
    Q_OBJECT

public:
    explicit OrderWindow(QWidget *parent, QSqlDatabase db_, bool flag);
    ~OrderWindow();
    bool is_valid();

private slots:
    void on_button_addcontractor_clicked();
    void on_button_addproduct_clicked();
    void on_buttonBox_accepted();

private:
    Ui::OrderWindow *ui;
    QSqlDatabase db;

    QSqlQueryModel *model_names;
    QSqlQueryModel *model_ids;

    bool is_filled;
    bool is_purchase_mode;

    void config_table();
    void prompt_error(QString text, bool exit_flag = false);
    void fill_contractors_combobox();
    void append_to_goods_table(int id, QString name, QString type);
    double get_minimal_price(int id_good);
};

#endif // ORDERWINDOW_H
