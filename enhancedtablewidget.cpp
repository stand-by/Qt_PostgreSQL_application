#include "enhancedtablewidget.h"

EnhancedTableWidget::EnhancedTableWidget(QWidget* obj): QTableWidget(obj) {
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(handle_header_click(int)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handle_cell_doubleclick(int,int)));

    asc_color = "#e3f5ab";
    desc_color = "#febbbb";

    initialize_sorting();
}

void EnhancedTableWidget::config_default_behavior() {
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    for (int i = 0; i < this->horizontalHeader()->count(); ++i)
        this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
}

void EnhancedTableWidget::initialize_sorting() {
    previous_index = 0;
    asc_sort_order = true;
    fill_column_with_color(0,asc_color);

    is_filtering = false;
    filter_column = -1;
    for(int i = 0; i < this->rowCount(); i++)
        this->showRow(this->item(i, 0)->row());
}

void EnhancedTableWidget::render_cell_tooltip() {
    for (int i=0; i < this->rowCount(); i++)
        for (int j=0; j < this->columnCount(); j++) {
            QTableWidgetItem *item =  this->item(i,j);
            item->setToolTip(item->text());
        }
}

//HANDLE RETURN VALUE
bool EnhancedTableWidget::fill_table_with_query(QSqlDatabase db, QString query) {
    QSqlQuery sql_query = db.exec(query);

    if(sql_query.lastError().isValid()) {
        qDebug() << sql_query.lastError();
        return false;
        //prompt_error("Виникла помилка при завантаженнi данних до таблицi!");
    }

    else {
        int n = this->columnCount();
        this->setRowCount(sql_query.size());
        sql_query.first();

        int row = 0;
        do {
            for (int i = 0; i < n; i++){
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(sql_query.value(i).toString());
                this->setItem(row, i, item);
            }
            row++;
        } while (sql_query.next());

        this->resizeColumnsToContents();

        this->initialize_sorting();
        this->render_cell_tooltip();
        return true;
    }
}

void EnhancedTableWidget::fill_column_with_color(int index, QString color) {
    for(int i = 0; i < this->rowCount(); i++)
        this->item(i, index)->setBackground(QBrush(QColor(color)));
}

void EnhancedTableWidget::show_whole_table() {
    for(int i = 0; i < this->rowCount(); i++)
        this->showRow(this->item(i, 0)->row());
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
