#include "productpicker.h"
#include "ui_productpicker.h"

ProductPicker::ProductPicker(QWidget *parent, QSqlDatabase db_): QDialog(parent), ui(new Ui::ProductPicker) {
    ui->setupUi(this);
    disconnect(ui->table_goods, SIGNAL(cellDoubleClicked(int,int)), ui->table_goods, SLOT(handle_cell_doubleclick(int,int)));

    db = db_;
    is_filled = false;
    is_searching = false;
    product_id = -1;

    this->setModal(true);
    this->setWindowTitle("Перелiк товарiв");
    this->setFixedSize(this->size());

    if(!ui->table_goods->fill_table_with_query(db,"SELECT * FROM goods_list;"))
        prompt_error("Виникла помилка при завантаженнi данних до таблицi!");
    ui->table_goods->config_default_behavior();
}

ProductPicker::~ProductPicker() {
    delete ui;
}

bool ProductPicker::is_valid() { return is_filled; }
int ProductPicker::pick_product_id() { return product_id; }
QString ProductPicker::pick_product_name() { return product_name; }
QString ProductPicker::pick_product_type() { return product_type; }

void ProductPicker::enable_search() {
    is_searching = true;
    ui->table_goods->selectionModel()->reset();
    ui->lineEdit_search->setEnabled(false);
    ui->pushButton_search->setText("Скасувати");
}

void ProductPicker::disable_search() {
    is_searching = false;
    ui->lineEdit_search->setEnabled(true);
    ui->lineEdit_search->setText("");
    ui->pushButton_search->setText("Знайти");
    ui->table_goods->show_whole_table();
}

void ProductPicker::on_buttonBox_accepted(){
    //qDebug() << ui->table_goods->selectionModel()->currentIndex();
    int row = ui->table_goods->selectionModel()->currentIndex().row();
    int col = ui->table_goods->selectionModel()->currentIndex().column();

    if(row < 0 || col < 0) {
        prompt_error("Ви не обрали товар зi списку! Повторiть спробу ще раз!");
        return;
    }

    product_id = ui->table_goods->item(row, 0)->text().toInt();
    product_name = ui->table_goods->item(row, 1)->text();
    product_type = ui->table_goods->item(row, 2)->text();

    is_filled = true;
}

void ProductPicker::on_pushButton_search_clicked() {
    if(!is_searching && ui->lineEdit_search->text()!="") {
        QString filter = ui->lineEdit_search->text();
        QList<QTableWidgetItem *> items = ui->table_goods->findItems(filter, Qt::MatchContains);

        for(int i = 0; i < ui->table_goods->rowCount(); i++)
            ui->table_goods->hideRow(ui->table_goods->item(i, 0)->row());
        for(int i = 0; i < items.count(); i++)
            if(items.at(i)->column()==1) ui->table_goods->showRow(items.at(i)->row());

        enable_search();

    } else {
        disable_search();
    }
}

void ProductPicker::on_pushButton_addproduct_clicked() {
    addproduct* form = new addproduct(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    ui->table_goods->fill_table_with_query(db,"SELECT * FROM goods_list;");
    ui->table_goods->config_default_behavior();

    ui->table_goods->selectionModel()->reset();
    disable_search();
}

void ProductPicker::prompt_error(QString text, bool exit_flag) {
    QMessageBox messageBox;
    messageBox.critical(0,"Помилка",text);
    messageBox.setFixedSize(500,200);
    if(exit_flag) {
        QApplication::closeAllWindows();
        exit(0);
    }
}
