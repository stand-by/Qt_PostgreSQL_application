#ifndef CONTRACTORWINDOW_H
#define CONTRACTORWINDOW_H

#include <QDialog>
#include <QtSql>
#include "modalformbase.h"

namespace Ui {
class ContractorWindow;
}

//class for adding new contractor
class ContractorWindow : public QDialog, public ModalFormBase {
    Q_OBJECT

public:
    explicit ContractorWindow(QWidget *parent, QSqlDatabase db_);
    ~ContractorWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ContractorWindow *ui;

    void configure();

    bool validate_juridical();
    bool validate_physical();

    void perform_juridical_insert();
    void perform_physical_insert();
};

#endif // CONTRACTORWINDOW_H
