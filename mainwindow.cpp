#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>
#include <iostream>
using namespace std;

//Main window call

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->centralWidget->setStyleSheet("background-image: url(/home/juan/lab_progra/prueba1/background.jpg)");
}

//Connection to database and table display

bool MainWindow::connect(){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery query;
    QString output;
    int i;

//Setting up the connection

    db.setHostName("localhost");
    db.setDatabaseName("proyecto_1");
    db.setUserName(ui->edit_username->text());
    db.setPassword(ui->edit_password->text());

    if(!db.open()){
        QMessageBox::critical(0,"Database error!",db.lastError().text());
        return false;
    }

    QMessageBox::information(this,"Welcome " + ui->edit_username->text() + "!","You are now logged in!");
    ui->edit_username->clear();
    ui->edit_password->clear();

//Filling "provincia", "nota", "año inicial, "año final" and "sigla" drop-down lists
/*
    ui->dropdown_provincia->clear();
    ui->dropdown_sigla->clear();
    ui->dropdown_nota_min->clear();
    ui->dropdown_nota_max->clear();
    ui->dropdown_ano_inicial->clear();
    ui->dropdown_ano_final->clear();
*/
    ui->dropdown_provincia->addItem("TODAS");
    query.exec("SELECT DISTINCT provincia FROM Ingresados ORDER BY provincia");
    while(query.next()){
        ui->dropdown_provincia->addItem(query.value(0).toString());
    }

    query.exec("SELECT DISTINCT sigla FROM Notas ORDER BY sigla");
    while(query.next()){
        ui->dropdown_sigla->addItem(query.value(0).toString());
    }

    for(i=0; i<=10; i++){
        ui->dropdown_nota_min->addItem(QString::number(i));
        ui->dropdown_nota_max->addItem(QString::number(i));
    }
    ui->dropdown_nota_max->setCurrentIndex(10);

    for(i=1973; i<=2013; i++){
        ui->dropdown_ano_inicial->addItem(QString::number(i));
        ui->dropdown_ano_final->addItem(QString::number(i));
    }
    ui->dropdown_ano_final->setCurrentIndex(40);

//Showing tables on the database

    query.exec("SHOW TABLES");
    output += "Tablas en la base de datos:\n\n";
    while(query.next()){
        output += query.value(0).toString()+"\n";
    }

    ui->text_output->setText(output);
    ui->button_connect->setEnabled(false);

    return true;
}

//Displaying names and quantity of graduates according to location

bool MainWindow::graduados_region(){

    QSqlQuery query;
    QString output;
    int graduados_bach = 0,graduados_lic=0;

    if(ui->dropdown_provincia->currentText() == "TODAS"){

        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND descripciontitulo = 'BACHILLER EN INGENIERIA ELECTRICA'");
        if(!query.exec()){
            QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
            return false;
        }

        output += "Graduados de bachillerato: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_bach += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_bach));
        output += "\n\n";

        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND descripciontitulo = 'LICENCIADO (A) EN INGENIERIA ELECTRICA'");
        if(!query.exec()){
            QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
            return false;
        }

        output += "Graduados de licenciatura: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_lic += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_lic));

        ui->text_output->setText(output);
    }

    else if(ui->dropdown_provincia->currentText() != "TODAS" && ui->dropdown_canton->currentText() == "TODOS"){
        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Graduados.descripciontitulo = 'BACHILLER EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());

        if(!query.exec()){
            QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
            return false;
        }

        output += "Graduados de bachillerato: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_bach += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_bach));
        output += "\n\n";

        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Graduados.descripciontitulo = 'LICENCIADO (A) EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.exec();

        output += "Graduados de licenciatura: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_lic += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_lic));

        ui->text_output->setText(output);
    }

    else if(ui->dropdown_provincia->currentText() != "TODAS" && ui->dropdown_canton->currentText() != "TODOS" && ui->dropdown_distrito->currentText() == "TODOS"){
        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Ingresados.canton = :canton AND Graduados.descripciontitulo = 'BACHILLER EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.bindValue(":canton",ui->dropdown_canton->currentText());

        if(!query.exec()){
            QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
            return false;
        }

        output += "Graduados de bachillerato: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_bach += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_bach));
        output += "\n\n";

        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Ingresados.canton = :canton AND Graduados.descripciontitulo = 'LICENCIADO (A) EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.bindValue(":canton",ui->dropdown_canton->currentText());
        query.exec();

        output += "Graduados de licenciatura: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_lic += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_lic));

        ui->text_output->setText(output);
    }

    else{
        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Ingresados.canton = :canton AND Ingresados.distrito = :distrito AND Graduados.descripciontitulo = 'BACHILLER EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.bindValue(":canton",ui->dropdown_canton->currentText());
        query.bindValue(":distrito",ui->dropdown_distrito->currentText());

        if(!query.exec()){
            QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
            return false;
        }

        output += "Graduados de bachillerato: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_bach += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_bach));
        output += "\n\n";

        query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.provincia = :provincia AND Ingresados.canton = :canton AND Ingresados.distrito = :distrito AND Graduados.descripciontitulo = 'LICENCIADO (A) EN INGENIERIA ELECTRICA'");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.bindValue(":canton",ui->dropdown_canton->currentText());
        query.bindValue(":distrito",ui->dropdown_distrito->currentText());
        query.exec();

        output += "Graduados de licenciatura: \n";

        while(query.next()){
            output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
            graduados_lic += 1;
        }

        output += "\n Total: ";
        output.append(QString("%1").arg(graduados_lic));

        ui->text_output->setText(output);
    }

    return true;
}

//Displaying names and scores given "curso", "año inicial", "año final" and "notas" in certain range

bool MainWindow::nota_curso_limitada(){

    QSqlQuery query;
    QString output;

    query.prepare("SELECT nombre, apellido1, apellido2, notaordinaria FROM Notas WHERE notaordinaria >= :nota_min AND notaordinaria <= :nota_max AND sigla = :curso AND anio >= :ano_inicial AND anio <= :ano_final");
    query.bindValue(":nota_min",ui->dropdown_nota_min->currentText().toInt());
    query.bindValue(":nota_max",ui->dropdown_nota_max->currentText().toInt());
    query.bindValue(":curso",ui->dropdown_sigla->currentText());
    query.bindValue(":ano_inicial",ui->dropdown_ano_inicial->currentText().toInt());
    query.bindValue(":ano_final",ui->dropdown_ano_final->currentText().toInt());

    if(!query.exec()){
        QMessageBox::critical(0,"Database error!",trUtf8("Debe iniciar sesión"));
        return false;
    }

    if(ui->dropdown_nota_min->currentText().toInt() > ui->dropdown_nota_max->currentText().toInt() || ui->dropdown_ano_inicial->currentText().toInt() > ui->dropdown_ano_final->currentText().toInt())
        QMessageBox::critical(0,"Eighth layer error",trUtf8("¡No sea imbécil!"));

    while(query.next()){
        output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + ", Nota: " + query.value(3).toString() + "\n";
    }

    ui->text_output->setText(output);

    return true;
}

//Filling "canton" drop-down list

bool MainWindow::fill_list_canton(){

    QSqlQuery query;

    ui->dropdown_canton->clear();

    if(ui->dropdown_provincia->currentText()!="TODAS"){
        query.prepare("SELECT DISTINCT canton FROM Ingresados WHERE provincia = :provincia ORDER BY canton");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.exec();
        ui->dropdown_canton->addItem("TODOS");

        while(query.next()){
            ui->dropdown_canton->addItem(query.value(0).toString());
        }
    }

    return true;
}

//Filling "distrito" drop-down list

bool MainWindow::fill_list_distrito(){

    QSqlQuery query;

    ui->dropdown_distrito->clear();

    if(ui->dropdown_canton->currentText()!="TODOS"){
        query.prepare("SELECT DISTINCT distrito FROM Ingresados WHERE provincia = :provincia AND canton = :canton ORDER BY distrito");
        query.bindValue(":provincia",ui->dropdown_provincia->currentText());
        query.bindValue(":canton",ui->dropdown_canton->currentText());
        query.exec();
        ui->dropdown_distrito->addItem("TODOS");

        while(query.next()){
            ui->dropdown_distrito->addItem(query.value(0).toString());
        }
    }

    return true;
}

bool MainWindow::promedio(){

    QSqlQuery query;
    QString output;
    double counter = 0.0, prom = 0.0;

    query.prepare("SELECT sigla, notaordinaria FROM Notas WHERE sigla = :sigla");
    query.bindValue(":sigla",ui->dropdown_sigla->currentText());
    query.exec();

   while(query.next()){

        if (query.value(1)=="RJ" || query.value(1)=="RM"){
            ;
        }

        else if(query.value(1)=="RI"){
            counter+=1.0;
            prom+=4.0;
        }

        else if(query.value(1)=="PE"){
            counter+=1.0;
            prom+=5.5;
        }

        else{
            counter+=1.0;
            prom+=query.value(1).toDouble();
        }

    }
    if(counter==0.0){
       ui->text_output->setText("No hay datos a mostrar");
    }

    else{
        output+="Promedio del Curso " + ui->dropdown_sigla->currentText() + ": " + QString::number(prom/counter);
        ui->text_output->setText(output);
    }
    return true;
}

bool MainWindow::promedio_estudiante(){
    QSqlQuery query;
    QString output;
    double counter = 0.0, prom = 0.0;

    query.prepare("SELECT nombre, apellido1, apellido2,notaordinaria FROM Notas WHERE carne = :carne AND anio >= :ano_inicial AND anio <= :ano_final");
    query.bindValue(":carne",ui->edit_carne->text());
    query.bindValue(":ano_inicial",ui->dropdown_ano_inicial->currentText().toInt());
    query.bindValue(":ano_final",ui->dropdown_ano_final->currentText().toInt());
    query.exec();

    while(query.next()){

        if (query.value(3)=="RJ" || query.value(3)=="RM"){
            ;
        }

        else if(query.value(3)=="RI"){
            counter+=1.0;
            prom+=4.0;
        }

        else if(query.value(3)=="PE"){
            counter+=1.0;
            prom+=5.5;
        }

        else{
            counter+=1.0;
            prom+=query.value(3).toDouble();
        }

    }
    if(counter==0.0){
       ui->text_output->setText("No hay datos a mostrar");
    }

    else{
        output+="Promedio del estudiante en el periodo especificado: " + QString::number(prom/counter);
        ui->text_output->setText(output);
    }
    return true;
}

MainWindow::~MainWindow(){
    delete ui;
}
