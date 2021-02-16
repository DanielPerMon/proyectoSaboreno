#ifndef MESERO_MENU_H
#define MESERO_MENU_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>

namespace Ui {
class mesero_menu;
}

class mesero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit mesero_menu(QSqlDatabase, QString, QWidget *parent = nullptr);
    ~mesero_menu();

private slots:
    void on_salir_clicked();

    void on_verMesas_pushButton_clicked();

    void on_mesa1_clicked();

    void on_mesa2_clicked();

    void on_mesa3_clicked();

    void on_mesa4_clicked();

    void on_mesa5_clicked();

    void on_mesa6_clicked();

    void on_mesa7_clicked();

    void on_mesa8_clicked();

    void on_mesa9_clicked();

    void on_menu_Platillos_tabBarClicked(int index);

    void handleItem();

    void on_agregarord_pushButton_clicked();

    void on_btnmenos_clicked();

    void on_btnmas_clicked();

    void on_orden_tabBarClicked(int index);

    void on_enviarord_pushButton_clicked();

    void borrarItem();

    void on_pagarord_pushButton_clicked();

private:
    Ui::mesero_menu *ui;
    QSqlDatabase baseMesero;

    void cargarMesas();
    void agregarProducto(QString);
    void cargarOrden(QString);
    QList<QPushButton *> botones;
    QList<QString> listaProductos;
    QList<int> aux;
    QString temporal;
    int numeroCuenta;
    int idMesero;
    int filaactual;
    int filafinal;
    QString mesa;
    int platillosNuevos;
};

#endif // MESERO_MENU_H
