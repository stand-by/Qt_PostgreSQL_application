#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QDialog>

namespace Ui {
class addproduct;
}

class addproduct : public QDialog
{
    Q_OBJECT

public:
    explicit addproduct(QWidget *parent = 0);
    ~addproduct();

private:
    Ui::addproduct *ui;
};

#endif // ADDPRODUCT_H
