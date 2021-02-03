#include "mesero_menu.h"
#include "ui_mesero_menu.h"

mesero_menu::mesero_menu(QSqlDatabase base, QString usuario, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mesero_menu)
{
    ui->setupUi(this);
    baseMesero = base;
    setWindowTitle("Saboreño Mesero");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->lblMesero->setText(ui->lblMesero->text()+" "+usuario);
    ui->lblMesero->setWordWrap(true);
    cargarMesas();
}

mesero_menu::~mesero_menu()
{
    delete ui;
}

void mesero_menu::on_salir_clicked()
{
    this->close();
}

void mesero_menu::on_verMesas_pushButton_clicked()
{
    ui->Mesero_menu->setCurrentIndex(0);
}

void mesero_menu::cargarMesas()
{
    QSqlQuery *estado = new QSqlQuery();
    QList<QPushButton *> botones;
    botones = ui->mesas->findChildren<QPushButton *>();

    if(estado->exec("SELECT * FROM mesa")){

        while(estado->next()){
            for(int i=0;i<botones.size();i++){
                if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(3) == "libre")
                {
                    botones[i]->setStyleSheet("*{background-color: #0f790f; border-radius:15px;}");
                }

                else{
                    if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(3) == "ocupada"){
                        botones[i]->setStyleSheet("*{background-color: #d12d1d; border-radius:15px;}");
                    }
                    else{
                        if(estado->value(0).toString() == botones[i]->objectName().right(1) && estado->value(3) == "reservada"){
                            botones[i]->setStyleSheet("*{background-color: rgb(255, 178, 21); border-radius:15px;}");
                        }
                    }
                }
            }
        }
    }
    else{
        qDebug() << "Error en ver mesas  " << estado->lastError();
    }
}

