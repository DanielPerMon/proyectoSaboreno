#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
