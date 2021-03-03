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

    ui->res_calendarWidget->setMinimumDate(QDate::currentDate());
    nombre = "";
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
    ActualizarTabla();
}

void anfitrion_menu::on_btnAgendarRes_clicked()
{
    if(ui->nom->text() == "" || ui->num_telefono->text() == "" || ui->personas->text() == ""){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Campos Vacíos"),
                               tr("LLene todos los campos"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        QSqlQuery reservacion, comparacion;
        QDate fecha = ui->res_calendarWidget->selectedDate();
        QString f_entrada = fecha.toString("yyyyMMdd");
        QTime hora = ui->res_timeEdit->time();
        QString h_entrada = hora.toString();

        QString nMesa = ui->mesa_comboBox->currentText().right(1);
        qDebug() << f_entrada << h_entrada <<  nMesa;

        comparacion.exec("SELECT fecha, hora, id_mesa, nombreCliente FROM reservacion");
        while(comparacion.next()){

            if(comparacion.value(0).toDate() == fecha && comparacion.value(1).toTime() == hora &&
                    comparacion.value(2).toString() == nMesa ){
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de Inserción"),
                                       "La mesa "+nMesa+" ya tiene reservación con esos parámetros",
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }

            else{
                if(comparacion.value(3).toString() == ui->nom->text()){
                    QMessageBox messageBox(QMessageBox::Information,
                                           tr("Error de Inserción"),
                                           "Ya hay una reservación a ese nombre",
                                           QMessageBox::Yes,
                                           this);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                }
                else{
                    reservacion.exec("INSERT INTO reservacion(fecha,hora,nombreCliente,numPersonas,telefono,id_mesa)"
                                     "VALUES('"+f_entrada+"','"+h_entrada+"','"+ui->nom->text()+"','"+ui->personas->text()+"','"+ui->num_telefono->text()+"',"
                                     "'"+ui->mesa_comboBox->currentText().right(1)+"')");
                    QMessageBox messageBox(QMessageBox::Information,
                                           tr("Inserción Correcta"),
                                           tr("Se ha agendado la reservación"),
                                           QMessageBox::Yes,
                                           this);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                }
            }

        }
    }
}

void anfitrion_menu::ActualizarTabla()
{
    ui->Reserva_tableWidget->setRowCount(0);
    QSqlQuery reserva;
    int fila;

    reserva.exec("SELECT fecha, hora, nombreCliente, numPersonas, telefono, id_mesa from reservacion WHERE estado = '0'");
    while(reserva.next()){
        QDate da=QDate::fromString(reserva.value(0).toString(),"yyyy-MM-dd");
        QString dw = da.toString("dd-MM-yyyy");
        fila = ui->Reserva_tableWidget->rowCount();
        ui->Reserva_tableWidget->insertRow(fila);
        ui->Reserva_tableWidget->setItem(fila,0,new QTableWidgetItem(reserva.value(2).toString()));
        ui->Reserva_tableWidget->setItem(fila,1,new QTableWidgetItem(reserva.value(4).toString()));
        ui->Reserva_tableWidget->setItem(fila,2,new QTableWidgetItem(reserva.value(5).toString()));
        ui->Reserva_tableWidget->setItem(fila,3,new QTableWidgetItem(reserva.value(3).toString()));
        ui->Reserva_tableWidget->setItem(fila,4,new QTableWidgetItem(dw));
        ui->Reserva_tableWidget->setItem(fila,5,new QTableWidgetItem(reserva.value(1).toString()));
    }

    qDebug() << reserva.lastError();
}

void anfitrion_menu::on_btnCancelReserva_clicked()
{
    if(nombre == ""){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Error"),
                               tr("Seleccione una reservación para cancelar"),
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
        confirmacion.setText("Se cancelará toda la reservación a nombre de "+nombre+"\n¿Desea continuar?");
        int res = confirmacion.exec();
        if(res == 0){
            QSqlQuery elimina;
            elimina.exec("DELETE FROM reservacion WHERE nombreCliente = '"+nombre+"' AND estado = '0'");
            ActualizarTabla();
            nombre = "";
        }
    }
}

void anfitrion_menu::on_Reserva_tableWidget_cellClicked(int row, int column)
{
    nombre = ui->Reserva_tableWidget->item(row,0)->text();
}
