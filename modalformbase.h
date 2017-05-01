#ifndef MODALFORMBASE_H
#define MODALFORMBASE_H

#include <QtSql>
#include <QMessageBox>
#include <QApplication>

class ModalFormBase {
public:
    ModalFormBase(QSqlDatabase db_);

    bool is_valid();
    void prompt_error(QString text, bool exit_flag = false);

protected:
    QSqlDatabase db;
    bool is_filled;

};

#endif // MODALFORMBASE_H
