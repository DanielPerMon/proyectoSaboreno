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
    Id_Modificar = 0;//Indices seleccionados
    Id_Categoria = 0;//Indices seleccionados
    Id_Platillo = 0; //Indece del platillo a trabajar

    /*------------------Llenado de tabla Empleados----------------------*/


    ActualizarTablaEmpleados();

    /*-------------------------------------------------------------------------*/

    /*Configuraccion tabla Categorias*/
    ui->categorias_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categorias_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categorias_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->platillos_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->platillos_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->platillos_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /*-------------------------------*/
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
        confirmacion.setWindowTitle("Confirmación");
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
        confirmacion.setWindowTitle("Confirmación");
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

void gerente_menu::on_menu_pushButton_clicked()
{
    ui->Menu_gerente->setCurrentIndex(2);
}

/*void gerente_menu::on_anadir_pushButton_clicked()
{
    ui->Nuevo_Plato->show();
}*/


/*Metodo para actualiar el numero de platillos registrado por categoria*/
void gerente_menu::on_categorias_tableWidget_cellClicked(int row)
{
    QTableWidgetItem *valor = ui->categorias_tableWidget->item(row,0);
    QString valor_mandar = valor->text();
    qDebug() << "Fila seleccionada " << row;
    qDebug() << "valor obtenido" << valor_mandar;
    Id_Categoria = valor_mandar.toInt();
    QueryCuentaPLatillosCategoria(Id_Categoria);

    //QueryEditarEmpleado(Id_Modificar);
    //qDebug() << "Nip seleccionado : " << Id_Modificar;
}
/*   Metodo para abrir la ventana siguiente y llenar la tabla de acuerdo a categorias*/
void gerente_menu::on_categorias_tableWidget_cellDoubleClicked(int row)
{
    QTableWidgetItem *valor = ui->categorias_tableWidget->item(row,0);
    QString valor_mandar = valor->text();
    QTableWidgetItem *valor2 = ui->categorias_tableWidget->item(row,1);
    QString valor_mandar2 = valor2->text();
    Id_Categoria = valor_mandar.toInt();
    ActualizarTablaPlatillos(Id_Categoria);
    ui->Menu_gerente->setCurrentIndex(3);
    ui->Categoria_usada_label->setText( "Categoria actual:  " + valor_mandar2);
}

void gerente_menu::on_volver_categoria_pushButton_clicked()
{
    //Limpiar todo el formulario y la tabla
    ui->nom_lineEdit->clear();
    ui->precio_lineEdit->clear();
    ui->desc_textEdit->clear();
    ui->foto_label->clear();
    Id_Categoria = 0;
    ui->Menu_gerente->setCurrentIndex(2);
}
/*Botonoes de agregar nuevo platillo*/
void gerente_menu::on_Guardarplato_pushButton_clicked()
{
    //algún campo vacío
    if(ui->nuevo_nombre_lineEdit->text() == "" || ui->nuevoprecio_lineEdit->text() == "" || ui->nuevadesc_textEdit->toPlainText() == "" || archivo_foto == ""){

        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor rellene todos los campos"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }else{
        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setWindowTitle("Confirmación");
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("¿Seguro que quieres insertar este platillo?");
        int ret = confirmacion.exec();
        if(ret == 0)
        {
            QSqlQuery query;


            query.prepare("INSERT INTO platillos (id_Categoria, NombrePlatillo, precio,descripcion,imagen) "
                          "VALUES (:id_categoria, :nombre, :precio,:descripcion, :img)");
            query.bindValue(":id_categoria", Id_Categoria);
            query.bindValue(":nombre", ui->nuevo_nombre_lineEdit->text());
            query.bindValue(":precio", ui->nuevoprecio_lineEdit->text());
            query.bindValue(":descripcion", ui->nuevadesc_textEdit->toPlainText());

            QFile file(archivo_foto);
            file.open(QIODevice::ReadOnly);
            QByteArray bytes = file.readAll();
            query.bindValue(":img", QVariant(bytes));
            query.exec();

            QMessageBox messageBox2(QMessageBox::Information,
                                   tr("Platillo Insertado"),
                                   tr("El platillo se registró con éxito"),
                                   QMessageBox::Yes,
                                   this);
            messageBox2.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox2.exec();
            ui->nuevo_nombre_lineEdit->clear();
            ui->nuevoprecio_lineEdit->clear();
            ui->nuevadesc_textEdit->clear();
            ui->nuevafoto_label->clear();
            archivo_foto.clear();
             ActualizarTablaPlatillos(Id_Categoria);
            ui->Menu_gerente->setCurrentIndex(3);
        }


    }
}
/*Metodo para cancelar un registro de platillo*/
void gerente_menu::on_CancelarPlatopushButton_clicked()
{
    //Limpiar celdas de nuevo producto.
    ui->Menu_gerente->setCurrentIndex(3);
    ui->nuevo_nombre_lineEdit->clear();
    ui->nuevoprecio_lineEdit->clear();
    ui->nuevadesc_textEdit->clear();
    ui->nuevafoto_label->clear();
    archivo_foto.clear();

}
/*Botones de agredar nuevo platillo*/
void gerente_menu::on_Nuevo_platillo_pushButton_clicked()
{
   ui->Menu_gerente->setCurrentIndex(6);
}
/*Metodo para subir una foto a la interfaz de qt*/
void gerente_menu::on_nuevafoto_pushButton_clicked()
{
    archivo_foto = QFileDialog::getOpenFileName(this,tr("Abrir Archivo"),tr("/home/"),tr("Imagenes(*.png *.xpm *.jpg *.jpeg"));
    ui->nuevafoto_label->setPixmap(archivo_foto);
    qDebug() << "Ruta de la foto : " << archivo_foto;



}
/*Metodo para actualizar la tabla de acuerdo a la categoria se actualiza la tabla de platillos registrados*/
void gerente_menu::ActualizarTablaPlatillos(int id_categoria)
{
    //ui->editar_tableWidget->clearContents();
    ui->platillos_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->platillos_tableWidget->clear();
    ui->platillos_tableWidget->setRowCount(0);
    QStringList titulos;
    titulos<<"ID"<<"NOMBRE"<<"PRECIO";
    ui->platillos_tableWidget->setColumnWidth(0,30);
    ui->platillos_tableWidget->setColumnWidth(1,190);
    ui->platillos_tableWidget->setColumnWidth(2,70);
    ui->platillos_tableWidget->setColumnCount(3);
    ui->platillos_tableWidget->setHorizontalHeaderLabels(titulos);


    QSqlQuery platillos;

    /*Query para llenar la tabla resumida de los empleados*/
    if(platillos.prepare("select idPlatillos, NombrePlatillo,Precio from platillos where id_Categoria = "+QString::number(id_categoria)+";"))
    {
        platillos.exec();
        while(platillos.next()){

            ui->platillos_tableWidget->insertRow(ui->platillos_tableWidget->rowCount());
            int fila2 = ui->platillos_tableWidget->rowCount()-1;
            ui->platillos_tableWidget->setItem(fila2,0,new QTableWidgetItem(platillos.value(0).toString()));
            ui->platillos_tableWidget->setItem(fila2,1,new QTableWidgetItem(platillos.value(1).toString()));
            ui->platillos_tableWidget->setItem(fila2,2,new QTableWidgetItem("$"+platillos.value(2).toString()+".00"));
        }
    }

}
/*Metodo para resaltar un platillo a modificar*/
void gerente_menu::on_platillos_tableWidget_cellClicked(int row)
{
    QTableWidgetItem *valor = ui->platillos_tableWidget->item(row,0);
    QString valor_mandar = valor->text();
    qDebug() << "Fila seleccionada " << row;
    qDebug() << "valor obtenido" << valor_mandar;
    Id_Platillo = valor_mandar.toInt();
    QueryEditarPlatillo(Id_Platillo);
    qDebug() << "Nip platillo seleccionado : " << Id_Platillo;


}

void gerente_menu::QueryEditarPlatillo(int id)
{
    QSqlQuery platillo;
    QPixmap pix;
    /*Query para llenar la tabla resumida de los empleados*/
    if(platillo.prepare("select NombrePlatillo,Precio,"
                         "descripcion,imagen from platillos WHERE idPlatillos ="+ QString::number(id) +";"))
    {
        platillo.exec(); // LLenado de campos de edicion
        while(platillo.next()){
            ui->nom_lineEdit->setText(platillo.value(0).toString());
            ui->precio_lineEdit->setText("$"+platillo.value(1).toString()+".00");
            ui->desc_textEdit->setText(platillo.value(2).toString());
            pix.loadFromData(platillo.value(3).toByteArray());
            ui->foto_label->setPixmap(pix);
        }
    }

}

void gerente_menu::on_eliminaplatillo_pushButton_clicked()
{
    if(Id_Platillo != 0){
        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setWindowTitle("Confirmación");
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("¿Seguro que quieres eliminar este platillo?");
        int ret = confirmacion.exec();
        if(ret == 0)
        {
            QSqlQuery Elimina;
            if(Elimina.exec("DELETE from platillos WHERE idPlatillos = '"+QString::number(Id_Platillo)+"'; "))
            {
                qDebug()<<"Se Elimino el platillo";
                QMessageBox::warning(this,"Informacion","Se elimino el platillo");
                ui->nom_lineEdit->clear();
                ui->precio_lineEdit->clear();
                ui->desc_textEdit->clear();
                ui->foto_label->clear();
                Id_Platillo = 0;

                ActualizarTablaPlatillos(Id_Categoria);
            }
            else
            {
                qDebug()<<"No se elimino platillo";
                qDebug()<<Elimina.lastError();
            }
        }else{
            qDebug() << "Prefirio no eliminar platillo";
        }

    }
    else{
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Informacion"),
                               tr("Seleccione un platillo"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
}
/*Cuenta el numero de platillos dada un id de categoria*/
void gerente_menu::QueryCuentaPLatillosCategoria(int id)
{
    QSqlQuery platillo;

    /*Query para llenar la tabla resumida de los empleados*/
    if(platillo.prepare("select count(*) from platillos where id_Categoria = "+QString::number(id)+";"))
    {
        platillo.exec(); // LLenado de campos de edicion
        while(platillo.next()){
        ui->num_platilloslabel->setText(platillo.value(0).toString());
        }
    }
}

void gerente_menu::on_editarPlato_push_clicked()
{


    QMessageBox confirmacion(this);
    confirmacion.addButton("Sí",QMessageBox::YesRole);
    confirmacion.addButton("No",QMessageBox::NoRole);
    confirmacion.setWindowTitle("Confirmación");
    confirmacion.setIcon(QMessageBox::Question);
    confirmacion.setText("¿Seguro que desea editar el platillo?");
    int ret = confirmacion.exec();
    if(ret == 0)
    {
        QSqlQuery platillo;
        if( platillo.exec("UPDATE platillos SET NombrePlatillo = '"+ui->nom_lineEdit->text()+"', Precio = '"+ui->precio_lineEdit->text()+"',"
                          "descripcion = '"+ui->desc_textEdit->toPlainText()+"' WHERE idPlatillos = '"+QString::number(Id_Platillo)+"'; "))
        {
            QMessageBox messageBox2(QMessageBox::Information,
                                   tr("Cambios Completos"),
                                   tr("Se editó la información con éxito"),
                                   QMessageBox::Yes,
                                   this);
            messageBox2.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox2.exec();

            ActualizarTablaPlatillos(Id_Categoria);
        }
        else
        {
            qDebug()<<"Error de edición";
            qDebug()<<platillo.lastError();
        }
    }


}

void gerente_menu::on_inventario_btn_clicked()
{
    ui->Menu_gerente->setCurrentIndex(5);
}

void gerente_menu::on_ventas_btn_clicked()
{
    ui->Menu_gerente->setCurrentIndex(4);
}
