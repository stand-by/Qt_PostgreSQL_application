#include "orderwindow.h"
#include "ui_orderwindow.h"

OrderWindow::OrderWindow(QWidget *parent, QSqlDatabase db_, bool flag): QDialog(parent), ui(new Ui::OrderWindow) {
    ui->setupUi(this);

    db = db_;
    is_filled = false;
    is_purchase_mode = flag;

    model_names = new QSqlQueryModel(this);
    model_ids = new QSqlQueryModel(this);

    this->setModal(true);
    this->setWindowTitle("Оформити замовлення");
    this->setFixedSize(this->size());

    ui->dateEdit_order_date->setDate(QDate::currentDate());
    ui->dateEdit_order_date->setEnabled(false);

    config();
    fill_contractors_combobox();
}

OrderWindow::~OrderWindow() {
    delete model_names;
    delete model_ids;
    delete ui;
}

void OrderWindow::on_buttonBox_accepted() {
    if(ui->table_goods->rowCount()==0) {
        prompt_error("Вам потрiбно обрати хоча б один товар, для того щоб оформити замовлення!");
        return;
    }

    QString contractor_id = model_ids->data(ui->comboBox_contractors->view()->currentIndex()).toString();
    QString date = ui->dateEdit_order_date->text();
    QString id_goods_list = "";
    QString id_amount_list = "";
    QString id_price_list = "";

    for(int i = 0; i < ui->table_goods->rowCount(); i++) {
        id_goods_list += ui->table_goods->item(i, 0)->text();
        id_amount_list += QString::number(static_cast<QSpinBox*>(ui->table_goods->cellWidget(i,3))->value());
        id_price_list += QString::number(static_cast<QSpinBox*>(ui->table_goods->cellWidget(i,4))->value());

        if(i != ui->table_goods->rowCount()-1) {
            id_goods_list += ",";
            id_amount_list += ",";
            id_price_list += ",";
        }
    }

    if(is_purchase_mode) {
        QString s = QString("SELECT add_purchase_order(%1,'{%2}'::INT[],'{%3}'::INT[],'{%4}'::numeric(10,2)[],'%5');")
                            .arg(contractor_id,id_goods_list,id_amount_list,id_price_list,date);
        QSqlQuery sq = db.exec(s);

        qDebug() << s;
        if(db.lastError().isValid()) {
            qDebug() << db.lastError();
            prompt_error("Виникла помилка! Оформлення замовлення не вiдбулося!\n"+db.lastError().text().split("(P0001)").first());
        } else {
            is_filled = true;
        }


    } else {
        QString s = QString("SELECT add_sell_order(%1,'{%2}'::INT[],'{%3}'::INT[],'{%4}'::numeric(10,2)[],'%5');")
                            .arg(contractor_id,id_goods_list,id_amount_list,id_price_list,date);
        QSqlQuery sq = db.exec(s);

        qDebug() << s;
        if(db.lastError().isValid()) {
            qDebug() << db.lastError();
            prompt_error("Виникла помилка! Оформлення замовлення не вiдбулося!\n"+db.lastError().text().split("(P0001)").first());
        } else {
            is_filled = true;
        }
    }
}

void OrderWindow::fill_contractors_combobox() {
    if(is_purchase_mode) {
        model_names->setQuery("SELECT name FROM contractors WHERE table_type='juridical';", db);
        model_ids->setQuery("SELECT id_contractor FROM contractors WHERE table_type='juridical';", db);
    } else {
        model_names->setQuery("SELECT name FROM contractors;", db);
        model_ids->setQuery("SELECT id_contractor FROM contractors;", db);
    }

    ui->comboBox_contractors->setModel(model_names);
    ui->comboBox_contractors->setCurrentIndex(0);
    ui->comboBox_contractors->view()->setCurrentIndex(model_names->index(0,0));
}

void OrderWindow::append_to_goods_table(int id, QString name, QString type) {
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

    int min_price = ceil(get_minimal_price(id));

    spin = new QSpinBox(ui->table_goods);
    spin->setValue(min_price);
    spin->setMinimum(min_price);
    spin->setMaximum(1000000);
    ui->table_goods->setCellWidget(ui->table_goods->rowCount()-1,4,spin);
}

double OrderWindow::get_minimal_price(int id_good) {
    QString s = QString("SELECT calculate_minimal_price_for_product(%1);").arg(id_good);
    QSqlQuery sq = db.exec(s);

    qDebug() << s;
    sq.first();
    double val = sq.value(0).toDouble();

    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Виникла помилка! Неможливо пiдрахувати мiнiмальну цiну для цього товару!",true);
    }

    return val;
}

bool OrderWindow::is_valid() {
    return is_filled;
}

void OrderWindow::config() {
    ui->table_goods->setSelectionMode(QAbstractItemView::NoSelection);
    for (int i = 0; i < ui->table_goods->horizontalHeader()->count(); ++i)
        ui->table_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
}

void OrderWindow::on_button_addcontractor_clicked() {
    ContractorWindow* form = new ContractorWindow(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    fill_contractors_combobox();
}

void OrderWindow::on_button_addproduct_clicked() {
    ProductPicker* form = new ProductPicker(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};

    if(form->is_valid())
        append_to_goods_table(form->pick_product_id(), form->pick_product_name(), form->pick_product_type());

    delete form;
}

void OrderWindow::prompt_error(QString text, bool exit_flag) {
    QMessageBox messageBox;
    messageBox.critical(0,"Помилка",text);
    messageBox.setFixedSize(500,200);
    if(exit_flag) {
        QApplication::closeAllWindows();
        exit(0);
    }
}
