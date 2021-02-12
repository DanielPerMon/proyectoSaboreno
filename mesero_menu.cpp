#include "mesero_menu.h"
#include "ui_mesero_menu.h"

mesero_menu::mesero_menu(QSqlDatabase base, QString usuario, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mesero_menu)
{
    ui->setupUi(this);
    baseMesero = base;
    setWindowTitle("Saboreño Mesero");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->lblMesero->setText(ui->lblMesero->text()+" "+usuario);
    ui->lblMesero->setWordWrap(true);
    ui->Mesero_menu->setCurrentIndex(0);
    temporal = "";
    cargarMesas();

    QSqlQuery aux;
    aux.exec("SELECT id_mesero FROM mesero INNER JOIN empleado ON mesero.id_empleado = empleado.idEmpleado WHERE NombreEmpleado = '"+usuario+"'");
    aux.next();
    idMesero = aux.value(0).toInt();
}

mesero_menu::~mesero_menu()
{
    delete ui;
}

void mesero_menu::on_salir_clicked()
{
    this->close();
}

void mesero_menu::on_verMesas_pushButton_clicked()
{
    ui->Mesero_menu->setCurrentIndex(0);
}

void mesero_menu::cargarMesas()
{
    QSqlQuery *estado = new QSqlQuery();
    botones = ui->mesas->findChildren<QPushButton *>();

    if(estado->exec("SELECT * FROM mesa")){

        while(estado->next()){
            for(int i=0;i<botones.size();i++){
                if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(1) == "libre")
                {
                    botones[i]->setStyleSheet("*{background-color: #0f790f; border-radius:15px;}");
                }

                else{
                    if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(1) == "ocupada"){
                        botones[i]->setStyleSheet("*{background-color: #d12d1d; border-radius:15px;}");
                    }
                    else{
                        if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(1) == "reservada"){
                            botones[i]->setStyleSheet("*{background-color: rgb(255, 178, 21); border-radius:15px;}");
                        }
                    }
                }
            }
        }
    }
    else{
        qDebug() << "Error en ver mesas  " << estado->lastError();
    }
    botones.clear();
}

void mesero_menu::agregarProducto(QString numMesa)
{
    ui->label_23->setText("TOMAR ORDEN MESA "+numMesa);

    QSqlQuery *verEstado = new QSqlQuery();

    verEstado->exec("SELECT Estado FROM mesa WHERE idMesa = '"+numMesa+"'");
    verEstado->next();

    if(verEstado->value(0).toString() == "libre" || verEstado->value(0).toString() == "reservada"){
        qDebug() << "entra al if";
        QDate fecha = QDate::currentDate();
        QString FechaActual = fecha.toString("yyyy-MM-dd");
        QMessageBox confirmacion(this);
        confirmacion.addButton("Sí",QMessageBox::YesRole);
        confirmacion.addButton("No",QMessageBox::NoRole);
        confirmacion.setWindowTitle("Confirmación");
        confirmacion.setIcon(QMessageBox::Question);
        confirmacion.setText("Esta mesa no tiene una cuenta activa \n ¿Desea agregar una nueva cuenta?");
        int ret = confirmacion.exec();
        if(ret == 0)
        {
            QSqlQuery item;
            if( item.exec("INSERT INTO cuenta(fecha, estado, idmesa, idmesero) VALUES('"+FechaActual+"', 0, '"+numMesa+"', '"+QString::number(idMesero)+"')"))
            {
                QSqlQuery actualizaEstado;
                actualizaEstado.exec("UPDATE mesa SET estado = 'ocupada' WHERE idMesa = '"+numMesa+"'");
                cargarMesas();
            }
            else
            {
                qDebug()<<"Error de crear cuenta";
                qDebug()<<item.lastError();
            }
        }
        else
        {
            ui->Mesero_menu->setCurrentIndex(0);
        }
    }
    else{
        QSqlQuery vercuenta;

        vercuenta.exec("SELECT id_cuenta FROM cuenta WHERE idmesa = '"+numMesa+"' AND estado = '0' ");
        vercuenta.next();
        numeroCuenta = vercuenta.value(0).toInt();

        ui->tablaOrden->setRowCount(0);
        QStringList titulos;
        titulos<<"Platillo"<<"Cantidad"<<"";
        ui->tablaOrden->setColumnCount(3);
        ui->tablaOrden->setHorizontalHeaderLabels(titulos);

        vercuenta.exec("SELECT p.NombrePlatillo, o.cantidadPlatillo FROM platillos as p INNER JOIN orden as o ON p.idPlatillos = o.id_platillo "
                       " INNER JOIN cuenta as c ON o.id_cuenta = c.id_cuenta INNER JOIN mesa as m ON c.idmesa = m.idMesa WHERE c.estado = '0' "
                       "AND m.idMesa = '"+numMesa+"' ");

        int fila;
        while(vercuenta.next()){
            fila = ui->tablaOrden->rowCount();
            ui->tablaOrden->insertRow(ui->tablaOrden->rowCount());
            ui->tablaOrden->setItem(fila,0,new QTableWidgetItem(vercuenta.value(0).toString()));
            ui->tablaOrden->setItem(fila,1,new QTableWidgetItem(vercuenta.value(1).toString()));
        }
    }

    delete verEstado;
}

void mesero_menu::cargarMenu()
{
    QSqlQuery *items = new QSqlQuery();
    QSqlQuery *cat = new QSqlQuery();
    cat->exec("SELECT * FROM categoria");
    if(items->exec("SELECT NombrePlatillos, id_Categoria FROM platillos WHERE id_Categoria = '"+cat->value(0).toString()+"' ")){
        qDebug() << "Error en cargar menú " << items->lastError();
    }
    else
        qDebug() << "Error en cargar menú " << items->lastError();
}


void mesero_menu::on_mesa1_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("1");
}

void mesero_menu::on_mesa2_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("1");
}

void mesero_menu::on_mesa3_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("3");
}

void mesero_menu::on_mesa4_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("4");
}

void mesero_menu::on_mesa5_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("5");
}

void mesero_menu::on_mesa6_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("6");
}

void mesero_menu::on_mesa7_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("7");
}

void mesero_menu::on_mesa8_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("8");
}

void mesero_menu::on_mesa9_clicked()
{
    ui->Mesero_menu->setCurrentIndex(1);
    ui->tablaOrden->clear();
    agregarProducto("9");
}

void mesero_menu::on_menu_Platillos_tabBarClicked(int index)
{
    botones.clear();
    QSqlQuery *items = new QSqlQuery();
    int conta=0,x=0,y=0;

    switch (index) {
    case 0:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Entradas')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollEntradas);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 1:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Ensaladas')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollEnsaladas);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 2:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Sopas')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollSopas);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 3:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Carnes')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollCarnes);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 4:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Guarniciones')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollGuarniciones);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 5:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Especialidades')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollEspe);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 6:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Postres')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollPostres);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    case 7:
        if(items->exec("SELECT NombrePlatillo FROM platillos WHERE id_Categoria IN (SELECT id_categoria FROM categoria WHERE nombre = 'Bebidas')")){
            while(items->next()){
                QPushButton *boton =  new QPushButton(ui->scrollBebidas);
                if(conta%2==0 && conta!=0){
                    x=0;
                    y+=101;
                }
                boton->setStyleSheet("* {border-image: none; background-color: rgb(255, 178, 21); border: 2px solid; border-radius: 1px; font-family: Calisto MT;"
                                     "font-style: normal;font-size: 10pt;font-weight: bold;}"
                                     "*:hover{border: 3.5px solid black}");
                boton->setText(items->value(0).toString());
                boton->setObjectName(items->value(0).toString());
                boton->resize(247,100);
                boton->move(x,y);
                boton->show();
                x+= 1 + boton->width();
                conta++;
                connect(boton, SIGNAL (clicked()),this, SLOT (handleItem()));
            }
        }
        else{
            qDebug() << items->lastError();
        }
        break;
    default:
        break;
    }
    delete items;
}

void mesero_menu::handleItem()
{
    ui->orden->setCurrentIndex(0);
    QObject *senderObj = sender();
    QPixmap imagen;
    QSqlQuery datos;

    datos.exec("SELECT imagen FROM platillos WHERE NombrePlatillo = '"+senderObj->objectName()+"'");
    datos.next();

    imagen.loadFromData(datos.value(0).toByteArray());

    ui->fotoOrden->setPixmap(imagen);
    temporal = senderObj->objectName();
}

void mesero_menu::on_agregarord_pushButton_clicked()
{
    ui->tablaOrden->insertRow(ui->tablaOrden->rowCount());
    int fila = ui->tablaOrden->rowCount()-1;
    ui->tablaOrden->setItem(fila,0,new QTableWidgetItem(temporal));
    ui->tablaOrden->setItem(fila,1,new QTableWidgetItem(ui->cantidad_lineEdit->text()));

    QWidget* pWidget = new QWidget();
    QPushButton* btn_edit = new QPushButton();
    btn_edit->setText("Elim");
    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(btn_edit);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);
    ui->tablaOrden->setCellWidget(fila, 2, pWidget);

    btn_edit->setObjectName("btn"+QString::number(fila));
    btn_edit->setProperty("row",fila);

    connect(btn_edit, SIGNAL (clicked()),this, SLOT (borrarItem()));
}

void mesero_menu::on_btnmenos_clicked()
{
    if(ui->cantidad_lineEdit->text().toInt() > 1){
        ui->cantidad_lineEdit->setText(QString::number(ui->cantidad_lineEdit->text().toInt()-1));
    }
}

void mesero_menu::on_btnmas_clicked()
{
    ui->cantidad_lineEdit->setText(QString::number(ui->cantidad_lineEdit->text().toInt()+1));
}

void mesero_menu::on_orden_tabBarClicked(int index)
{
    if(index == 1){

    }
}

void mesero_menu::on_enviarord_pushButton_clicked()
{
    QMessageBox confirmacion(this);
    confirmacion.addButton("Sí",QMessageBox::YesRole);
    confirmacion.addButton("No",QMessageBox::NoRole);
    confirmacion.setWindowTitle("Confirmación");
    confirmacion.setIcon(QMessageBox::Question);
    confirmacion.setText("¿Desea enviar la orden a la cocina?");
    int ret = confirmacion.exec();
    if(ret == 0)
    {
        QSqlQuery orden;
        for(int i = 0; i < ui->tablaOrden->rowCount();i++){
            QSqlQuery aux;
            aux.exec("SELECT * FROM platillos WHERE NombrePlatillo = '"+ui->tablaOrden->item(i,0)->text()+"'");
            aux.next();
            orden.exec("INSERT INTO orden(id_cuenta,precio,cantidadPlatillo,id_platillo) VALUES('"+QString::number(numeroCuenta)+"',"
                        "'"+aux.value(3).toString()+"','"+ui->tablaOrden->item(i,1)->text()+"', '"+aux.value(0).toString()+"' )");
        }
    }
}

void mesero_menu::borrarItem()
{
    QObject *senderObj = sender();
    int row = ui->tablaOrden->currentRow();
    if (row == -1) {
      if (QWidget* focused = ui->tablaOrden->focusWidget()) {
        row = focused->property("row").toInt();
      }
    }

    ui->tablaOrden->removeRow(row);
}
