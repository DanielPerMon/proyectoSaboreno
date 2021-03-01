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
    valor = 0;
    montoAux = 0;
}

cancelar::~cancelar()
{
    delete ui;
}

void cancelar::on_btnAceptar_clicked()
{
    QMessageBox confirmacion(this);
    confirmacion.addButton("Sí",QMessageBox::YesRole);
    confirmacion.addButton("No",QMessageBox::NoRole);
    confirmacion.setWindowTitle("Confirmación");
    confirmacion.setIcon(QMessageBox::Question);
    confirmacion.setText("Se eliminarán los productos seleccionados \n¿Desea continuar?");
    int res = confirmacion.exec();
    if(res == 0){
        autoriza *ventana = new autoriza(this);
        ventana->exec();
        if(ventana->valor() == 1){
            QSqlQuery actualiza;
            for(int i=0;i<idOrden.size();i++){
                actualiza.exec("UPDATE orden SET estado = '2' WHERE id_orden = '"+idOrden[i]+"';");
            }
            QSqlQuery monto;
            monto.exec("SELECT precio, cantidadPlatillo FROM orden WHERE id_cuenta = '"+QString::number(cta)+"' AND estado = '1';");

            while(monto.next()){
                montoAux += (monto.value(0).toInt() * monto.value(1).toInt());
            }

            actualiza.exec("UPDATE cuenta SET total = '"+QString::number(montoAux)+"' WHERE id_cuenta = '"+QString::number(cta)+"'");

            valor = 1;
            this->close();
        }
        else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Error"),
                                   tr("No está autorizado"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
            valor = 0;
        }
    }

}

void cancelar::on_btnCancelar_clicked()
{
    this->close();
}

void cancelar::borrarItem()
{
    QWidget* focused = ui->tablaItems->focusWidget();
    int row = focused->property("row").toInt();

    QTableWidgetItem *valor = ui->tablaItems->item(row,0);
    idOrden.append(valor->text());
    focused->hide();

    for(int i=0;i<idOrden.size();i++){
        qDebug() << idOrden[i];
    }
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
        QWidget* pWidget = new QWidget();
        QPushButton* btn_edit = new QPushButton();
        btn_edit->setStyleSheet("border-image: url(:/iconos/minus.png);background-color: transparent;");
        btn_edit->setMinimumWidth(30);
        btn_edit->setMinimumHeight(30);
        btn_edit->setMaximumWidth(30);
        btn_edit->setMaximumHeight(30);
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->addWidget(btn_edit);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);
        ui->tablaItems->setCellWidget(fila, 5, pWidget);

        btn_edit->setProperty("row",fila);

        connect(btn_edit, SIGNAL (clicked()),this, SLOT (borrarItem()));
    }

    qDebug() << vercuenta.lastError();
}

int cancelar::val()
{
    return valor;
}
