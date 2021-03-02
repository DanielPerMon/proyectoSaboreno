#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Iniciar Sesion");
    user = "";

    mDatabase = QSqlDatabase::addDatabase("QMYSQL");
    /* Suegerencia, cada quien agrege sus métodos de acceso y coméntelos en bloque, recuerden que es una versión de git
    //Acceso Marco Antonio Martinez Cruz
    mDatabase.setPort(3307);
    mDatabase.setHostName("localhost");
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.setDatabaseName("restaurantdb");*/


     //Acceso Daniel Perez Montiel
    mDatabase.setPort(3306);
    mDatabase.setHostName("localhost");
    mDatabase.setUserName("root");
    mDatabase.setPassword("root");
    mDatabase.setDatabaseName("restaurantdb");

    //Acceso Itzel Lopez
    /*
   mDatabase.setPort(3306);
   mDatabase.setHostName("localhost");
   mDatabase.setUserName("root");
   mDatabase.setPassword("");
   mDatabase.setDatabaseName("restaurantdb");
   */



    if(mDatabase.open()){
        qDebug() << "Base abierta";
    }
    else{
        qDebug() << "Error de base" << mDatabase.lastError();
    }

    ui->inicio->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_i_gerente_pushButton_clicked()
{
    ui->inicio->setCurrentIndex(3);
}

void MainWindow::on_i_anfitrion_pushButton_clicked()
{
    ui->inicio->setCurrentIndex(2);
}

void MainWindow::on_i_cajero_pushButton_clicked()
{
    ui->inicio->setCurrentIndex(1);
}

void MainWindow::on_i_mesero_pushButton_clicked()
{
    QSqlQuery *meseros = new QSqlQuery();
    int conta=0,x=30,y=70;

    meseros->exec("SELECT NombreEmpleado FROM empleado WHERE Perfil = 'Mesero'");
    while(meseros->next()){
        QPushButton *boton =  new QPushButton(ui->m_der);
        if(conta%3==0 && conta!=0){
            x=30;
            y+=135;
        }
        boton->setStyleSheet("* {border-image: none; background-color: rgba(255, 255, 255,200); border-radius: 9px; font-family: Calisto MT;"
                             "font-style: normal;font-size: 15pt;font-weight: bold;}"
                             "*:hover{border: 2px solid black}");
        boton->setText(meseros->value(0).toString());
        boton->setObjectName(meseros->value(0).toString());
        boton->resize(120,100);
        boton->move(x,y);
        boton->show();
        x+=20 + boton->width();
        conta++;
    }

    botones = ui->m_der->findChildren<QPushButton *>();
    for(int i=0;i<botones.size();i++){
        connect(botones[i], SIGNAL (clicked()),this, SLOT (handleMesero()));
    }

    ui->inicio->setCurrentIndex(5);
}

void MainWindow::on_i_cocinero_pushButton_clicked()
{
    ui->inicio->setCurrentIndex(4);
}

void MainWindow::on_gSalir_pushButton_clicked()
{
    ui->inicio->setCurrentIndex(0);
}

//Ingresamos como gerente, por practicidad, solo hay un gerente
void MainWindow::on_gIngresar_pushButton_clicked()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->NipGerente_lineEdit->text() != ""){
        if(inicia->exec("select * from empleado where perfil = 'Gerente' and NIP = '"+ui->NipGerente_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipGerente_lineEdit->text()){
                gerente_menu *ventana = new gerente_menu(mDatabase,this);
                ventana->show();
            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un nip válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->NipGerente_lineEdit->clear();
    delete inicia;
}
//hace lo mismo que arriba pero permite dar enter después de escribir el nip
void MainWindow::on_NipGerente_lineEdit_returnPressed()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->NipGerente_lineEdit->text() != ""){
        if(inicia->exec("select * from empleado where perfil = 'gerente' and NIP = '"+ui->NipGerente_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipGerente_lineEdit->text()){
                gerente_menu *ventana = new gerente_menu(mDatabase,this);
                ventana->show();

            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();

            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un nip válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->NipGerente_lineEdit->clear();
    delete inicia;
}

void MainWindow::on_gSalir_pushButton_2_clicked()
{
    ui->inicio->setCurrentIndex(0);
}

void MainWindow::handleMesero()
{
    QObject *senderObj = sender(); // This will give Sender object
    ui->label_18->setText(senderObj->objectName());
    user = senderObj->objectName();
}

void MainWindow::on_Mingresar_pushButton_clicked()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->mesero_nip_lineEdit->text() != ""){
        if(inicia->exec("SELECT * FROM empleado WHERE Perfil = 'Mesero' and NombreEmpleado = '"+user+"' and NIP = '"+ui->mesero_nip_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->mesero_nip_lineEdit->text()){
                mesero_menu *ventana = new mesero_menu(mDatabase,user, this);
                ventana->show();
            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un nip válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->mesero_nip_lineEdit->clear();
    delete inicia;
}

void MainWindow::on_cSalir_pushButton_clicked()
{
  ui->inicio->setCurrentIndex(0);
}

void MainWindow::on_IngreCocinero_pushButton_clicked()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->NipCocinero_lineEdit->text() != ""){
        if(inicia->exec("SELECT * FROM empleado WHERE Perfil = 'Cocinero' and NIP = '"+ui->NipCocinero_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipCocinero_lineEdit->text()){
                cocinero_menu *ventana = new cocinero_menu(mDatabase,user, this);
                ventana->show();
            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un nip válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->NipCocinero_lineEdit->clear();
    delete inicia;
}

//Inicio sesion cajero
void MainWindow::on_gIngresar_pushButton_5_clicked()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->NipCajero_lineEdit->text() != ""){
        if(inicia->exec("SELECT * FROM empleado WHERE Perfil = 'Cajero' and NIP = '"+ui->NipCajero_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipCajero_lineEdit->text()){
                cajero_menu *ventana = new cajero_menu(mDatabase,user, this);
                ventana->show();
            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un nip válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->NipCajero_lineEdit->clear();
    delete inicia;

}

void MainWindow::on_cSalir_clicked()
{
    ui->inicio->setCurrentIndex(0);
}


//INICIO SESIÓN ANFITRION
void MainWindow::on_btnIngresaH_clicked()
{
    QSqlQuery *inicia = new QSqlQuery();
    QString aux;
    if(ui->NipHost_lineEdit->text() != ""){
        if(inicia->exec("SELECT * FROM empleado WHERE Perfil = 'Anfitrion' and NIP = '"+ui->NipHost_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipHost_lineEdit->text()){
                anfitrion_menu *ventana = new anfitrion_menu(mDatabase, this);
                ventana->show();
            }
            else{
                QMessageBox messageBox(QMessageBox::Information,
                                       tr("Error de credenciales"),
                                       tr("El número de identificación no es el correcto"),
                                       QMessageBox::Yes,
                                       this);
                messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                messageBox.exec();
            }
        }
        else{
            qDebug() << inicia->lastError();
        }
    }
    else{
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Campos vacíos"),
                               tr("Por favor ingrese un NIP válido"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();

    }
    ui->NipHost_lineEdit->clear();
    delete inicia;

}

void MainWindow::on_H_Salir_clicked()
{
    ui->inicio->setCurrentIndex(0);
}
