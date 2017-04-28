#ifndef CONTRACTORWINDOW_H
#define CONTRACTORWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class ContractorWindow;
}

class ContractorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ContractorWindow(QWidget *parent, QSqlDatabase db_);
    ~ContractorWindow();
    bool is_valid();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ContractorWindow *ui;
    QSqlDatabase db;
    bool is_filled;

    void configure();
    void prompt_error(QString text, bool exit_flag = false);

    bool validate_juridical();
    bool validate_physical();

    void perform_juridical_insert();
    void perform_physical_insert();
};

#endif // CONTRACTORWINDOW_H
