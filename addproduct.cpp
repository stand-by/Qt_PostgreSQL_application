#include "addproduct.h"
#include "ui_addproduct.h"

addproduct::addproduct(QWidget *parent, QSqlDatabase db_): QDialog(parent), ModalFormBase(db_), ui(new Ui::addproduct), insert_new_type(false) {
    ui->setupUi(this);

    //hide parts that necessary for adding new type
    ui->input_new_type->hide();
    ui->label_new_type->hide();

    //initial window config
    this->setModal(true);
    this->setWindowTitle("Додати новий товар");
    this->setFixedSize(this->size());

    //query two columns of one table into two different models
    model_names = new QSqlQueryModel(this);
    model_names->setQuery("SELECT name_of_type FROM type_of_goods ORDER BY id_type_of_goods;", db);
    model_ids = new QSqlQueryModel(this);
    model_ids->setQuery("SELECT id_type_of_goods FROM type_of_goods ORDER BY id_type_of_goods;", db);

    //set some initial value to combobox
    ui->box_good_type->setModel(model_names);
    ui->box_good_type->setCurrentIndex(0);
    ui->box_good_type->view()->setCurrentIndex(model_names->index(0,0));
}

addproduct::~addproduct() {
    delete model_names;
    delete model_ids;
    delete ui;
}

//hide or show additional fields
void addproduct::on_button_add_type_clicked() {
    switch_modes();
}

//change UI to give ability to add new goods type
void addproduct::switch_modes() {
    insert_new_type = !insert_new_type;
    if(insert_new_type) {
        ui->input_new_type->show();
        ui->label_new_type->show();
        ui->box_good_type->setEnabled(false);
        ui->button_add_type->setText("-");
    } else {
        ui->input_new_type->hide();
        ui->label_new_type->hide();
        ui->box_good_type->setEnabled(true);
        ui->button_add_type->setText("+");
    }
}

//add new goods type to db and update type_id which will be used later in product insertion
void addproduct::perform_type_insert() {
    QString query = QString("SELECT add_type_of_goods('%1')").arg(ui->input_new_type->text());
    QSqlQuery qs = db.exec(query);

    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Додавання нового типу не вiдбулося");
        return;
    }

    qs.first();
    type_id = qs.value(0).toInt();
}

//add new product to goods table with appropriate type_id
void addproduct::perform_product_insert() {
    qDebug() << "type_id =" << type_id;

    QString query = QString("SELECT add_goods('%1','%2'::int)").arg(ui->input_good_name->text(),QString::number(type_id));
    db.exec(query);
    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Додавання нового товару не вiдбулося");
    }
}

void addproduct::on_button_box_accepted() {
    if(insert_new_type && ui->input_new_type->text()=="") {
        prompt_error("Ви залишили пустi поля. Заповнiть форму знову!");
        return;
    }
    if(ui->input_good_name->text()=="") {
        prompt_error("Ви залишили пустi поля. Заповнiть форму знову!");
        return;
    }

    type_id = model_ids->data(ui->box_good_type->view()->currentIndex()).toInt();
    if(insert_new_type) perform_type_insert();
    perform_product_insert();

    is_filled = true;
}
