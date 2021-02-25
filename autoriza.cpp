#include "autoriza.h"
#include "ui_autoriza.h"

autoriza::autoriza(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::autoriza)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    x = 0;
}

autoriza::~autoriza()
{
    delete ui;
}

void autoriza::on_btnAceptar_clicked()
{
    QSqlQuery empleado;

    if(ui->lnPs->text() == "" || ui->lnUs->text() == ""){
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Campos Vacíos"),
                               tr("LLene ambos campos para autorizar"),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.exec();
    }
    else
    {
        empleado.exec("SELECT idEmpleado, NIP from empleado WHERE Perfil = 'Gerente'");
        empleado.next();
        if(empleado.value(0).toString() == ui->lnUs->text() && empleado.value(1).toString() == ui->lnPs->text())
        {
            x = 1;
            this->close();
        }
        else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Credenciales Incorrectas"),
                                   tr("No coinciden las credenciales \nIntente de nuevo"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }
}

void autoriza::on_btnCancelar_clicked()
{
    this->close();
}

int autoriza::valor()
{
    return x;
}
