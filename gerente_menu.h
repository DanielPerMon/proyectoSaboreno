#ifndef GERENTE_MENU_H
#define GERENTE_MENU_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QTextEdit>
#include <QStyle>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QVariant>
#include <QList>
#include <QPixmap>

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

    void on_editar_tableWidget_cellClicked(int row);

    void QueryEditarEmpleado(int); // Se utiliza para consultar la info de los empleados

    void on_edit_pushButton_clicked();

    void EditarEmpleado(QString,QString,int);

    void ActualizarTablaEmpleados();

    void on_eliminarU_pushButton_clicked();

    void on_menu_pushButton_clicked();

    void on_categorias_tableWidget_cellClicked(int row);

    void on_categorias_tableWidget_cellDoubleClicked(int row);

    void on_volver_categoria_pushButton_clicked();

    void on_Guardarplato_pushButton_clicked();

    void on_CancelarPlatopushButton_clicked();

    void on_Nuevo_platillo_pushButton_clicked();

    void on_nuevafoto_pushButton_clicked();

    void ActualizarTablaPlatillos(int);

    void on_platillos_tableWidget_cellClicked(int row);

    void QueryEditarPlatillo(int);//Se utiliza para colocar la info en

    void on_eliminaplatillo_pushButton_clicked();

    void QueryCuentaPLatillosCategoria(int);

    void on_editarPlato_push_clicked();

    void on_inventario_btn_clicked();

    void on_ventas_btn_clicked();

private:
    Ui::gerente_menu *ui;
    QSqlDatabase baseGerente;
    QWidget *principal;
    bool Edito;//Se utiliza para saber si se a editado el empleado
    int Id_Modificar;//Se utiliza para saber que empleado modificare
    int Id_Categoria;
    int Id_Platillo;
    QString archivo_foto;
};

#endif // GERENTE_MENU_H
