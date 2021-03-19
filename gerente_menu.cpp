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
    ui->editarProducto_groupBox->hide();

    /*............... Graficas de Ventas ................*/
    totalVentas = 0;
    id_cuentas = new QList<int>;
    for(int i = 0; i < 7; i++){
        Ganancias_Categorias[i] = 0.0;
    }
    QDate Aux = QDate::currentDate();
    QString f = Aux.toString("dd/MM/yyyy");
    ActualizarVectorCategorias(f);


    /*Llenado de grafica 1*/
    series = new QVector<QBarSeries*>({0,0,0,0,0,0,0});

    //RECORRE EL VECTOR PARA INICIALIZAR CADA QBARSERIES
    for(int itrSerie=0;itrSerie<series->count();itrSerie++)
        series->operator[](itrSerie) = new QBarSeries();

    //INICIALIZA LOS QBARSET
    QBarSet *set1 = new QBarSet("Entradas");
    QBarSet *set2 = new QBarSet("Sopas");
    QBarSet *set3 = new QBarSet("Ensaladas");
    QBarSet *set4 = new QBarSet("Carnes");
    QBarSet *set5 = new QBarSet("Especialidades");
    QBarSet *set6 = new QBarSet("Postres");
    QBarSet *set7 = new QBarSet("Bebidas");

    //INSERTA VALORES DE CADA QBARSET
    *set1 << Ganancias_Categorias[0];
    *set2 << Ganancias_Categorias[1];
    *set3 << Ganancias_Categorias[2];
    *set4 << Ganancias_Categorias[3];
    *set5 << Ganancias_Categorias[4];
    *set6 << Ganancias_Categorias[5];
    *set7 << Ganancias_Categorias[6];

    //INSERTA LOS BARSET A LOS QBARSERIES
    series->at(0)->append(set1);
    series->at(1)->append(set2);
    series->at(2)->append(set3);
    series->at(3)->append(set4);
    series->at(4)->append(set5);
    series->at(5)->append(set6);
    series->at(6)->append(set7);

    //INICIALIZA EL QCHART
    QChart *chart = new QChart();

    //INSERTA CADA QBARSERIES AL QCHART
    for(int itrSerie=0;itrSerie<series->count();itrSerie++)
        chart->addSeries(series->at(itrSerie));


    //INICIALIZA EJE X CON SUS RESPECTIVOS CAMPOS
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList({"Entradas","Sopas","Ensaladas","Carnes","Especialidades","Postres","Bebidas"}));
    chart->addAxis(axisX, Qt::AlignBottom);

    //INICIALIZA EJE Y CON SUS RESPECTIVOS CAMPOS
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setRange(0,3000);
    axisY->setTickCount(8);
    axisY->setLabelFormat("%.0f");

    //RECORRE EL VECTOR QBARSERIES PARA VINCULARLO CON EL EJE Y
    for(int itrSerie=0;itrSerie<series->count();itrSerie++)
        series->at(itrSerie)->attachAxis(axisY);

    //PREPARA CONFIGURACION DE VISUALIZACION DEL QCHART
    //chart->setTitle("Ventas del dia "+f + " .");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    //INICIALIZA LA VISTA DEL QCHARTVIEW COLOCANDO EL QCHART COMO PADRE
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //SE INSERTA EL QCHARTVIEW AL LAYOUT
    ui->grafica->addWidget(chartView);


    /*....................................................*/


    /*                  GRAFICA 3                   */
    series3 = new QVector<QBarSeries*>({0,0,0,0,0,0,0,0,0});

    //RECORRE EL VECTOR PARA INICIALIZAR CADA QBARSERIES
    for(int itrSerie=0;itrSerie<series3->count();itrSerie++)
        series3->operator[](itrSerie) = new QBarSeries();

    //INICIALIZA LOS QBARSET
    QBarSet *set1_3 = new QBarSet("Mesa 1");
    QBarSet *set2_3 = new QBarSet("Mesa 2");
    QBarSet *set3_3 = new QBarSet("Mesa 3");
    QBarSet *set4_3 = new QBarSet("Mesa 4");
    QBarSet *set5_3 = new QBarSet("Mesa 5");
    QBarSet *set6_3 = new QBarSet("Mesa 6");
    QBarSet *set7_3 = new QBarSet("Mesa 7");
    QBarSet *set8_3 = new QBarSet("Mesa 8");
    QBarSet *set9_3 = new QBarSet("Mesa 9");

    //INSERTA VALORES DE CADA QBARSET
    *set1_3 << Ganancias_Mesa[0];
    *set2_3 << Ganancias_Mesa[1];
    *set3_3 << Ganancias_Mesa[2];
    *set4_3 << Ganancias_Mesa[3];
    *set5_3 << Ganancias_Mesa[4];
    *set6_3 << Ganancias_Mesa[5];
    *set7_3 << Ganancias_Mesa[6];
    *set8_3 << Ganancias_Mesa[7];
    *set9_3 << Ganancias_Mesa[8];

    //INSERTA LOS BARSET A LOS QBARSERIES
    series3->at(0)->append(set1_3);
    series3->at(1)->append(set2_3);
    series3->at(2)->append(set3_3);
    series3->at(3)->append(set4_3);
    series3->at(4)->append(set5_3);
    series3->at(5)->append(set6_3);
    series3->at(6)->append(set7_3);
    series3->at(7)->append(set8_3);
    series3->at(8)->append(set9_3);

    //INICIALIZA EL QCHART
    QChart *chart3 = new QChart();

    //INSERTA CADA QBARSERIES AL QCHART
    for(int itrSerie=0;itrSerie<series3->count();itrSerie++)
        chart3->addSeries(series3->at(itrSerie));


    //INICIALIZA EJE X CON SUS RESPECTIVOS CAMPOS
    QBarCategoryAxis *axisX3 = new QBarCategoryAxis();
    axisX3->append(QStringList({"MESA 1","MESA 2","MESA 3","MESA 4","MESA 5","MESA 6","MESA 7","MESA 8","MESA 9"}));
    chart3->addAxis(axisX3, Qt::AlignBottom);

    //INICIALIZA EJE Y CON SUS RESPECTIVOS CAMPOS
    QValueAxis *axisY3 = new QValueAxis();
    chart3->addAxis(axisY3, Qt::AlignLeft);
    axisY3->setRange(0,3000);
    axisY3->setTickCount(8);
    axisY3->setLabelFormat("%.0f");

    //RECORRE EL VECTOR QBARSERIES PARA VINCULARLO CON EL EJE Y
    for(int itrSerie=0;itrSerie<series3->count();itrSerie++)
        series3->at(itrSerie)->attachAxis(axisY3);

    //PREPARA CONFIGURACION DE VISUALIZACION DEL QCHART
    //chart3->setTitle("Ventas del dia "+f + " por mesa.");
    chart3->setAnimationOptions(QChart::SeriesAnimations);
    chart3->legend()->setVisible(false);
    chart3->legend()->setAlignment(Qt::AlignBottom);

    //INICIALIZA LA VISTA DEL QCHARTVIEW COLOCANDO EL QCHART COMO PADRE
    chartView3 = new QChartView(chart3);
    chartView3->setRenderHint(QPainter::Antialiasing);

    //SE INSERTA EL QCHARTVIEW AL LAYOUT
    ui->grafica3->addWidget(chartView3);


    /*                   FIN                        */


    /*                    MESEROS VENTAS            */

    VentasMesero(f);


    /*                      FIN MESEROS VENTAS      */

    ui->Menu_gerente->setCurrentIndex(4);
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

//Codigo para el inventario------------------------------------------------------------------------------

void gerente_menu::on_inventario_btn_clicked()
{
    ui->Menu_gerente->setCurrentIndex(5);
    ActualizarTablaStock();
    ui->editarProducto_groupBox->hide();

    //AQUI DEBO ITERAR Y COLOREAR LAS FILAS EN LAS QUE SU CANTIDAD ES CERO

}

void gerente_menu::ActualizarTablaStock()
{

    ui->stock_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->stock_tableView->clearSpans();


    static QSqlQueryModel *mode= new QSqlQueryModel(ui->stock_tableView);

    mode->setQuery("select idProducto, NombreProducto, Cantidad from stock");
    mode->setHeaderData(0,Qt::Horizontal,tr("ID"));
    mode->setHeaderData(1, Qt::Horizontal, tr("PRODUCTO"));
    mode->setHeaderData(2, Qt::Horizontal, tr("CANTIDAD"));

    Productos = new QSortFilterProxyModel();
    Productos->setSourceModel(mode);
    Productos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    Productos->setFilterKeyColumn(1);

    ui->stock_tableView->setModel(Productos);
    ui->stock_tableView->horizontalHeader()->setStretchLastSection(true);

}

void gerente_menu::AgregarNuevoProductoStock()
{
    QString NombreProducto = ui->NombreNProducto_ineEdit->text();
    QString CantidadProducto = ui->CantidadNProducto_lineEdit_2->text();

    QSqlQuery NuevoProducto;

    if(NombreProducto != "" && CantidadProducto.toInt() != 0 ){

        if(NuevoProducto.prepare("INSERT INTO stock (NombreProducto, Cantidad) VALUES('"+NombreProducto+"', '"+CantidadProducto+"') ;")){

            NuevoProducto.exec();
        }

    }else{

        QMessageBox messageBox2(QMessageBox::Information,
                                tr("Advertencia"),
                                tr("Por favor llene todos los campos"),
                                QMessageBox::Yes,
                                this);
        messageBox2.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox2.exec();
    }
}

void gerente_menu::on_GuargarNproductopushButton_clicked()
{
    AgregarNuevoProductoStock();
    ActualizarTablaStock();
    ui->NombreNProducto_ineEdit->clear();
    ui->CantidadNProducto_lineEdit_2->clear();
}

void gerente_menu::on_EditarProducto_pushButton_clicked()
{
    if(ui->stock_tableView->selectionModel()->hasSelection()){
        ui->agregarProducto_groupBox->hide();
        ui->editarProducto_groupBox->show();

    }else{
        QMessageBox messageBox2(QMessageBox::Warning,
                                tr("Advertencia"),
                                tr("Por favor seleccione un producto"),
                                QMessageBox::Yes,
                                this);
        messageBox2.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox2.exec();
        qDebug()<<"Por favor seleccione un producto a editar";
    }
}

void gerente_menu::on_AgregarProducto_pushButton_clicked()
{
    ui->editarProducto_groupBox->hide();
    ui->agregarProducto_groupBox->show();

}

void gerente_menu::on_GuargarEproductopushButton_clicked()
{
    QString NombreP = ui->NombreEProducto_ineEdit->text();
    QString CantidadP = ui->CantidadEProducto_lineEdit->text();

    QSqlQuery ActualizarProducto;
    if(ActualizarProducto.prepare("UPDATE stock set NombreProducto = '"+NombreP+"', Cantidad = '"+CantidadP+"' where idProducto = '"+QString::number(Id_Producto)+"' ; ")){

        ActualizarProducto.exec();
        ActualizarTablaStock();
        ui->NombreEProducto_ineEdit->clear();
        ui->CantidadEProducto_lineEdit->clear();
    }

}

void gerente_menu::on_stock_tableView_activated(const QModelIndex &index)
{
    QModelIndex realIndex = ui->stock_tableView->model()->index(index.row(),0);
    Id_Producto = ui->stock_tableView->model()->data(realIndex).toInt();
    QModelIndex realIndex1 = ui->stock_tableView->model()->index(index.row(),1);
    QString NombreProducto = ui->stock_tableView->model()->data(realIndex1).toString();
    QModelIndex realIndex2 = ui->stock_tableView->model()->index(index.row(),2);
    QString Cantidad = ui->stock_tableView->model()->data(realIndex2).toString();

    ui->NombreEProducto_ineEdit->setText(NombreProducto);
    ui->CantidadEProducto_lineEdit->setText(Cantidad);


}

void gerente_menu::on_buscar_lineEdit_textChanged(const QString &arg1)
{
    Productos->setFilterFixedString(arg1);
}

void gerente_menu::on_EliminarProducto_pushButton_clicked()
{
    QMessageBox confirmacion(this);
    confirmacion.addButton("Sí",QMessageBox::YesRole);
    confirmacion.addButton("No",QMessageBox::NoRole);
    confirmacion.setWindowTitle("Confirmación");
    confirmacion.setIcon(QMessageBox::Question);
    confirmacion.setText("¿Está seguro que desea eliminar este producto?");
    int res = confirmacion.exec();

    QSqlQuery EliminarProducto;
    if(res == 0){
        if(EliminarProducto.prepare("DELETE FROM stock where idProducto = '"+QString::number(Id_Producto)+"';")){
            EliminarProducto.exec();

            ActualizarTablaStock();
            ui->NombreEProducto_ineEdit->clear();
            ui->CantidadEProducto_lineEdit->clear();
        }
    }

}

void gerente_menu::on_productosAgotados_pushButton_clicked()
{
    ui->stock_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->stock_tableView->clearSpans();


    static QSqlQueryModel *mode= new QSqlQueryModel(ui->stock_tableView);

    mode->setQuery("select idProducto, NombreProducto, Cantidad from stock where Cantidad = 0");
    mode->setHeaderData(0,Qt::Horizontal,tr("ID"));
    mode->setHeaderData(1, Qt::Horizontal, tr("PRODUCTO"));
    mode->setHeaderData(2, Qt::Horizontal, tr("CANTIDAD"));

    Productos = new QSortFilterProxyModel();
    Productos->setSourceModel(mode);
    Productos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    Productos->setFilterKeyColumn(1);

    ui->stock_tableView->setModel(Productos);
    ui->stock_tableView->horizontalHeader()->setStretchLastSection(true);
}

void gerente_menu::on_todosprodutos_pushButton_clicked()
{
    ActualizarTablaStock();
}

/**********codigo para ver ventas*/

void gerente_menu::on_ventas_btn_clicked()
{
    ui->Menu_gerente->setCurrentIndex(4);
}

void gerente_menu::ActualizarVectorCategorias(QString fecha)
{
    QSqlQuery *Ganancias = new QSqlQuery;
    QSqlQuery *Mesa = new QSqlQuery;
    QSqlQuery caja_ayer;
    totalVentas = 0.0;
    for(int i = 0; i < 7; i++){
        Ganancias_Categorias[i] = 0.0;
    }
    for(int i = 0; i < 9; i++){
        Ganancias_Mesa[i] = 0.0;
    }

    id_cuentas->clear();
    if(caja_ayer.prepare("SELECT total, tipoPago, estado,id_cuenta FROM cuenta WHERE fecha = '"+ fecha +"';"))
    {
        caja_ayer.exec(); // LLenado de campos de edicion
        while(caja_ayer.next()){

            if(caja_ayer.value(2).toString() == "pagada"){
                id_cuentas->append(caja_ayer.value(3).toInt());
                totalVentas += caja_ayer.value(0).toDouble();
            }
        }


    }
    else{
        qDebug() << caja_ayer.lastError();
    }

    for(int i = 0; i < id_cuentas->size(); i++){


        if(Ganancias->prepare("SELECT p.id_Categoria,SUM(o.precio*o.cantidadPlatillo) as Genero from platillos as p INNER JOIN orden as o ON p.idPlatillos = o.id_platillo "
                              "WHERE id_cuenta = "+QString::number(id_cuentas->value(i)) +" group by(p.id_categoria);")){
            Ganancias->exec();
            while(Ganancias->next()){
                Ganancias_Categorias[Ganancias->value(0).toInt() - 1] += Ganancias->value(1).toDouble();
            }
        }else{
            qDebug() << Ganancias->lastError();
        }

    }

    /*          Analisis Grafica 3     */

    if(Mesa->prepare("SELECT SUM(total) as Total, idmesa as Mesa from cuenta WHERE estado = 'pagada' AND fecha = '"+ fecha +"' group by(idmesa);")){
        Mesa->exec();
        while(Mesa->next()){
            Ganancias_Mesa[Mesa->value(1).toInt() - 1] += Mesa->value(0).toDouble();
        }
    }else{
        qDebug() << Mesa->lastError();
    }


    for(int i = 0; i < 9; i++){
        if(i < 7)
            qDebug() << "Categoria : "<< i << " " << Ganancias_Categorias[i];
        qDebug() << "Mesa : " << i << " " << Ganancias_Mesa[i];
    }



    ui->vendido->setText( " Total vendido : $" + QString::number(totalVentas) + ".00");






}

void gerente_menu::on_calendar_principal_selectionChanged()
{
    QDate fecha = ui->calendar_principal->selectedDate();
    QString f = fecha.toString("yyyyMMdd");
    qDebug() << "Fecha " << f;
    ActualizarVectorCategorias(f);
    VentasMesero(f);

    /*         Limiamos       */

    for(int itrSerie=0;itrSerie<series->count();itrSerie++)
        series->operator[](itrSerie)->clear();

    //INICIALIZA EL NUEVO QBARSET PARA EL NUEVO DATO
    //INICIALIZA LOS QBARSET
    QBarSet *set1 = new QBarSet("Entradas");
    QBarSet *set2 = new QBarSet("Sopas");
    QBarSet *set3 = new QBarSet("Ensaladas");
    QBarSet *set4 = new QBarSet("Carnes");
    QBarSet *set5 = new QBarSet("Especialidades");
    QBarSet *set6 = new QBarSet("Postres");
    QBarSet *set7 = new QBarSet("Bebidas");

    //INSERTA VALORES DE CADA QBARSET
    *set1 << Ganancias_Categorias[0];
    *set2 << Ganancias_Categorias[1];
    *set3 << Ganancias_Categorias[2];
    *set4 << Ganancias_Categorias[3];
    *set5 << Ganancias_Categorias[4];
    *set6 << Ganancias_Categorias[5];
    *set7 << Ganancias_Categorias[6];

    //SE INSERTA EL NUEVO BARSET AL BARSERIES SELECCIONADO
    series->operator[](0)->append(set1);
    series->operator[](1)->append(set2);
    series->operator[](2)->append(set3);
    series->operator[](3)->append(set4);
    series->operator[](4)->append(set5);
    series->operator[](5)->append(set6);
    series->operator[](6)->append(set7);

    for(int itrSerie=0;itrSerie<series3->count();itrSerie++)
        series3->operator[](itrSerie)->clear();
    //INICIALIZA LOS QBARSET
    QBarSet *set1_3 = new QBarSet("Mesa 1");
    QBarSet *set2_3 = new QBarSet("Mesa 2");
    QBarSet *set3_3 = new QBarSet("Mesa 3");
    QBarSet *set4_3 = new QBarSet("Mesa 4");
    QBarSet *set5_3 = new QBarSet("Mesa 5");
    QBarSet *set6_3 = new QBarSet("Mesa 6");
    QBarSet *set7_3 = new QBarSet("Mesa 7");
    QBarSet *set8_3 = new QBarSet("Mesa 8");
    QBarSet *set9_3 = new QBarSet("Mesa 9");

    //INSERTA VALORES DE CADA QBARSET
    *set1_3 << Ganancias_Mesa[0];
    *set2_3 << Ganancias_Mesa[1];
    *set3_3 << Ganancias_Mesa[2];
    *set4_3 << Ganancias_Mesa[3];
    *set5_3 << Ganancias_Mesa[4];
    *set6_3 << Ganancias_Mesa[5];
    *set7_3 << Ganancias_Mesa[6];
    *set8_3 << Ganancias_Mesa[7];
    *set9_3 << Ganancias_Mesa[8];

    //INSERTA LOS BARSET A LOS QBARSERIES
    series3->at(0)->append(set1_3);
    series3->at(1)->append(set2_3);
    series3->at(2)->append(set3_3);
    series3->at(3)->append(set4_3);
    series3->at(4)->append(set5_3);
    series3->at(5)->append(set6_3);
    series3->at(6)->append(set7_3);
    series3->at(7)->append(set8_3);
    series3->at(8)->append(set9_3);


}

void gerente_menu::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void gerente_menu::VentasMesero(QString fecha)
{
    ui->meserosTab->clear();
    ui->meserosTab->setRowCount(0);
    QStringList titulos;

    titulos<< "Identidicador" << "Nombre Completo" << "Total Vendido";
    ui->meserosTab->setColumnCount(3);
    ui->meserosTab->setHorizontalHeaderLabels(titulos);
    ui->meserosTab->resizeColumnToContents(1);
    QSqlQuery *registros = new QSqlQuery();
    if(registros->exec("SELECT e.NombreEmpleado, e.ApellidoPaterno,m.id_mesero,SUM(c.total) as Total FROM empleado as e  INNER JOIN mesero as m ON e.idEmpleado = m.id_empleado INNER JOIN cuenta as c ON m.id_mesero = c.idmesero WHERE c.estado = 'pagada' AND fecha = '"+ fecha+ "'  group by(c.idmesero);")){

        int fila;
        while(registros->next()){

            //reservaciones[fila-1] = reservaciones->value(5).toInt();
            ui->meserosTab->insertRow(ui->meserosTab->rowCount());
            fila = ui->meserosTab->rowCount() - 1;
            ui->meserosTab->setItem(fila,0,new QTableWidgetItem(registros->value(2).toString()));
            ui->meserosTab->setItem(fila,1,new QTableWidgetItem(registros->value(0).toString()
                                                                + " " + registros->value(1).toString()));
            ui->meserosTab->setItem(fila,2,new QTableWidgetItem("$ " + registros->value(3).toString() + ".00"));
            ui->diaMesero->setText(registros->value(0).toString() + " " + registros->value(1).toString());
        }
    }else{
        qDebug()<< "** Error "<< registros->lastError();
    }
    delete registros;
}
