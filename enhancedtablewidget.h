#ifndef ENHANCEDTABLEWIDGET_H
#define ENHANCEDTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QtDebug>
#include <QtSql>

//class which wraps QTableWidget and extends it with necessary features
class EnhancedTableWidget : public QTableWidget {
    Q_OBJECT

private:
    //properties for sorting
    bool asc_sort_order;
    int previous_index;
    QString asc_color;
    QString desc_color;

    //properties for filtering
    bool is_filtering;
    int filter_column;

    void make_bold_column(int col);
    void make_notbold_column(int col);

public:
    EnhancedTableWidget(QWidget* obj);
    virtual ~EnhancedTableWidget() {}

    void config_default_behavior();
    void render_cell_tooltip();
    void initialize_sorting();

    void fill_column_with_color(int index, QString color);
    bool fill_table_with_query(QSqlDatabase db, QString query);

    void show_whole_table();

private slots:
    void handle_header_click(int index);
    void handle_cell_doubleclick(int row, int column);

};

#endif // ENHANCEDTABLEWIDGET_H
