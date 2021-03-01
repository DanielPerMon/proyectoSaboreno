#include "pago.h"
#include "ui_pago.h"

Pago::Pago(QWidget *parent, int id, double monto) :
    QDialog(parent),
    ui(new Ui::Pago)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    Id_cuenta = id;
    Monto = monto;
    ui->monto->setText(ui->monto->text() + QString::number(monto) + ".00");
    ui->monto_2->setText(ui->monto_2->text() + QString::number(monto)+ ".00");
    qDebug() << " Id_cuenta " << id;
    qDebug() << " Monto: " << monto;
}

Pago::~Pago()
{
    delete ui;
}

void Pago::on_efectivo_radioButton_clicked()
{
 ui->pagoWidget->setCurrentIndex(0);
}

void Pago::on_tarjeta_radioButton_clicked()
{
    ui->pagoWidget->setCurrentIndex(1);
}

void Pago::on_pagar_pushButton_clicked()
{
    QSqlQuery finaliza;
    if(ui->efectivo_radioButton->isChecked()){ // Efectivo
        if(ui->lineEdit->text() != "" && (ui->lineEdit->text().toDouble() >= Monto)){
            QMessageBox confirmacion(this);
            confirmacion.addButton("Sí",QMessageBox::YesRole);
            confirmacion.addButton("No",QMessageBox::NoRole);
            confirmacion.setWindowTitle("Confirmación");
            confirmacion.setIcon(QMessageBox::Question);
            confirmacion.setText("¿Esta seguro de la siguiente transaccion?\n"
                                 "Cantidad recibida: $ "+ui->lineEdit->text()+".00\n"
                                                                              "Cambio a regresar: $ "+QString::number(ui->lineEdit->text().toDouble() - Monto) + ".00");
            int ret = confirmacion.exec();
            if(ret == 0)
            {
                /*Solicitud de pago */
                if(Id_cuenta != 0){
                    if(finaliza.exec("UPDATE cuenta SET estado = 'pagada' WHERE id_cuenta = '"+QString::number(Id_cuenta)+"'")){


                        QMessageBox *mbox = new QMessageBox;
                        mbox->setWindowTitle(tr("Pago Exitoso"));
                        mbox->setText("El pago se realizó con éxito");
                        mbox->setIconPixmap(QPixmap(":/iconos/cobrar.png").scaled(QSize(100,135),Qt::KeepAspectRatio));
                        mbox->show();
                        // Imprimir();
                        QTimer::singleShot(3000, mbox, SLOT(hide()));
                        emit pagada();
                        accept();
                        delete ui;
                    }
                    else
                        qDebug() << finaliza.lastError();


                }
            }
        }else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago incorrecto."),
                                   tr("Proporcione una cantidad recibida correcta"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }else{//Tarjeta
        if(ui->lineEdit_3->text() != ""){
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago Correcto Tarjeta."),
                                   tr("Su pago a sido registrado"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

        }else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago incorrecto."),
                                   tr("Proporcione el nombre del propietario"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }
}

void Pago::on_cancelar_pushButton_clicked()
{
    reject();
    delete ui;
}

