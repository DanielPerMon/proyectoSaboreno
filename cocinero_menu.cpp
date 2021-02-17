#include "cocinero_menu.h"
#include "ui_cocinero_menu.h"

cocinero_menu::cocinero_menu(QSqlDatabase base, QString usuario, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cocinero_menu)
{
    ui->setupUi(this);
    baseCocinero = base;
    setWindowTitle("Saboreño Cocinero");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->lblCocinero->setText(ui->lblCocinero->text()+" "+usuario);
    ui->lblCocinero->setWordWrap(true);
    FilaOrdenCola = 0;//Por defecto es la cola

    if(baseCocinero.isOpen())
        qDebug() << "está abierta la bdd";
    else
        qDebug() << "error en la base";
    qDebug() << "Numero de filas es : " << ui->orden_tableWidget->rowCount();

    ActualizarTablas();

    if(ui->orden_tableWidget->rowCount()!= 0){
    QTableWidgetItem *valor = ui->orden_tableWidget->item(0,0);
    int valor_mandar = valor->text().toInt();
    OrdenElegida(valor_mandar);
    }
}

cocinero_menu::~cocinero_menu()
{
    delete ui;
}



void cocinero_menu::on_ordenlista_pushButton_clicked()
{
 if(ui->orden_tableWidget->rowCount()!= 0){
    QTableWidgetItem *valor = ui->orden_tableWidget->item(0,0);
    int id_cocinado = valor->text().toInt();
    QSqlQuery finaliza;
    if(finaliza.exec("UPDATE orden SET estado = '1' WHERE id_orden = '"+QString::number(id_cocinado)+"'")){

        QMessageBox *mbox = new QMessageBox;
            mbox->setWindowTitle(tr("Orden Lista"));
            mbox->setText("Notificacion Exitosa");
            mbox->setIconPixmap(QPixmap(":/iconos/ordenlista.png").scaled(QSize(185,215),Qt::KeepAspectRatio));
            mbox->show();
            QTimer::singleShot(2000, mbox, SLOT(hide()));
    }
    else
        qDebug() << finaliza.lastError();




    /*Primero se ejecuta la orden*/
    ActualizarTablas();

    QTableWidgetItem *valor2 = ui->orden_tableWidget->item(0,0);
    int valor_mandar = valor2->text().toInt();
    OrdenElegida(valor_mandar);
 }else{
     QMessageBox messageBox(QMessageBox::Information,
                            tr("Cola de Espera"),
                            tr("No hay ordenes a preparar"),
                            QMessageBox::Yes,
                            this);
     messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
     messageBox.exec();
 }
}

void cocinero_menu::ActualizarTablas() // Funcion que se ejecutara cada que el cocinero relize una orden;
{
    QSqlQuery *verordenes = new QSqlQuery;
    QSqlQuery *verordeneslistas = new QSqlQuery;
    QSqlQuery *contar = new QSqlQuery;
    ui->orden_tableWidget->setRowCount(0);

    contar->exec("SELECT count(1) numeroOrdenes, o.id_orden, c.idmesa FROM orden as o INNER JOIN cuenta as c ON o.id_cuenta= c.id_cuenta WHERE o.estado = '0';");
    int OrdenesCola = 0;
    while(contar->next()){
        OrdenesCola = contar->value(0).toInt();
        qDebug()<< "Ordenes Contadas : " << OrdenesCola;
    }
    if(OrdenesCola != 0){
    verordenes->exec("SELECT o.id_orden, c.idmesa FROM orden as o INNER JOIN cuenta as c ON o.id_cuenta= c.id_cuenta "
                     "WHERE o.estado = '0'; ");

    int fila;

    while(verordenes->next()){

        fila = ui->orden_tableWidget->rowCount();
        ui->orden_tableWidget->insertRow(ui->orden_tableWidget->rowCount());
        ui->orden_tableWidget->setItem(fila,0,new QTableWidgetItem(verordenes->value(0).toString()));
        ui->orden_tableWidget->setItem(fila,1,new QTableWidgetItem(verordenes->value(1).toString()));

    }
    ui->orden_tableWidget->item(0,1)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
    ui->orden_tableWidget->item(0,0)->setData(Qt::BackgroundRole, QVariant(QColor(255, 0, 0, 127)));
    }
    /*Lenado de tabla orden listas*/
    ui->orden_lista_tableWidget->setRowCount(0);

/*Puede fallar el query*/
    verordeneslistas->exec("SELECT o.id_orden, c.idmesa from orden as o inner join cuenta as c ON o.id_cuenta= c.id_cuenta WHERE o.estado = '1'"
                          " ORDER BY id_orden desc LIMIT 10;");

    int fila2;
    while(verordeneslistas->next()){
        fila2 = ui->orden_lista_tableWidget->rowCount();
        ui->orden_lista_tableWidget->insertRow(ui->orden_lista_tableWidget->rowCount());
        ui->orden_lista_tableWidget->setItem(fila2,0,new QTableWidgetItem(verordeneslistas->value(0).toString()));
        ui->orden_lista_tableWidget->setItem(fila2,1,new QTableWidgetItem(verordeneslistas->value(1).toString()));

        ui->orden_lista_tableWidget->item(fila2,1)->setData(Qt::BackgroundRole, QVariant(QColor(0, 255, 0, 127)));
        ui->orden_lista_tableWidget->item(fila2,0)->setData(Qt::BackgroundRole, QVariant(QColor(0, 255, 0, 127)));

    }




    delete verordenes;
    delete verordeneslistas;
}

void cocinero_menu::OrdenElegida(int id_orden)
{
    QSqlQuery platillo;
    QPixmap pix;
    /*Query para llenar la tabla resumida de los empleados*/
    if(platillo.prepare("SELECT p.NombrePlatillo,p.imagen,o.cantidadPlatillo,o.detalles FROM platillos as p INNER JOIN orden as o ON "
                         "p.idPlatillos = o.id_platillo WHERE id_orden = " + QString::number(id_orden)+";"))
    {
        platillo.exec(); // LLenado de campos de edicion
        while(platillo.next()){
            ui->nombre_label->setText(platillo.value(0).toString());
            pix.loadFromData(platillo.value(1).toByteArray());
            ui->foto_label->setPixmap(pix);
            ui->cantidad_label->setText(platillo.value(2).toString());
            ui->detalles_label->setText(platillo.value(3).toString());

        }
    }



}

void cocinero_menu::on_Csalir_pushButton_clicked()
{
    this->close();
    delete ui;
}
