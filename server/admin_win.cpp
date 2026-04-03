#include "admin_win.h"
#include "ui_admin_win.h"

admin_win::admin_win(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::admin_win)
{
    ui->setupUi(this);
}

admin_win::~admin_win()
{
    delete ui;
}

