#include "gerente_menu.h"
#include "ui_gerente_menu.h"

gerente_menu::gerente_menu(QSqlDatabase base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gerente_menu)
{
    ui->setupUi(this);
    setWindowTitle("Saboreño Gerente");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    baseGerente = base;
    principal = parent;

    if(baseGerente.isOpen())
        qDebug() << "está abierta la bdd";
    else
        qDebug() << "error en la base";
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

//Validamos los 3 posibles errores al insertar un usuario
void gerente_menu::on_acept_pushButton_clicked()
{
    QSqlQuery *inserta = new QSqlQuery();

    //algún campo vacío
    if(ui->nombre_lineEdit->text() == "" || ui->apellido_lineEdit->text() == "" || ui->correo_lineEdit->text() == ""
            || ui->telefono_lineEdit->text() == "" || ui->nip_lineEdit->text() == "" || ui->confirmanip_lineEdit->text() == ""){

        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor rellene todos los campos"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else{
        //los nip no coiciden
        if(ui->nip_lineEdit->text() != ui->confirmanip_lineEdit->text()){
            QMessageBox messageBox(QMessageBox::Information,
                                   tr("Error de credenciales"),
                                   tr("Los NIPs no coinciden"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
        else{
            //no se ha seleccionado ninguna ocupación
            if(ui->ocupacion_comboBox->currentIndex() == 0){
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error"),
                                       tr("Seleccione una ocupación"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
            else{
                //Se insertan los campos correctos
                if(inserta->exec("insert into empleado(NombreEmpleado,ApellidoPaterno,Correo,NumTelefono,Perfil,NIP)"
                                 "values('"+ui->nombre_lineEdit->text()+"','"+ui->apellido_lineEdit->text()+"','"+ui->correo_lineEdit->text()+"',"
                                 "'"+ui->telefono_lineEdit->text()+"','"+ui->ocupacion_comboBox->itemText(ui->ocupacion_comboBox->currentIndex())+"',"
                                 "'"+ui->nip_lineEdit->text()+"')")){
                    QMessageBox messageBox(QMessageBox::Information,
                                           tr("Éxito"),
                                           tr("Se agregó con éxito el nuevo usuario"),
                                           QMessageBox::Yes,
                                           this);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                }
                else
                    qDebug() << "error de inserción" << inserta->lastError();

                ui->nombre_lineEdit->clear();
                ui->apellido_lineEdit->clear();
                ui->correo_lineEdit->clear();
                ui->telefono_lineEdit->clear();

                ui->nip_lineEdit->clear();
                ui->confirmanip_lineEdit->clear();
                ui->ocupacion_comboBox->setCurrentIndex(0);
            }
        }
    }
    qDebug() << ui->ocupacion_comboBox->itemText(ui->ocupacion_comboBox->currentIndex());
    delete inserta;
}

void gerente_menu::on_cancel_pushButton_clicked()
{
    ui->nombre_lineEdit->clear();
    ui->apellido_lineEdit->clear();
    ui->correo_lineEdit->clear();
    ui->telefono_lineEdit->clear();

    ui->nip_lineEdit->clear();
    ui->confirmanip_lineEdit->clear();
    ui->ocupacion_comboBox->setCurrentIndex(0);
}

void gerente_menu::on_Gsalir_pushButton_clicked()
{
    this->close();
    principal->show();
}
