#include "pago.h"
#include "ui_pago.h"

Pago::Pago(QWidget *parent, int id, double monto) :
    QDialog(parent),
    ui(new Ui::Pago)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    Id_cuenta = id;
    Monto = monto;
    ui->monto->setText(ui->monto->text() + QString::number(monto) + ".00");
    ui->monto_2->setText(ui->monto_2->text() + QString::number(monto)+ ".00");
    qDebug() << " Id_cuenta " << id;
    qDebug() << " Monto: " << monto;
}

Pago::~Pago()
{
    delete ui;
}

void Pago::on_efectivo_radioButton_clicked()
{
 ui->pagoWidget->setCurrentIndex(0);
}

void Pago::on_tarjeta_radioButton_clicked()
{
    ui->pagoWidget->setCurrentIndex(1);
}

void Pago::on_pagar_pushButton_clicked()
{
    QSqlQuery finaliza;
    if(ui->efectivo_radioButton->isChecked()){ // Efectivo
        if(ui->lineEdit->text() != "" && (ui->lineEdit->text().toDouble() >= Monto)){
            QMessageBox confirmacion(this);
            confirmacion.addButton("Sí",QMessageBox::YesRole);
            confirmacion.addButton("No",QMessageBox::NoRole);
            confirmacion.setWindowTitle("Confirmación");
            confirmacion.setIcon(QMessageBox::Question);
            confirmacion.setText("¿Esta seguro de la siguiente transaccion?\n"
                                 "Cantidad recibida: $ "+ui->lineEdit->text()+".00\n"
                                 "Cambio a regresar: $ "+QString::number(ui->lineEdit->text().toDouble() - Monto) + ".00");
            int ret = confirmacion.exec();
            if(ret == 0)
            {
                /*Solicitud de pago */
                if(Id_cuenta != 0){
                   if(finaliza.exec("UPDATE cuenta SET estado = 'pagada' WHERE id_cuenta = '"+QString::number(Id_cuenta)+"'")){


                       QMessageBox *mbox = new QMessageBox;
                           mbox->setWindowTitle(tr("Pago Exitoso"));
                           mbox->setText("Imprimiendo ticket");
                           mbox->setIconPixmap(QPixmap(":/iconos/cobrar.png").scaled(QSize(185,215),Qt::KeepAspectRatio));
                           mbox->show();
                          // Imprimir();
                           QTimer::singleShot(3000, mbox, SLOT(hide()));
                           emit pagada();
                           accept();
                           delete ui;
                   }
                   else
                       qDebug() << finaliza.lastError();


               }
            }
        }else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago incorrecto."),
                                   tr("Proporcione una cantidad recibida correcta"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }else{//Tarjeta
        if(ui->lineEdit_3->text() != ""){
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago Correcto Tarjeta."),
                                   tr("Su pago a sido registrado"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

        }else{
            QMessageBox messageBox(QMessageBox::Warning,
                                   tr("Pago incorrecto."),
                                   tr("Proporcione el nombre del propietario"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
            messageBox.exec();
        }
    }
}

void Pago::on_cancelar_pushButton_clicked()
{
    reject();
    delete ui;
}

void Pago::Imprimir()
{
    QString numero = "99";
    QString nom = ":/ticket"+numero+".pdf"; //Ruta General

    QDate aux = QDate::currentDate();
    QString aux_fecha = aux.toString("yyyy/MM/dd");
    QString ruta_img = ":/imagenes/LOGO.png";
        QString html = "<body style='background-color:#F4F4F4;'>"
                       "<img src='"+ ruta_img +"' width='50px' style='margin-left:200px;'>"
                       "<br><br><br>"
                       "<div><p style='margin-top:170px; margin-left:350px; font-size:20px;'><b>SABOREÑO 'EL MERO MERO SABOR NORTEÑO'</b></p></div>"
                       "<div style='margin-top:100px; margin-left:150px; border : 2px solid black;'><p>CALAJAN 2000 MNL 807670497</p><p>Prolongacion Juan de Borbo</p>"
                       "<p>Telefono: 023 4458923 </p><p>Fecha: "+ aux_fecha+"</p>";
/*

                        int Creditostotal = 0;
                        int aprovadas = 0;
                        int numCreditos = 0;
                        int semestre_final = alumno->GetSemestre();
                        QString html_medio =  "<div style='width:100%;'>";
                        for(int j = 1; j <= semestre_final + 1; j++){
                            aprovadas = 0;

                            numCreditos = 0;
                            html_medio += "<p style='margin-left:150px; margin-top: 50px;'> <b>Semestre Numero " + QString::number(j) + "</b></p>";
                            html_medio += "<table><thead>"
                                          "<tr><th>Campus</th><th>Nombre</th><th>Horas Cursadas</th><th>Creditos</th>"
                                          "</tr>"
                                          "</thead>";
                            for(int i = 0; i < KardexSemestre.size(); i++){
                                if(KardexSemestre[i].GetSemestre() == j){
                                    html_medio += "<tbody><tr>";
                                    html_medio += "<td>Ciudad Universitaria</td><td>"+ KardexSemestre[i].GetNombre() + "</td><td>"  + KardexSemestre[i].GetHoras() +"</td><td>" +QString::number(KardexSemestre[i].GetCreditos()) + "</td";
                                    html_medio += "</tr>";

                                    numCreditos +=KardexSemestre[i].GetCreditos();
                                    aprovadas++;
                                }

                            }

                            html_medio += "</tbody></table>";

                            html_medio += "<p><b>Aprobadas:</b>  " + QString::number(aprovadas) + "            <b>Creditos :</b> " + QString::number(numCreditos)+ "</p>";

                            Creditostotal += numCreditos;



                            html_medio +=  "</div>";
                        }

                        qDebug() << semestre_final;

                     QString html_fin = "<p><b> Creditos : " + QString::number(Creditostotal) + "</b></p>";

                     */
                QString html_fin /*+*/= "</body>"
                                          "</html>";

                      QString Kardex_html = html +/*html_medio +*/ html_fin;
        QTextDocument document;
        document.setHtml(Kardex_html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(nom);
        printer.setPageMargins(QMarginsF(45, 25, 45, 25));

        document.print(&printer);

//       ui->ruta_lineEdit->clear();




}
