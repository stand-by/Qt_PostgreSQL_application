#include "modalformbase.h"

ModalFormBase::ModalFormBase(QSqlDatabase db_) {
    db = db_;
    is_filled = false;
}

void ModalFormBase::prompt_error(QString text, bool exit_flag) {
    QMessageBox messageBox;
    messageBox.critical(0,"Помилка",text);
    messageBox.setFixedSize(500,200);
    if(exit_flag) {
        QApplication::closeAllWindows();
        exit(0);
    }
}

bool ModalFormBase::is_valid() { return is_filled; }
