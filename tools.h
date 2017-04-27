#ifndef TOOLS
#define TOOLS

#include <QTableWidget>

namespace tools {

    inline void enhance_table_with_cell_tooltip(QTableWidget* table) {
        for (int i=0; i < table->rowCount(); i++)
            for (int j=0; j < table->columnCount(); j++) {
                QTableWidgetItem *item =  table->item(i,j);
                item->setToolTip(item->text());
            }
    }

}

#endif // TOOLS

