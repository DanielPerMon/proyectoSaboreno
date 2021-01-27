#include "gerente_menu.h"
#include "ui_gerente_menu.h"

gerente_menu::gerente_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gerente_menu)
{
    ui->setupUi(this);
}

gerente_menu::~gerente_menu()
{
    delete ui;
}

void gerente_menu::on_nuevoUsuario_pushButton_clicked()
{
    ui->Menu_gerente->setCurrentIndex(0);
}

void gerente_menu::on_usuarios_pushButton_clicked()
{
    ui->Menu_gerente->setCurrentIndex(1);
}
