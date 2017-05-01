#ifndef MOVEORDER_H
#define MOVEORDER_H

#include <QDialog>
#include <QtSql>
#include <QSpinBox>
#include "modalformbase.h"
#include "contractorwindow.h"
#include "productpicker.h"

namespace Ui {
class MoveOrder;
}

//class that allows to create move order for goods
class MoveOrder : public QDialog, public ModalFormBase {
    Q_OBJECT

public:
    explicit MoveOrder(QWidget *parent, QSqlDatabase db_);
    ~MoveOrder();

private slots:
    void on_buttonBox_accepted();
    void on_button_addproduct_clicked();

private:
    Ui::MoveOrder *ui;

    void config_table();
    void append_to_goods_table(int id, QString name, QString type);
};

#endif // MOVEORDER_H
