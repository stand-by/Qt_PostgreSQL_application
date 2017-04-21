#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->center_and_resize_window(900,600);
    this->setWindowTitle("Магазин Автозапчастин");

    this->configure_tab_widget();
    this->configure_tabs();
    this->configure_tables();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::center_and_resize_window(int w, int h) {
    this->resize(w,h);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
}

void MainWindow::configure_tab_widget() {
    QString config = "#tab_widget {border:0px solid black;}";

    ui->tab_widget->setStyleSheet(config);
}

void MainWindow::configure_tabs() {
    QString config = ".QWidget {background-color:rgb(238,238,238);}";

    ui->tab_goods_list->setStyleSheet(config);
    ui->tab_purchase_goods->setStyleSheet(config);
    ui->tab_sell_goods->setStyleSheet(config);
    ui->tab_move_goods->setStyleSheet(config);
}

void MainWindow::configure_tables() {
    ui->table_goods_list->setStyleSheet("background-color: white");
    ui->table_purchase_goods->setStyleSheet("background-color: white");
    ui->table_sell_goods->setStyleSheet("background-color: white");
    ui->table_move_goods->setStyleSheet("background-color: white");

    for (int i = 0; i < ui->table_goods_list->horizontalHeader()->count(); ++i)
        ui->table_goods_list->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_purchase_goods->horizontalHeader()->count(); ++i)
        ui->table_purchase_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_sell_goods->horizontalHeader()->count(); ++i)
        ui->table_sell_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_move_goods->horizontalHeader()->count(); ++i)
        ui->table_move_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
}
