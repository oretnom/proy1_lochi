#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>

//Main window call

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->edit_password->setEchoMode(QLineEdit::Password);
}

//Connection to database and showing tables

bool MainWindow::connect(){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery query;
    QString output;
    int i;

    db.setHostName("localhost");
    db.setDatabaseName("proyecto_1");
    db.setUserName(ui->edit_username->text());
    db.setPassword(ui->edit_password->text());

    if(!db.open()){
        QMessageBox::critical(0,"Database error!",db.lastError().text());
        return false;
    }
    QMessageBox::information(this,"Success!","Connection established");
    ui->edit_username->clear();
    ui->edit_password->clear();

    query.exec("SELECT DISTINCT provincia FROM Ingresados ORDER BY provincia");
    while(query.next()){
        ui->dropdown_provincia->addItem(query.value(0).toString());
    }

    query.exec("SELECT DISTINCT sigla FROM Notas ORDER BY sigla");
    while(query.next()){
        ui->dropdown_sigla->addItem(query.value(0).toString());
    }

    for(i=0; i<11; i++) ui->dropdown_nota_min->addItem(QString::number(i));

    query.exec("SHOW TABLES");
    output += "Tablas en la base de datos:\n\n";
    while(query.next()){
        output += query.value(0).toString()+"\n";
    }

    ui->text_output->setText(output);

    return true;
}

//Nombre y cantidad de graduados dado cierto cantón

bool MainWindow::graduados_canton(){

    QSqlQuery query;
    QString output,input;
    int graduados = 0;

    input = ui->dropdown_canton->currentText();

    query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.canton = :canton");
    query.bindValue(":canton",input);
    query.exec();

    while(query.next()){
        output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
        graduados += 1;
    }

    output += "\n Total de graduados en " + input + ": ";
    output.append(QString("%1").arg(graduados));
    ui->text_output->setText(output);

    return true;
}

//Nombre y nota de estudiantes dado un curso y un umbral mínimo de nota

bool MainWindow::nota_curso_limitada(){

    QSqlQuery query;
    QString output,input_curso;
    int input_nota;

    input_curso = ui->dropdown_sigla->currentText();
    input_nota = ui->dropdown_nota_min->currentText().toInt();

    query.prepare("SELECT nombre, apellido1, apellido2, notaordinaria FROM Notas WHERE notaordinaria > :nota AND sigla = :curso");
    query.bindValue(":nota",input_nota);
    query.bindValue(":curso",input_curso);
    query.exec();

    while(query.next()){
        output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + " " + query.value(3).toString() + "\n";
    }

    ui->text_output->setText(output);

    return true;
}

//Llenado de la lista de cantones según la provincia seleccionada

bool MainWindow::fill_list_canton(){

    QSqlQuery query;

    ui->dropdown_canton->clear();
    query.prepare("SELECT DISTINCT canton FROM Ingresados WHERE provincia = :provincia ORDER BY canton");
    query.bindValue(":provincia",ui->dropdown_provincia->currentText());
    query.exec();
    while(query.next()){
        ui->dropdown_canton->addItem(query.value(0).toString());
    }

    return true;
}

//Llenado de la lista de distritos según el cantón seleccionado

bool MainWindow::fill_list_distrito(){

    QSqlQuery query;

    ui->dropdown_distrito->clear();
    query.prepare("SELECT DISTINCT distrito FROM Ingresados WHERE canton = :canton ORDER BY distrito");
    query.bindValue(":canton",ui->dropdown_canton->currentText());
    query.exec();
    while(query.next()){
        ui->dropdown_distrito->addItem(query.value(0).toString());
    }

    return true;
}

MainWindow::~MainWindow(){
    delete ui;
}
