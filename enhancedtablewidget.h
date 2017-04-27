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

    int filter_column;
    bool is_filtering;

    void fill_column_with_color(int index, QString color);
    void make_bold_column(int col);
    void make_notbold_column(int col);

public:
    EnhancedTableWidget(QWidget* obj);
    virtual ~EnhancedTableWidget() {}
    void refresh();

private slots:
    void handle_header_click(int index);
    void handle_cell_doubleclick(int row, int column);
};

#endif // ENHANCEDTABLEWIDGET_H
