#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    //qt basic config
    ui->setupUi(this);

    //db connection config
    this->get_user_credentials();
    this->create_db_connection();

    //window config
    this->center_and_resize_window(1200,600);
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

    //append username to label_you_entered_as
    ui->label_you_entered_as->setText(ui->label_you_entered_as->text()+this->login);

    //disable insert actions when logged in as inspector
    if(login == "inspector") this->change_observer_mode(false);
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

void MainWindow::change_observer_mode(bool state) {
    ui->button_add_goods_list->setEnabled(state);
    ui->button_add_purchase_goods->setEnabled(state);
    ui->button_contractor_purchase_goods->setEnabled(state);
    ui->button_add_sell_goods->setEnabled(state);
    ui->button_contractor_sell_goods->setEnabled(state);
    ui->button_add_move_goods->setEnabled(state);
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
        QApplication::closeAllWindows();
        exit(0);
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
    //ban cell editing
    ui->table_goods_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_purchase_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_sell_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_move_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //ban cell selection only for goods table
    ui->table_goods_list->setSelectionMode(QAbstractItemView::NoSelection);
    //allow only single row selection
    ui->table_purchase_goods->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_sell_goods->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_move_goods->setSelectionMode(QAbstractItemView::SingleSelection);

    //set row selection for purchase, sell and move tables
    ui->table_purchase_goods->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_sell_goods->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_move_goods->setSelectionBehavior(QAbstractItemView::SelectRows);

    //stretch columns to fill whole window
    for (int i = 0; i < ui->table_goods_list->horizontalHeader()->count(); ++i)
        ui->table_goods_list->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_purchase_goods->horizontalHeader()->count(); ++i)
        ui->table_purchase_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_sell_goods->horizontalHeader()->count(); ++i)
        ui->table_sell_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_move_goods->horizontalHeader()->count(); ++i)
        ui->table_move_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    //similar configurations for parametric tables
    ui->table_datails_purchase_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_details_sell_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_details_move_goods->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->table_datails_purchase_goods->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_details_sell_goods->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_details_move_goods->setSelectionMode(QAbstractItemView::NoSelection);

    for (int i = 0; i < ui->table_datails_purchase_goods->horizontalHeader()->count(); ++i)
        ui->table_datails_purchase_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_details_sell_goods->horizontalHeader()->count(); ++i)
        ui->table_details_sell_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->table_details_move_goods->horizontalHeader()->count(); ++i)
        ui->table_details_move_goods->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    /*
    //remove indexing column
    ui->table_goods_list->verticalHeader()->setVisible(false);
    ui->table_purchase_goods->verticalHeader()->setVisible(false);
    ui->table_sell_goods->verticalHeader()->setVisible(false);
    ui->table_move_goods->verticalHeader()->setVisible(false);
    */

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
    ui->table_goods_list->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_goods_list);
}

void MainWindow::refresh_table_purchase_goods() {
    QString query = "SELECT * FROM short_purchase_goods;";
    this->fill_table_with_query(ui->table_purchase_goods, query);
    ui->table_purchase_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_purchase_goods);

    ui->table_purchase_goods->clearSelection();
    ui->table_datails_purchase_goods->clearContents();
    ui->table_datails_purchase_goods->setRowCount(0);
}

void MainWindow::refresh_table_sell_goods() {
    QString query = "SELECT * FROM short_sell_goods;";
    this->fill_table_with_query(ui->table_sell_goods, query);
    ui->table_sell_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_sell_goods);

    ui->table_sell_goods->clearSelection();
    ui->table_details_sell_goods->clearContents();
    ui->table_details_sell_goods->setRowCount(0);
}

void MainWindow::refresh_table_move_goods() {
    QString query = "SELECT * FROM short_move_goods;";
    this->fill_table_with_query(ui->table_move_goods, query);
    ui->table_move_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_move_goods);

    ui->table_move_goods->clearSelection();
    ui->table_details_move_goods->clearContents();
    ui->table_details_move_goods->setRowCount(0);
}

void MainWindow::refresh_table_details_purchase_goods(int id_doc) {
    QString query = QString("SELECT * FROM goods_details_on_document(%1);").arg(id_doc);
    this->fill_table_with_query(ui->table_datails_purchase_goods, query);
    ui->table_datails_purchase_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_datails_purchase_goods);
}

void MainWindow::refresh_table_details_sell_goods(int id_doc) {
    QString query = QString("SELECT * FROM goods_details_on_document(%1);").arg(id_doc);
    this->fill_table_with_query(ui->table_details_sell_goods, query);
    ui->table_details_sell_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_details_sell_goods);
}

void MainWindow::refresh_table_details_move_goods(int id_doc) {
    QString query = QString("SELECT * FROM goods_details_on_document(%1);").arg(id_doc);
    this->fill_table_with_query(ui->table_details_move_goods, query);
    ui->table_details_move_goods->refresh();
    tools::enhance_table_with_cell_tooltip(ui->table_details_move_goods);
}

void MainWindow::on_button_refresh_goods_list_clicked() {
    this->refresh_table_goods_list();
    this->configure_tables();
}

void MainWindow::on_button_refresh_purchase_goods_clicked() {
    this->refresh_table_purchase_goods();
    this->configure_tables();
}

void MainWindow::on_button_refresh_sell_goods_clicked() {
    this->refresh_table_sell_goods();
    this->configure_tables();
}

void MainWindow::on_button_refresh_move_goods_clicked() {
    this->refresh_table_move_goods();
    this->configure_tables();
}

void MainWindow::on_button_add_goods_list_clicked() {
    addproduct* form = new addproduct(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_goods_list();
    this->configure_tables();
}

void MainWindow::on_button_add_purchase_goods_clicked() {
    OrderWindow* form = new OrderWindow(this, db, true);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_purchase_goods();
    this->configure_tables();
}

void MainWindow::on_button_add_sell_goods_clicked() {
    OrderWindow* form = new OrderWindow(this, db, false);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_sell_goods();
    this->configure_tables();
}

void MainWindow::on_button_add_move_goods_clicked() {
    MoveOrder* form = new MoveOrder(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_move_goods();
    this->configure_tables();
}

void MainWindow::on_button_contractor_sell_goods_clicked() {
    ContractorWindow* form = new ContractorWindow(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_sell_goods();
    this->configure_tables();
}

void MainWindow::on_button_contractor_purchase_goods_clicked() {
    ContractorWindow* form = new ContractorWindow(this, db);
    while(form->exec()!=QDialog::Rejected && !form->is_valid()) {};
    delete form;

    this->refresh_table_purchase_goods();
    this->configure_tables();
}

void MainWindow::on_table_purchase_goods_itemSelectionChanged() {
    int row_number = ui->table_purchase_goods->selectionModel()->currentIndex().row();
    QModelIndex index = ui->table_purchase_goods->model()->index(row_number,0,QModelIndex());
    int id_doc = ui->table_purchase_goods->model()->data(index).toInt();

    this->refresh_table_details_purchase_goods(id_doc);
    this->configure_tables();
}

void MainWindow::on_table_sell_goods_itemSelectionChanged() {
    int row_number = ui->table_sell_goods->selectionModel()->currentIndex().row();
    QModelIndex index = ui->table_sell_goods->model()->index(row_number,0,QModelIndex());
    int id_doc = ui->table_sell_goods->model()->data(index).toInt();

    this->refresh_table_details_sell_goods(id_doc);
    this->configure_tables();
}

void MainWindow::on_table_move_goods_itemSelectionChanged() {
    int row_number = ui->table_move_goods->selectionModel()->currentIndex().row();
    QModelIndex index = ui->table_move_goods->model()->index(row_number,0,QModelIndex());
    int id_doc = ui->table_move_goods->model()->data(index).toInt();

    this->refresh_table_details_move_goods(id_doc);
    this->configure_tables();
}
