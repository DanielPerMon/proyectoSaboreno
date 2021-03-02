#include "anfitrion_menu.h"
#include "ui_anfitrion_menu.h"

anfitrion_menu::anfitrion_menu(QSqlDatabase base,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::anfitrion_menu)
{
    ui->setupUi(this);
    baseAnfitrion = base;
    setWindowTitle("Saboreño Anfitrión");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    if(baseAnfitrion.isOpen())
        qDebug() << "está abierta la BD";
    else
        qDebug() << "error en la base";
}

anfitrion_menu::~anfitrion_menu()
{
    delete ui;
}

void anfitrion_menu::on_btnSalirH_clicked()
{
    this->close();
    delete ui;
}

void anfitrion_menu::on_btnReservacion_clicked()
{
    ui->menu_host->setCurrentIndex(0);
}

void anfitrion_menu::on_btnReservaciones_clicked()
{
    ui->menu_host->setCurrentIndex(1);
}
