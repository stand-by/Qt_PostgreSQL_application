#ifndef MOVEORDER_H
#define MOVEORDER_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSpinBox>
#include "contractorwindow.h"
#include "productpicker.h"

namespace Ui {
class MoveOrder;
}

//class that allows to create move order for goods
class MoveOrder : public QDialog {
    Q_OBJECT

public:
    explicit MoveOrder(QWidget *parent, QSqlDatabase db_);
    ~MoveOrder();
    bool is_valid();

private slots:
    void on_buttonBox_accepted();
    void on_button_addproduct_clicked();

private:
    Ui::MoveOrder *ui;
    QSqlDatabase db;

    bool is_filled;

    void config_table();
    void prompt_error(QString text, bool exit_flag = false);
    void append_to_goods_table(int id, QString name, QString type);
};

#endif // MOVEORDER_H
