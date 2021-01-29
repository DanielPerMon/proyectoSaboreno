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

    Edito = false;
    Id_Modificar = 0;
    /*------------------Llenado de tabla Empleados----------------------*/


    ActualizarTablaEmpleados();

    /*-------------------------------------------------------------------------*/
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
    ActualizarTablaEmpleados();
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

void gerente_menu::on_editar_tableWidget_cellClicked(int row) //Saber que empleado a sido seleccionado
{
    QTableWidgetItem *valor = ui->editar_tableWidget->item(row,0);
    QString valor_mandar = valor->text();

    Id_Modificar = valor_mandar.toInt();
    QueryEditarEmpleado(Id_Modificar);
    qDebug() << "Nip seleccionado : " << Id_Modificar;
}

void gerente_menu::QueryEditarEmpleado(int id)
{
    QSqlQuery empleados;

    /*Query para llenar la tabla resumida de los empleados*/
    if(empleados.prepare("select NombreEmpleado,ApellidoPaterno,"
                         "Correo,NumTelefono,perfil,NIP from empleado WHERE idEmpleado ="+ QString::number(id) +";"))
    {
        empleados.exec(); // LLenado de campos de edicion
        while(empleados.next()){
            ui->editnom_lineEdit->setText(empleados.value(0).toString());
            ui->editapell_lineEdit->setText(empleados.value(1).toString());
            ui->edittel_lineEdit->setText(empleados.value(2).toString());
            ui->editcorreo_lineEdit->setText(empleados.value(3).toString());
            ui->NipActualLineEdit->setText(empleados.value(5).toString());

            if(empleados.value(4).toString() == "Cajero"){
                ui->editocupacion_comboBox->setCurrentIndex(1);
            }else{
                if(empleados.value(4).toString() == "Mesero"){
                    ui->editocupacion_comboBox->setCurrentIndex(2);
                }else{
                    if(empleados.value(4).toString() == "Cocinero"){
                        ui->editocupacion_comboBox->setCurrentIndex(3);
                    }else{
                        if(empleados.value(4).toString() == "Bartender"){
                            ui->editocupacion_comboBox->setCurrentIndex(4);
                        }else{
                            ui->editocupacion_comboBox->setCurrentIndex(5);
                        }
                    }
                }
            }
        }
    }
}

void gerente_menu::on_edit_pushButton_clicked()
{
    if(Id_Modificar != 0){

        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("¿Seguro que quieres editar este empleado?");
        int ret = confirmacion.exec();
        if(ret == 0)
        {
            QSqlQuery *inserta = new QSqlQuery();

            //algún campo vacío
            if(ui->editnom_lineEdit->text() == "" || ui->editapell_lineEdit->text() == "" || ui->editcorreo_lineEdit->text() == ""
                    || ui->edittel_lineEdit->text() == ""){

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
                if(ui->editnip_lineEdit->text() != ui->editconnip_lineEdit->text()){
                    QMessageBox messageBox(QMessageBox::Information,
                                           tr("Error de credenciales"),
                                           tr("Los NIPs no coinciden"),
                                           QMessageBox::Yes,
                                           this);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();
                }
                else{
                    //Se insertan los campos editados
                    if(ui->editnip_lineEdit->text() != ""){
                        EditarEmpleado("NIP",ui->editnip_lineEdit->text(),Id_Modificar);
                    }
                    EditarEmpleado("NombreEmpleado",ui->editnom_lineEdit->text(),Id_Modificar);
                    EditarEmpleado("ApellidoPaterno",ui->editapell_lineEdit->text(),Id_Modificar);
                    EditarEmpleado("Correo",ui->editcorreo_lineEdit->text(),Id_Modificar);
                    EditarEmpleado("NumTelefono",ui->edittel_lineEdit->text(),Id_Modificar);

                    EditarEmpleado("Perfil",ui->editocupacion_comboBox->itemText(ui->editocupacion_comboBox->currentIndex()),Id_Modificar);
                    QMessageBox messageBox(QMessageBox::Information,
                                           tr("Éxito"),
                                           tr("Modificacion exitosa"),
                                           QMessageBox::Yes,
                                           this);
                    messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                    messageBox.exec();

                    ui->editnom_lineEdit->clear();
                    ui->editapell_lineEdit->clear();
                    ui->editcorreo_lineEdit->clear();
                    ui->edittel_lineEdit->clear();

                    ui->editnip_lineEdit->clear();
                    ui->editconnip_lineEdit->clear();
                    ui->editocupacion_comboBox->setCurrentIndex(0);
                    ui->NipActualLineEdit->setText("");
                    Id_Modificar = 0;
                    ActualizarTablaEmpleados();

                }
            }
            qDebug() << ui->ocupacion_comboBox->itemText(ui->ocupacion_comboBox->currentIndex());
            delete inserta;
        }

    }else{
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Informacion"),
                               tr("Seleccione un empleado"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    ActualizarTablaEmpleados();
}

void gerente_menu::EditarEmpleado(QString atributo,QString valor, int id)
{
    QSqlQuery InsertarMat;
    if(InsertarMat.exec("UPDATE empleado SET "+ atributo +" = '"+ valor +"' WHERE idEmpleado = '"+QString::number(id)+"'; "))
    {
        qDebug()<<"Se insertó" + atributo;
    }
    else
    {
        qDebug()<<"no se insertó" + valor;
        qDebug()<<InsertarMat.lastError();
    }
}

void gerente_menu::ActualizarTablaEmpleados()
{

    //ui->editar_tableWidget->clearContents();
    ui->editar_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->editar_tableWidget->clear();
    ui->editar_tableWidget->setRowCount(0);
    QStringList titulos;
    titulos<<"ID"<<"NOMBRE"<<"APELLIDOS"<<"PERFIL";
    ui->editar_tableWidget->setColumnWidth(0,20);
    ui->editar_tableWidget->setColumnWidth(1,130);
    ui->editar_tableWidget->setColumnWidth(2,130);
    ui->editar_tableWidget->setColumnWidth(3,100);
    ui->editar_tableWidget->setColumnCount(4);
    ui->editar_tableWidget->setHorizontalHeaderLabels(titulos);

    ui->editar_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->editar_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QSqlQuery empleados;

    /*Query para llenar la tabla resumida de los empleados*/
    if(empleados.prepare("select idEmpleado, NombreEmpleado,ApellidoPaterno,perfil from empleado where perfil != 'Gerente';"))
    {
        empleados.exec();
        while(empleados.next()){

            ui->editar_tableWidget->insertRow(ui->editar_tableWidget->rowCount());
            int fila2 = ui->editar_tableWidget->rowCount()-1;
            qDebug() << empleados.value(0).toInt();
            ui->editar_tableWidget->setItem(fila2,0,new QTableWidgetItem(empleados.value(0).toString()));
            ui->editar_tableWidget->setItem(fila2,1,new QTableWidgetItem(empleados.value(1).toString()));
            ui->editar_tableWidget->setItem(fila2,2,new QTableWidgetItem(empleados.value(2).toString()));
            ui->editar_tableWidget->setItem(fila2,3,new QTableWidgetItem(empleados.value(3).toString()));
        }
    }

}

void gerente_menu::on_eliminarU_pushButton_clicked()
{
    if(Id_Modificar != 0){
        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("¿Seguro que quieres eliminar este empleado?");
        int ret = confirmacion.exec();
        if(ret == 0)
        {
            QSqlQuery Elimina;
            if(Elimina.exec("DELETE from empleado WHERE idEmpleado = '"+QString::number(Id_Modificar)+"'; "))
            {
                qDebug()<<"Se Elimino";
                QMessageBox::warning(this,"Informacion","Se elimino el empleado");
                ui->editnom_lineEdit->clear();
                ui->editapell_lineEdit->clear();
                ui->editcorreo_lineEdit->clear();
                ui->edittel_lineEdit->clear();

                ui->editnip_lineEdit->clear();
                ui->editconnip_lineEdit->clear();
                ui->editocupacion_comboBox->setCurrentIndex(0);
                Id_Modificar = 0;
                ui->NipActualLineEdit->setText("");
                ActualizarTablaEmpleados();
            }
            else
            {
                qDebug()<<"No se elimino";
                qDebug()<<Elimina.lastError();
            }
        }else{

        }

    }
    else{
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Informacion"),
                               tr("Seleccione un empleado"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
}
