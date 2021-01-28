#ifndef GERENTE_MENU_H
#define GERENTE_MENU_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>

namespace Ui {
class gerente_menu;
}

class gerente_menu : public QDialog
{
    Q_OBJECT

public:
    explicit gerente_menu(QSqlDatabase ,QWidget *parent = nullptr);
    ~gerente_menu();

private slots:
    void on_nuevoUsuario_pushButton_clicked();

    void on_usuarios_pushButton_clicked();

    void on_acept_pushButton_clicked();

    void on_cancel_pushButton_clicked();

    void on_Gsalir_pushButton_clicked();

private:
    Ui::gerente_menu *ui;
    QSqlDatabase baseGerente;
    QWidget *principal;
};

#endif // GERENTE_MENU_H
