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
    monto_enviar = 0;
    FilaCuentas = -1;
    if(baseCajero.isOpen())
        qDebug() << "está abierta la bdd";
    else
        qDebug() << "error en la base";
    //qDebug() << "Numero de filas es : " << ui->orden_tableWidget->rowCount();

    ActualizarTabla();


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

void cajero_menu::on_cobrar_pushButton_clicked()
{
    ui->menuCajero->setCurrentIndex(0);
}

void cajero_menu::ActualizarTabla()
{
    QSqlQuery *vercuentas = new QSqlQuery;
    QSqlQuery *contar = new QSqlQuery;
    ui->Cuentas_tableWidget->setRowCount(0);
    ui->Cuentas_tableWidget->setColumnWidth(2,170);
    QString Signo = "       $ ";
    contar->exec("SELECT count(1) numero_ordenes FROM cuenta WHERE estado = 'finalizada';");
    int OrdenesCola = 0;
    while(contar->next()){
        OrdenesCola = contar->value(0).toInt();
        qDebug()<< "Ordenes Contadas : " << OrdenesCola;
    }
    if(OrdenesCola != 0){
    vercuentas->exec("SELECT id_cuenta, idmesa, total FROM cuenta WHERE estado = 'finalizada';");
    int fila;

    while(vercuentas->next()){

        fila = ui->Cuentas_tableWidget->rowCount();
        ui->Cuentas_tableWidget->insertRow(ui->Cuentas_tableWidget->rowCount());
        ui->Cuentas_tableWidget->setItem(fila,0,new QTableWidgetItem(vercuentas->value(0).toString()));
        ui->Cuentas_tableWidget->setItem(fila,1,new QTableWidgetItem(vercuentas->value(1).toString()));
        ui->Cuentas_tableWidget->setItem(fila,2,new QTableWidgetItem(vercuentas->value(2).toString() + ".00"));

    }
  //  ui->orden_tableWidget->item(0,1)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
  //  ui->orden_tableWidget->item(0,0)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
    }

    delete vercuentas;
    delete contar;
}

void cajero_menu::CuentaElegida(int id_cuenta)
{
    /*Hay riesgo de error si la cuenta no tiene platillos*/
    QSqlQuery platillo;
    QSqlQuery cuenta;

    /*Query para llenar el detalle de la cuenta*/

    if(cuenta.prepare("SELECT id_cuenta, idmesa, total FROM cuenta WHERE id_cuenta= " + QString::number(id_cuenta)+";"))
    {
        cuenta.exec(); // LLenado de campos de edicion
        while(cuenta.next()){
            ui->order_label->setText(cuenta.value(0).toString());
            ui->m_label->setText(cuenta.value(1).toString());
            ui->pagar_label->setText(cuenta.value(2).toString() + ".00");
        }
    }
    QString Signo = "     $";
    ui->DetallesC_tableWidget->setRowCount(0);
    if(platillo.prepare("SELECT P.NombrePlatillo, P.Precio, O.cantidadPlatillo FROM platillos as P INNER JOIN orden as O ON "
                        "P.idPlatillos = O.id_platillo WHERE O.id_cuenta = " + QString::number(id_cuenta)+" "
                        "AND O.estado != '2';"))
    {
        int fila;
        platillo.exec(); // LLenado de campos de edicion
        while(platillo.next()){
            fila = ui->DetallesC_tableWidget->rowCount();
            ui->DetallesC_tableWidget->insertRow(ui->DetallesC_tableWidget->rowCount());
            ui->DetallesC_tableWidget->setItem(fila,0,new QTableWidgetItem(platillo.value(0).toString()));
            ui->DetallesC_tableWidget->setItem(fila,1,new QTableWidgetItem(platillo.value(1).toString()));
            ui->DetallesC_tableWidget->setItem(fila,2,new QTableWidgetItem(platillo.value(2).toString()));
            double sub = platillo.value(2).toDouble() * platillo.value(1).toDouble();
            QString SubTotal = QString::number(sub);
            ui->DetallesC_tableWidget->setItem(fila,3,new QTableWidgetItem(Signo + SubTotal + ".00"));


        }

    }
    else{
    qDebug() << platillo.lastError();
    }
}

void cajero_menu::CuentaElegidaDet(int id_cuenta)
{
    /*Hay riesgo de error si la cuenta no tiene platillos*/
    QSqlQuery platillo;
    QSqlQuery cuenta;

    /*Query para llenar el detalle de la cuenta*/

    if(cuenta.prepare("SELECT id_cuenta, idmesa, total, estado FROM cuenta WHERE id_cuenta= " + QString::number(id_cuenta)+";"))
    {
        cuenta.exec(); // LLenado de campos de edicion
        while(cuenta.next()){
            ui->orden_label->setText(cuenta.value(0).toString());
            ui->mesa_label->setText(cuenta.value(1).toString());
            ui->total_label->setText(cuenta.value(2).toString() + ".00");
            ui->estado_label->setText(cuenta.value(3).toString());
        }
    }
    QString Signo = "     $";
    ui->detallesH_tableWidget->setRowCount(0);
    if(platillo.prepare("SELECT P.NombrePlatillo, P.Precio, O.cantidadPlatillo FROM platillos as P INNER JOIN orden as O ON P.idPlatillos = O.id_platillo "
                        "WHERE O.id_cuenta = " + QString::number(id_cuenta)+" AND O.estado != '2';")){

        int fila;
        platillo.exec(); // LLenado de campos de edicion
        while(platillo.next()){
            fila = ui->detallesH_tableWidget->rowCount();
            ui->detallesH_tableWidget->insertRow(ui->detallesH_tableWidget->rowCount());
            ui->detallesH_tableWidget->setItem(fila,0,new QTableWidgetItem(platillo.value(0).toString()));
            ui->detallesH_tableWidget->setItem(fila,2,new QTableWidgetItem(platillo.value(1).toString()));
            ui->detallesH_tableWidget->setItem(fila,1,new QTableWidgetItem(platillo.value(2).toString()));
            double sub = platillo.value(2).toDouble() * platillo.value(1).toDouble();
            QString SubTotal = QString::number(sub);
            ui->detallesH_tableWidget->setItem(fila,3,new QTableWidgetItem(Signo + SubTotal + ".00"));


        }

    }
    else{
    qDebug() << platillo.lastError();
    }
}

void cajero_menu::on_Cuentas_tableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem *valor = ui->Cuentas_tableWidget->item(row,0);
    FilaCuentas = valor->text().toInt();
    QTableWidgetItem *valor2 = ui->Cuentas_tableWidget->item(row,2);
    monto_enviar = valor2->text().toDouble();
    QTableWidgetItem *table = ui->Cuentas_tableWidget->item(row,1);
    mesa = table->text().toDouble();
    CuentaElegida(FilaCuentas);
}

void cajero_menu::on_cobrar_pushButton_2_clicked()
{
    if(FilaCuentas == -1){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Error"),
                               tr("Seleccione una cuenta para la operación"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        Pago *ventana = new Pago(this,FilaCuentas,monto_enviar);
        ventana->exec();
        connect(ventana,SIGNAL(pagada()), this, SLOT(ActualizarTabla()));

        ui->DetallesC_tableWidget->setRowCount(0);
    }
    ActualizarTabla();
}


void cajero_menu::on_btnHistorial_clicked()
{
    ui->menuCajero->setCurrentIndex(2);
    QSqlQuery *vercuentas = new QSqlQuery;
    QSqlQuery *contar = new QSqlQuery;
    ui->Historial_tableWidget->setRowCount(0);
    ui->Historial_tableWidget->setColumnWidth(2,170);
    QString Signo = "       $ ";
    contar->exec("SELECT count(1) numero_ordenes FROM cuenta WHERE estado = 'pagada' OR estado = 'cancelada';");
    int OrdenesCola = 0;
    while(contar->next()){
        OrdenesCola = contar->value(0).toInt();
        qDebug()<< "Ordenes Contadas : " << OrdenesCola;
    }
    if(OrdenesCola != 0){
    vercuentas->exec("SELECT id_cuenta, idmesa, total FROM cuenta WHERE estado = 'pagada' OR estado = 'cancelada' ;");
    int fila;

    while(vercuentas->next()){

        fila = ui->Historial_tableWidget->rowCount();
        ui->Historial_tableWidget->insertRow(ui->Historial_tableWidget->rowCount());
        ui->Historial_tableWidget->setItem(fila,0,new QTableWidgetItem(vercuentas->value(0).toString()));
        ui->Historial_tableWidget->setItem(fila,1,new QTableWidgetItem(vercuentas->value(1).toString()));
        ui->Historial_tableWidget->setItem(fila,2,new QTableWidgetItem(vercuentas->value(2).toString() + ".00"));

    }
  //  ui->orden_tableWidget->item(0,1)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
  //  ui->orden_tableWidget->item(0,0)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
    }

    delete vercuentas;
    delete contar;
}

void cajero_menu::on_Historial_tableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem *valor = ui->Historial_tableWidget->item(row,0);
    FilaDet = valor->text().toInt();
    qDebug() << FilaDet;
    QTableWidgetItem *valor2 = ui->Historial_tableWidget->item(row,2);
    monto_enviar = valor2->text().toDouble();
    CuentaElegidaDet(FilaDet);
}

void cajero_menu::on_btnEliminaCuenta_clicked()
{
    if(FilaCuentas == -1){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Error"),
                               tr("Seleccione una cuenta para la operación"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setWindowTitle("Confirmación");
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("Se cancelará toda la cuenta "+QString::number(FilaCuentas)+"\n¿Desea continuar?");
        int res = confirmacion.exec();
        if(res == 0){
            autoriza *ventana = new autoriza(this);
            ventana->exec();

            if(ventana->valor() == 1){
                qDebug() << "está autorizado";
                QSqlQuery cancela;
                cancela.exec("UPDATE cuenta SET estado = 'cancelada' WHERE id_cuenta = '"+QString::number(FilaCuentas)+"'");
                FilaCuentas = -1;
                ActualizarTabla();
            }
            else{
                qDebug() << "No está autorizado";
                ventana->close();
                delete ventana;
            }
        }
    }
}

void cajero_menu::on_btnCancelarProducto_clicked()
{
    if(FilaCuentas == -1){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Error"),
                               tr("Seleccione una cuenta para la operación"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        cancelar *ventana = new cancelar(this,FilaCuentas,mesa);
        ventana->exec();
        CuentaElegida(FilaCuentas);
    }
}

void cajero_menu::on_btnImprimir_clicked()
{
    imprime(FilaCuentas);
}

void cajero_menu::on_imprimirHistorial_clicked()
{
    imprime(FilaDet);
}

void cajero_menu::imprime(int numcuenta)
{
    QString numero = QString::number(numcuenta);
    QString nom = "C:/Users/Daniel Pérez/Desktop/Recibo"+numero+".pdf"; //Ruta General

    QDate aux = QDate::currentDate();
    QString aux_fecha = aux.toString("yyyy/MM/dd");
    QString ruta_img = ":/imagenes/LOGO.png";
    QString html = "<body style='background-color:#F4F4F4;'>"
                   "<img src='"+ ruta_img +"' width='10px' height='10px' style='margin-left:100px;'>"

                                           "<div><p style='margin-top:50px; margin-left:100px; font-size:15px;'><b>SABOREÑO <br>'EL MERO MERO SABOR NORTEÑO'</b></p></div>"
                                           "<div style='margin-top:100px; margin-left:150px; border : 2px solid black;'><p>CALAJAN 2000 MNL 807670497</p><p>Prolongacion Juan de Borbo</p>"
                                           "<p>Telefono: 023 4458923 </p><p>Fecha: "+ aux_fecha+"</p>";

    html += "<hr>";



    QSqlQuery platillo;
    QSqlQuery cuenta;


    QString htmlbody;

    if(platillo.prepare("SELECT P.NombrePlatillo, P.Precio, O.cantidadPlatillo FROM platillos as P INNER JOIN orden as O ON P.idPlatillos = O.id_platillo WHERE O.id_cuenta = " + numero +";"))
    {
        platillo.exec(); // LLenado de campos de edicion
        htmlbody+="<p><b>Nombre de platillo </b>                               <b>Precio</b>"
                  "                             <b>Cantidad</b>                               <b>SubTotal</b></p>";
        while(platillo.next()){
            double sub = platillo.value(2).toDouble() * platillo.value(1).toDouble();
            QString SubTotal = QString::number(sub);
            htmlbody += "<p>"+ platillo.value(0).toString() + "                                                      $" + platillo.value(1).toString()
                    + ".00                                                    " + platillo.value(2).toString()+ "                                             $"+SubTotal+".00</p>";
        }


    }
    else{
        qDebug() << platillo.lastError();
    }
    htmlbody+="<hr>";
    //Query para llenar el detalle de la cuenta/
    QString htmlfinal;
    if(cuenta.prepare("SELECT id_cuenta, idmesa, total, estado FROM cuenta WHERE id_cuenta= " + numero +";"))
    {
        cuenta.exec(); // LLenado de campos de edicion
        while(cuenta.next()){
            htmlfinal+= "<p>   Numero de cuenta: "+ cuenta.value(0).toString() +
                    "     Numero de Mesa: "+ cuenta.value(1).toString()+
                    "</p><p>Total: <b>$ "+ cuenta.value(2).toString()+".00</b></p></body></html>";
        }
    }



    QString Kardex_html = html + htmlbody + htmlfinal;
    QTextDocument document;
    document.setHtml(Kardex_html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(nom);
    printer.setPageMargins(QMarginsF(45, 25, 45, 25));

    document.print(&printer);

    QMessageBox *mbox = new QMessageBox;
    mbox->setWindowTitle(tr("Pago Exitoso"));
    mbox->setText("Imprimiendo ticket");
    mbox->setIconPixmap(QPixmap(":/iconos/ticket.png").scaled(QSize(185,215),Qt::KeepAspectRatio));
    mbox->show();

    QTimer::singleShot(3000, mbox, SLOT(hide()));
}

void cajero_menu::on_corte_caja_clicked()
{
   ui->menuCajero->setCurrentIndex(1);
}
