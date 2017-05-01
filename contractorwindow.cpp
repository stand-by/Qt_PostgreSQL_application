#include "contractorwindow.h"
#include "ui_contractorwindow.h"

ContractorWindow::ContractorWindow(QWidget *parent, QSqlDatabase db_): QDialog(parent), ModalFormBase(db_), ui(new Ui::ContractorWindow) {
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle("Add new contractor");
    this->setFixedSize(this->size());

    configure();
}

ContractorWindow::~ContractorWindow() {
    delete ui;
}

void ContractorWindow::configure() {
    ui->tab_widget->setStyleSheet("#tab_widget {border:0px solid black;}");
    ui->tab_juridical->setStyleSheet(".QWidget {background-color:rgb(238,238,238);}");
    ui->tab_physical->setStyleSheet(".QWidget {background-color:rgb(238,238,238);}");

    //QRegExp numbers_only("\\d+");/^[a-z ,.'-]+$/i
    ui->lineEdit_phone_jur->setValidator(new QIntValidator(this));
    ui->lineEdit_code_jur->setValidator(new QIntValidator(this));
    ui->lineEdit_phone_phy->setValidator(new QIntValidator(this));
    ui->lineEdit_code_phy->setValidator(new QIntValidator(this));

    QRegExp reg(QString("^[A-zА-я ,.'-]+$/i"));
    ui->lineEdit_name_phy->setValidator(new QRegExpValidator(reg,this));
    ui->lineEdit_surname_phy->setValidator(new QRegExpValidator(reg,this));
    ui->lineEdit_pathronymic_phy->setValidator(new QRegExpValidator(reg,this));
}

void ContractorWindow::perform_juridical_insert() {
    QString query = QString("SELECT add_juridical_person('%1','%2'::int,'%3'::int,'%4')").arg(ui->lineEdit_name_jur->text(),
                                                                                              ui->lineEdit_code_jur->text(),
                                                                                              ui->lineEdit_phone_jur->text(),
                                                                                              ui->lineEdit_address_jur->text());
    db.exec(query);
    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Can not add new contractor");
    } else {
        is_filled = true;
    }
}

void ContractorWindow::perform_physical_insert() {
    QString query = QString("SELECT add_physical_person('%1','%2','%3','%4','%5','%6'::int,'%7'::int)")
                                                                                            .arg(ui->lineEdit_surname_phy->text(),
                                                                                            ui->lineEdit_name_phy->text(),
                                                                                            ui->lineEdit_pathronymic_phy->text(),
                                                                                            ui->dateEdit_birth_phy->text(),
                                                                                            ui->lineEdit_address_phy->text(),
                                                                                            ui->lineEdit_code_phy->text(),
                                                                                            ui->lineEdit_phone_phy->text());
    db.exec(query);
    if(db.lastError().isValid()) {
        qDebug() << db.lastError();
        prompt_error("Can not add new contractor");
    } else {
        is_filled = true;
    }
}

void ContractorWindow::on_buttonBox_accepted() {
    //juridical case
    if(ui->tab_widget->currentIndex() == 0) {
        if(!validate_juridical()) {
            prompt_error("You have to fill out a form!");
            return;
        }
        perform_juridical_insert();

    //physical case
    } else if(ui->tab_widget->currentIndex() == 1) {
        if(!validate_physical()) {
            prompt_error("You have to fill out a form!");
            return;
        }
        perform_physical_insert();
    }
}

bool ContractorWindow::validate_juridical() {
    if(ui->lineEdit_name_jur->text().length()==0) return false;
    if(ui->lineEdit_code_jur->text().length()==0) return false;
    if(ui->lineEdit_phone_jur->text().length()==0) return false;
    if(ui->lineEdit_address_jur->text().length()==0) return false;
    return true;
}

bool ContractorWindow::validate_physical() {
    if(ui->lineEdit_name_phy->text().length()==0) return false;
    if(ui->lineEdit_surname_phy->text().length()==0) return false;
    if(ui->lineEdit_pathronymic_phy->text().length()==0) return false;
    if(ui->lineEdit_code_phy->text().length()==0) return false;
    if(ui->lineEdit_phone_phy->text().length()==0) return false;
    if(ui->lineEdit_address_phy->text().length()==0) return false;
    return true;
}
