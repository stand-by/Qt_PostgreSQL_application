#include "moveorder.h"
#include "ui_moveorder.h"

MoveOrder::MoveOrder(QWidget *parent, QSqlDatabase db_): QDialog(parent), ui(new Ui::MoveOrder) {
    ui->setupUi(this);

    db = db_;
    is_filled = false;

    this->setModal(true);
    this->setWindowTitle("Оформити перемiщення");
    this->setFixedSize(this->size());

    ui->dateEdit_order_date->setDate(QDate::currentDate());
    ui->dateEdit_order_date->setEnabled(false);

    config();
}

MoveOrder::~MoveOrder() {
    delete ui;
}

bool MoveOrder::is_valid() {
    return is_filled;
}

void MoveOrder::on_buttonBox_accepted() {
    if(ui->table_goods->rowCount()==0) {
        prompt_error("Вам потрiбно обрати хоча б один товар, для того щоб оформити замовлення!");
        return;
    }

    QString type_id = "-1";
    if(ui->comboBox_types->currentText()=="Переміщення з складу в магазин") type_id = "3";
    else if(ui->comboBox_types->currentText()=="Переміщення з магазину в склад") type_id = "4";
    QString date = ui->dateEdit_order_date->text();
    QString id_goods_list = "";
    QString id_amount_list = "";

    for(int i = 0; i < ui->table_goods->rowCount(); i++) {
        id_goods_list += ui->table_goods->item(i, 0)->text();
        id_amount_list += ui->table_goods->item(i, 3)->text();

        if(i != ui->table_goods->rowCount()-1) {
            id_goods_list += ",";
            id_amount_list += ",";
        }
    }

    QString s = QString("SELECT add_move_order('{%1}'::INT[],%2,'{%3}'::INT[],'%4');")
                        .arg(id_goods_list,type_id,id_amount_list,date);
    QSqlQuery sq = db.exec(s);

    qDebug() << s;
    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Виникла помилка! Оформлення замовлення не вiдбулося!\n"+db.lastError().text().split("(P0001)").first());
    } else {
        is_filled = true;
    }
}

void MoveOrder::on_button_addproduct_clicked() {
    ProductPicker* form = new ProductPicker(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};

    if(form->is_valid())
        append_to_goods_table(form->pick_product_id(), form->pick_product_name(), form->pick_product_type());

    delete form;
}

void MoveOrder::config() {
    spin_delegate = new SpinBoxDelegate(this);
    ui->table_goods->setItemDelegateForColumn(3,spin_delegate);

    ui->table_goods->setSelectionMode(QAbstractItemView::NoSelection);
    for (int i = 0; i < ui->table_goods->horizontalHeader()->count(); ++i)
        ui->table_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
}

void MoveOrder::prompt_error(QString text, bool exit_flag) {
    QMessageBox messageBox;
    messageBox.critical(0,"Помилка",text);
    messageBox.setFixedSize(500,200);
    if(exit_flag) {
        QApplication::closeAllWindows();
        exit(0);
    }
}

void MoveOrder::append_to_goods_table(int id, QString name, QString type) {
    QTableWidgetItem* item;
    ui->table_goods->insertRow(ui->table_goods->rowCount());

    item = new QTableWidgetItem(QString::number(id),QTableWidgetItem::Type);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->table_goods->setItem(ui->table_goods->rowCount()-1,0,item);

    item = new QTableWidgetItem(name,QTableWidgetItem::Type);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->table_goods->setItem(ui->table_goods->rowCount()-1,1,item);

    item = new QTableWidgetItem(type,QTableWidgetItem::Type);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->table_goods->setItem(ui->table_goods->rowCount()-1,2,item);

    item = new QTableWidgetItem("1",QTableWidgetItem::Type);
    ui->table_goods->setItem(ui->table_goods->rowCount()-1,3,item);
}
