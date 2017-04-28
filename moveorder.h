#ifndef MOVEORDER_H
#define MOVEORDER_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "contractorwindow.h"
#include "productpicker.h"
#include "spinboxdelegate.h"

namespace Ui {
class MoveOrder;
}

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
    SpinBoxDelegate *spin_delegate;

    bool is_filled;

    void config();
    void prompt_error(QString text, bool exit_flag = false);
    void append_to_goods_table(int id, QString name, QString type);
};

#endif // MOVEORDER_H
