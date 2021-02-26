#include "cancelar.h"
#include "ui_cancelar.h"

cancelar::cancelar(QWidget *parent, int cuenta, int table) :
    QDialog(parent),
    ui(new Ui::cancelar)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    cta = cuenta;
    mesa = table;
    cargarOrden();
}

cancelar::~cancelar()
{
    delete ui;
}

void cancelar::on_btnAceptar_clicked()
{

}

void cancelar::on_btnCancelar_clicked()
{

}

void cancelar::cargarOrden()
{
    QSqlQuery vercuenta;
    vercuenta.exec("SELECT p.NombrePlatillo, o.cantidadPlatillo, o.id_orden,o.precio FROM platillos as p INNER JOIN orden as o ON p.idPlatillos = o.id_platillo "
                   " INNER JOIN cuenta as c ON o.id_cuenta = c.id_cuenta INNER JOIN mesa as m ON c.idmesa = m.idMesa WHERE c.estado = 'finalizada' "
                   "AND m.idMesa = '"+QString::number(mesa)+"'");
    ui->tablaItems->setRowCount(0);
    int fila;
    while(vercuenta.next()){
        qDebug() << "entra " << vercuenta.value(0).toString();
        fila = ui->tablaItems->rowCount();
        ui->tablaItems->insertRow(fila);
        ui->tablaItems->setItem(fila,0,new QTableWidgetItem(vercuenta.value(2).toString()));
        ui->tablaItems->setItem(fila,1,new QTableWidgetItem(vercuenta.value(0).toString()));
        ui->tablaItems->setItem(fila,2,new QTableWidgetItem(vercuenta.value(1).toString()));
        ui->tablaItems->setItem(fila,3,new QTableWidgetItem("$"+vercuenta.value(3).toString()+".00"));
        double sub = vercuenta.value(3).toDouble() * vercuenta.value(1).toDouble();
        ui->tablaItems->setItem(fila,4,new QTableWidgetItem("     $"+QString::number(sub)+".00"));
    }

    qDebug() << vercuenta.lastError();
}
