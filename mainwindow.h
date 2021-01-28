#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include "gerente_menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase mDatabase;

private slots:
    void on_i_gerente_pushButton_clicked();

    void on_i_anfitrion_pushButton_clicked();

    void on_i_cajero_pushButton_clicked();

    void on_i_mesero_pushButton_clicked();

    void on_i_cocinero_pushButton_clicked();

    void on_gSalir_pushButton_clicked();

    void on_gIngresar_pushButton_clicked();

    void on_NipGerente_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
