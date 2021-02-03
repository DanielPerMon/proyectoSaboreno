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

private:
    Ui::mesero_menu *ui;
    QSqlDatabase baseMesero;

    void cargarMesas();
};

#endif // MESERO_MENU_H
