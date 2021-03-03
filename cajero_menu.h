#ifndef CAJERO_MENU_H
#define CAJERO_MENU_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include "pago.h"
#include "autoriza.h"
#include "cancelar.h"
#include <QList>
#include <QFileDialog>
#include <QTextStream>

namespace Ui {
class cajero_menu;
}

class cajero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit cajero_menu(QSqlDatabase, QString, QWidget *parent = nullptr);
    ~cajero_menu();

private slots:
    void on_saliC_pushButton_clicked();

    void on_cobrar_pushButton_clicked();

    void ActualizarTabla();

    void CuentaElegida(int);

    void CuentaElegidaDet(int);

    void on_Cuentas_tableWidget_cellClicked(int row, int column);

    void on_cobrar_pushButton_2_clicked();

    void on_btnHistorial_clicked();

    void on_Historial_tableWidget_cellClicked(int row, int column);

    void on_btnEliminaCuenta_clicked();

    void on_btnCancelarProducto_clicked();

    void on_btnImprimir_clicked();

    void on_imprimirHistorial_clicked();

    void on_corte_caja_clicked();

    void CorteCaja(QString);

    void on_btnCortedecaja_clicked();

    QString Porcentaje(double,double);

    void on_btndescarga_clicked();

private:
    Ui::cajero_menu *ui;
    QSqlDatabase baseCajero;
    void imprime(int);
    int FilaCuentas;
    int FilaDet;
    int mesa;
    double monto_enviar;
    QList<int> *id_cuentas;
    QString Usuario;
};

#endif // CAJERO_MENU_H
