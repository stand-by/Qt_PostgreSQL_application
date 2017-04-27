#ifndef ENHANCEDTABLEWIDGET_H
#define ENHANCEDTABLEWIDGET_H

#include <QTableWidget>
#include <QtDebug>
#include <QBitArray>
class EnhancedTableWidget : public QTableWidget {
    Q_OBJECT

private:
    int previous_index;
    bool asc_sort_order;
    QString asc_color;
    QString desc_color;

public:
    EnhancedTableWidget(QWidget* obj);
    virtual ~EnhancedTableWidget() {}
    void refresh();
    void fill_column_with_color(int index, QString color);

private slots:
    void handle_header_click(int index);
};

#endif // ENHANCEDTABLEWIDGET_H
