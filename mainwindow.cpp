#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Iniciar Sesion");

    mDatabase = QSqlDatabase::addDatabase("QMYSQL");
    /* Suegerencia, cada quien agrege sus métodos de acceso y coméntelos en bloque, recuerden que es una versión de git

    mDatabase.setPort(3306);
    mDatabase.setHostName("localhost");
    mDatabase.setUserName("root");
    mDatabase.setPassword("root");
    mDatabase.setDatabaseName("restaurantdb");

    */
    mDatabase.setPort(3306);
    mDatabase.setHostName("localhost");
    mDatabase.setUserName("root");
    mDatabase.setPassword("root");
    mDatabase.setDatabaseName("restaurantdb");
    if(mDatabase.open()){
        qDebug() << "Base abierta";
    }
    else{
        qDebug() << "Error de base";
    }

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
        if(inicia->exec("select * from empleado where perfil = 'gerente' and NIP = '"+ui->NipGerente_lineEdit->text()+"'")){
            while(inicia->next()){
                aux = inicia->value(6).toString();
            }
            if(aux == ui->NipGerente_lineEdit->text()){
                gerente_menu *ventana = new gerente_menu(mDatabase,this);
                this->close();
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
    qDebug() << aux;
    delete inicia;
}
//hace lo mismo que arriba pero permite dar enter en después de escribir el nip
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
                this->close();
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
    delete inicia;
}
