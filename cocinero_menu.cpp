#include "cocinero_menu.h"
#include "ui_cocinero_menu.h"

cocinero_menu::cocinero_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cocinero_menu)
{
    ui->setupUi(this);
}

cocinero_menu::~cocinero_menu()
{
    delete ui;
}


