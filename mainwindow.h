#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>
#include <QTableWidget>
#include "loginwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString login;
    QString password;
    QString ip;
    QString port;

    void configure_tabs();
    void configure_tab_widget();
    void configure_tables();
    void center_and_resize_window(int w, int h);

    void prompt_error(QString text, bool exit_flag = false);
    void fill_table_with_query(QTableWidget *tab, QString query);

    void get_user_credentials();
    void create_db_connection();
};

#endif // MAINWINDOW_H
