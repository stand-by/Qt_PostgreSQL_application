#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    //qt basic config
    ui->setupUi(this);

    //db connection config
    this->get_user_credentials();
    this->create_db_connection();

    //window config
    this->center_and_resize_window(1000,600);
    this->setWindowTitle("Магазин Автозапчастин");

    //ui config
    this->configure_tab_widget();
    this->configure_tabs();
    this->configure_tables();

    //initial tables filling
    this->refresh_table_goods_list();
    this->refresh_table_purchase_goods();
    this->refresh_table_sell_goods();
    this->refresh_table_move_goods();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fill_table_with_query(QTableWidget *tab, QString query){
    QSqlQuery sql_query = db.exec(query);

    //prompt error message, when sql query failed
    if(sql_query.lastError().isValid()) {
        qDebug() << sql_query.lastError();
        prompt_error("Виникла помилка при завантаженнi данних до таблицi!");
    }

    //otherwise fill table
    else {
        int n = tab->columnCount();
        tab->setRowCount(sql_query.size());
        sql_query.first();

        int row = 0;
        do {
            for (int i = 0; i < n; i++){
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(sql_query.value(i).toString());
                tab->setItem(row, i, item);
            }
            row++;
        } while (sql_query.next());

        tab->resizeColumnsToContents();
    }
}

void MainWindow::get_user_credentials() {
    LoginWindow login_form(this);
    login_form.show();

    if(login_form.exec() == QDialog::Accepted) {
        qDebug() << "Login Accepted";
        this->login = login_form.get_login();
        this->password = login_form.get_password();
    }

    else {
        qDebug() << "Login Rejected";
        QApplication::quit();
    }
}

void MainWindow::create_db_connection() {
    ip = "localhost";
    port = "5432";

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(ip);
    db.setPort(port.toInt());
    db.setDatabaseName("automotive_shop");
    db.setUserName(login);
    db.setPassword(password);

    if(db.open()){
        qDebug() << "connected to db";
    } else {
        qDebug() << "can not connected to db";
        qDebug() << db.lastError();
        prompt_error("Виникла помилка при пiдключеннi до бази данних!\nПеревiрте Ваш логiн/пароль, або статус бази данних.",true);
    }
}

void MainWindow::prompt_error(QString text, bool exit_flag) {
    QMessageBox messageBox;
    messageBox.critical(0,"Помилка",text);
    messageBox.setFixedSize(500,200);
    if(exit_flag) {
        QApplication::closeAllWindows();
        exit(0);
    }
}

void MainWindow::center_and_resize_window(int w, int h) {
    this->resize(w,h);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
}

void MainWindow::configure_tab_widget() {
    QString config = "#tab_widget {border:0px solid black;}";

    //remove border from tab_widget for the beauty sake
    ui->tab_widget->setStyleSheet(config);
}

void MainWindow::configure_tabs() {
    QString config = ".QWidget {background-color:rgb(238,238,238);}";

    //paint tabs in the same color as window
    ui->tab_goods_list->setStyleSheet(config);
    ui->tab_purchase_goods->setStyleSheet(config);
    ui->tab_sell_goods->setStyleSheet(config);
    ui->tab_move_goods->setStyleSheet(config);
}

void MainWindow::configure_tables() {
    //remove indexing column
    ui->table_goods_list->verticalHeader()->setVisible(false);
    ui->table_purchase_goods->verticalHeader()->setVisible(false);
    ui->table_sell_goods->verticalHeader()->setVisible(false);
    ui->table_move_goods->verticalHeader()->setVisible(false);

    //ban cell editing
    ui->table_goods_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_purchase_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_sell_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_move_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //ban cell selection
    ui->table_goods_list->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_purchase_goods->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_sell_goods->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_move_goods->setSelectionMode(QAbstractItemView::NoSelection);

    //stretch columns to fill whole window
    for (int i = 0; i < ui->table_goods_list->horizontalHeader()->count(); ++i)
        ui->table_goods_list->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_purchase_goods->horizontalHeader()->count(); ++i)
        ui->table_purchase_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_sell_goods->horizontalHeader()->count(); ++i)
        ui->table_sell_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_move_goods->horizontalHeader()->count(); ++i)
        ui->table_move_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    /*
    //unnecessary part
    ui->table_goods_list->setStyleSheet("background-color: white");
    ui->table_purchase_goods->setStyleSheet("background-color: white");
    ui->table_sell_goods->setStyleSheet("background-color: white");
    ui->table_move_goods->setStyleSheet("background-color: white");
    */
}

void MainWindow::refresh_table_goods_list() {
    QString query = "SELECT * FROM goods_list;";
    this->fill_table_with_query(ui->table_goods_list, query);
}

void MainWindow::refresh_table_purchase_goods() {
    QString query = "SELECT * FROM purchase_goods;";
    this->fill_table_with_query(ui->table_purchase_goods, query);
}

void MainWindow::refresh_table_sell_goods() {
    QString query = "SELECT * FROM sell_goods;";
    this->fill_table_with_query(ui->table_sell_goods, query);
}

void MainWindow::refresh_table_move_goods() {
    QString query = "SELECT * FROM move_goods;";
    this->fill_table_with_query(ui->table_move_goods, query);
}
