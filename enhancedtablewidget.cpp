#include "enhancedtablewidget.h"

#include <QHeaderView>

EnhancedTableWidget::EnhancedTableWidget(QWidget* obj): QTableWidget(obj) {
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(handle_header_click(int)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handle_cell_doubleclick(int,int)));

    asc_color = "#e3f5ab";
    desc_color = "#febbbb";

    refresh();
}

void EnhancedTableWidget::refresh() {
    previous_index = 0;
    asc_sort_order = true;
    fill_column_with_color(0,asc_color);

    is_filtering = false;
    filter_column = -1;
    for(int i = 0; i < this->rowCount(); i++)
        this->showRow(this->item(i, 0)->row());
}

void EnhancedTableWidget::fill_column_with_color(int index, QString color) {
    for(int i = 0; i < this->rowCount(); i++)
        this->item(i, index)->setBackground(QBrush(QColor(color)));
}

void EnhancedTableWidget::make_bold_column(int col) {
    QFont font;
    font.setBold(true);
    for(int i = 0; i < this->rowCount(); i++)
        this->item(i, col)->setFont(font);
}

void EnhancedTableWidget::make_notbold_column(int col) {
    QFont font;
    font.setBold(false);
    for(int i = 0; i < this->rowCount(); i++)
        this->item(i, col)->setFont(font);
}

void EnhancedTableWidget::handle_cell_doubleclick(int row, int column) {
    if(is_filtering) {
        make_notbold_column(filter_column);

        for(int i = 0; i < this->rowCount(); i++)
            this->showRow(this->item(i, filter_column)->row());

        is_filtering = false;
        filter_column = -1;
    } else if(!is_filtering && filter_column==-1) {
        make_bold_column(column);

        QString filter = this->item(row, column)->text();
        QList<QTableWidgetItem *> items = this->findItems(filter, Qt::MatchExactly);

        for(int i = 0; i < this->rowCount(); i++)
            this->hideRow(this->item(i, column)->row());
        for(int i = 0; i < items.count(); i++)
            if(items.at(i)->column()==column) this->showRow(items.at(i)->row());

        is_filtering = true;
        filter_column = column;
    }
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
