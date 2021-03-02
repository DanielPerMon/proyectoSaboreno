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

        comparacion.exec("SELECT fecha, hora, id_mesa FROM reservacion");
        while(comparacion.next()){

            if(comparacion.value(0).toDate() == fecha && comparacion.value(1).toTime() == hora &&
                    comparacion.value(2).toString() == nMesa){
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de Inserción"),
                                       "La mesa "+nMesa+" ya tiene reservación con esos parámetros",
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
