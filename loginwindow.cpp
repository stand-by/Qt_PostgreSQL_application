#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent): QDialog(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    this->setModal(true);
    this->setWindowTitle("Магазин Автозапчастин");
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_username->setText("manager");
    ui->lineEdit_password->setText("qwerty");
}

LoginWindow::~LoginWindow() {
    delete ui;
}

QString LoginWindow::get_login() {
    return ui->lineEdit_username->text();
}

QString LoginWindow::get_password() {
    return ui->lineEdit_password->text();
}
