#include "moveorder.h"
#include "ui_moveorder.h"

MoveOrder::MoveOrder(QWidget *parent, QSqlDatabase db_): QDialog(parent), ModalFormBase(db_), ui(new Ui::MoveOrder) {
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle("Order");
    this->setFixedSize(this->size());

    ui->dateEdit_order_date->setDate(QDate::currentDate());
    ui->dateEdit_order_date->setEnabled(false);

    config_table();
}

MoveOrder::~MoveOrder() {
    delete ui;
}

void MoveOrder::on_buttonBox_accepted() {
    if(ui->table_goods->rowCount()==0) {
        prompt_error("You should choose at least one product to fill order!");
        return;
    }

    QString type_id = "-1";
    //hardcoded ids for appropriate types
    if(ui->comboBox_types->currentText()=="Transportation from storage to shop") type_id = "3";
    else if(ui->comboBox_types->currentText()=="Transportation from shop to storage") type_id = "4";
    QString date = ui->dateEdit_order_date->text();
    QString id_goods_list = "";
    QString id_amount_list = "";

    for(int i = 0; i < ui->table_goods->rowCount(); i++) {
        id_goods_list += ui->table_goods->item(i, 0)->text();
        id_amount_list += QString::number(static_cast<QSpinBox*>(ui->table_goods->cellWidget(i,3))->value());

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
        prompt_error("An error occurred! Your order canceled!\n"+db.lastError().text().split("(P0001)").first());
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

void MoveOrder::config_table() {
    ui->table_goods->setSelectionMode(QAbstractItemView::NoSelection);
    for (int i = 0; i < ui->table_goods->horizontalHeader()->count(); ++i)
        ui->table_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
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

    QSpinBox* spin = new QSpinBox(ui->table_goods);
    spin->setValue(1);
    spin->setMinimum(1);
    spin->setMaximum(1000);
    ui->table_goods->setCellWidget(ui->table_goods->rowCount()-1,3,spin);
}
