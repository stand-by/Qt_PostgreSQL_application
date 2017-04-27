#include "enhancedtablewidget.h"

#include <QHeaderView>

EnhancedTableWidget::EnhancedTableWidget(QWidget* obj): QTableWidget(obj) {
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(handle_header_click(int)));

    asc_color = "#e3f5ab";
    desc_color = "#febbbb";

    refresh();
}

void EnhancedTableWidget::refresh() {
    previous_index = 0;
    asc_sort_order = true;

    fill_column_with_color(0,asc_color);
}

void EnhancedTableWidget::fill_column_with_color(int index, QString color) {
    for(int i = 0; i < this->rowCount(); i++)
        this->item(i, index)->setBackground(QBrush(QColor(color)));
}

void EnhancedTableWidget::handle_header_click(int index) {
    //qDebug() << "header clicked "+QString::number(index);

    if(previous_index == index) {
        if(!asc_sort_order) {
            this->sortByColumn(index, Qt::AscendingOrder);
            asc_sort_order = !asc_sort_order;

            fill_column_with_color(index, asc_color);
        } else {
            this->sortByColumn(index, Qt::DescendingOrder);
            asc_sort_order = !asc_sort_order;

            fill_column_with_color(index, desc_color);
        }

    } else {
        fill_column_with_color(previous_index, "white");

        asc_sort_order = true;
        this->sortByColumn(index, Qt::AscendingOrder);
        previous_index = index;

        fill_column_with_color(index, asc_color);
    }
}

