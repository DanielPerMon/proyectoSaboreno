#include "mesero_menu.h"
#include "ui_mesero_menu.h"

mesero_menu::mesero_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mesero_menu)
{
    ui->setupUi(this);
}

mesero_menu::~mesero_menu()
{
    delete ui;
}

void mesero_menu::on_salir_clicked()
{

}

void mesero_menu::on_verMesas_pushButton_clicked()
{
    ui->Mesero_menu->setCurrentIndex(0);
}
