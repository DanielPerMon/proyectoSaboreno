#ifndef GERENTE_MENU_H
#define GERENTE_MENU_H

#include <QDialog>

namespace Ui {
class gerente_menu;
}

class gerente_menu : public QDialog
{
    Q_OBJECT

public:
    explicit gerente_menu(QWidget *parent = nullptr);
    ~gerente_menu();

private slots:
    void on_nuevoUsuario_pushButton_clicked();

    void on_usuarios_pushButton_clicked();

private:
    Ui::gerente_menu *ui;
};

#endif // GERENTE_MENU_H
