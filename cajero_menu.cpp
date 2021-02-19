#include "cajero_menu.h"
#include "ui_cajero_menu.h"

cajero_menu::cajero_menu(QSqlDatabase base, QString usuario, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cajero_menu)
{
    ui->setupUi(this);
    baseCajero = base;
    setWindowTitle("Saboreño Cajero");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

cajero_menu::~cajero_menu()
{
    delete ui;
}

void cajero_menu::on_saliC_pushButton_clicked()
{
    this->close();
    delete ui;
}

void cajero_menu::on_historial_pushButton_clicked()
{
    ui->menuCajero->setCurrentIndex(0);
}
