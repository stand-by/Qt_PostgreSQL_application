#ifndef PRODUCTPICKER_H
#define PRODUCTPICKER_H

#include <QDialog>
#include <QtSql>
#include <QSqlQueryModel>
#include <QDesktopWidget>
#include <QMessageBox>
#include "addproduct.h"
#include "tools.h"

namespace Ui {
class ProductPicker;
}

class ProductPicker : public QDialog {
    Q_OBJECT

public:
    explicit ProductPicker(QWidget *parent, QSqlDatabase db);
    ~ProductPicker();

    bool is_valid();
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

    void refresh();
    void config();
    void prompt_error(QString text, bool exit_flag = false);
    void fill_table_with_query(QTableWidget *tab, QString query);
    void show_whole_table();
};

#endif // PRODUCTPICKER_H
