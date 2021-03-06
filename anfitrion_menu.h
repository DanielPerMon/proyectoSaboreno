#ifndef ANFITRION_MENU_H
#define ANFITRION_MENU_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>

namespace Ui {
class anfitrion_menu;
}

class anfitrion_menu : public QDialog
{
    Q_OBJECT

public:
    explicit anfitrion_menu(QSqlDatabase,QWidget *parent = nullptr);
    ~anfitrion_menu();

private slots:
    void on_btnSalirH_clicked();

    void on_btnReservacion_clicked();

    void on_btnReservaciones_clicked();

    void on_btnAgendarRes_clicked();

    void on_btnCancelReserva_clicked();

    void on_Reserva_tableWidget_cellClicked(int row, int column);

private:
    Ui::anfitrion_menu *ui;
    QSqlDatabase baseAnfitrion;
    void ActualizarTabla();
    QString nombre;
};

#endif // ANFITRION_MENU_H
