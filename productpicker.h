#ifndef PRODUCTPICKER_H
#define PRODUCTPICKER_H

#include <QDialog>

namespace Ui {
class ProductPicker;
}

class ProductPicker : public QDialog {
    Q_OBJECT

public:
    explicit ProductPicker(QWidget *parent = 0);
    ~ProductPicker();

private:
    Ui::ProductPicker *ui;
};

#endif // PRODUCTPICKER_H
