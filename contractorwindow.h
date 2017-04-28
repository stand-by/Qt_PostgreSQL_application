#ifndef CONTRACTORWINDOW_H
#define CONTRACTORWINDOW_H

#include <QDialog>

namespace Ui {
class ContractorWindow;
}

class ContractorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ContractorWindow(QWidget *parent = 0);
    ~ContractorWindow();

private:
    Ui::ContractorWindow *ui;
};

#endif // CONTRACTORWINDOW_H
