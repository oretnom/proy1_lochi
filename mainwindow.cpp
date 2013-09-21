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

    ui->dropdown_provincia->clear();
    query.exec("SELECT DISTINCT provincia FROM Ingresados ORDER BY provincia");
    while(query.next()){
        ui->dropdown_provincia->addItem(query.value(0).toString());
    }
    ui->dropdown_provincia->addItem("TODAS");

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

    return true;
}

//Displaying names and quantity of graduates according to "canton"

bool MainWindow::graduados_region(){

    QSqlQuery query;
    QString output,input_provincia,input_canton,input_distrito;
    int graduados = 0;

    input_provincia = ui->dropdown_provincia->currentText();
    input_canton = ui->dropdown_canton->currentText();
    input_distrito = ui->dropdown_distrito->currentText();

    query.prepare("SELECT Graduados.nombre,Graduados.apellido1,Graduados.apellido2 FROM Graduados,Ingresados WHERE Graduados.carne = Ingresados.carne AND Ingresados.canton = :canton");
    query.bindValue(":canton",input_canton);
    query.exec();

    while(query.next()){
        output += query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString() + "\n";
        graduados += 1;
    }

    output += "\n Total de graduados en la region: ";
    output.append(QString("%1").arg(graduados));
    ui->text_output->setText(output);

    return true;
}

//Displaying names and scores given "curso", "año inicial", "año final" and "notas" in certain range

bool MainWindow::nota_curso_limitada(){

    QSqlQuery query;
    QString output,input_curso;
    int input_nota_min,input_nota_max,input_ano_inicial,input_ano_final;

    input_curso = ui->dropdown_sigla->currentText();
    input_nota_min = ui->dropdown_nota_min->currentText().toInt();
    input_nota_max = ui->dropdown_nota_max->currentText().toInt();
    input_ano_inicial = ui->dropdown_ano_inicial->currentText().toInt();
    input_ano_final = ui->dropdown_ano_final->currentText().toInt();

    query.prepare("SELECT nombre, apellido1, apellido2, notaordinaria FROM Notas WHERE notaordinaria >= :nota_min AND notaordinaria <= :nota_max AND sigla = :curso AND anio >= :ano_inicial AND anio <= :ano_final");
    query.bindValue(":nota_min",input_nota_min);
    query.bindValue(":nota_max",input_nota_max);
    query.bindValue(":curso",input_curso);
    query.bindValue(":ano_inicial",input_ano_inicial);
    query.bindValue(":ano_final",input_ano_final);
    query.exec();

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

        while(query.next()){
            ui->dropdown_canton->addItem(query.value(0).toString());
        }
        ui->dropdown_canton->addItem("TODOS");
    }

    return true;
}

//Filling "distrito" drop-down list

bool MainWindow::fill_list_distrito(){

    QSqlQuery query;

    ui->dropdown_distrito->clear();

    if(ui->dropdown_canton->currentText()!="TODOS"){
        query.prepare("SELECT DISTINCT distrito FROM Ingresados WHERE canton = :canton ORDER BY distrito");
        query.bindValue(":canton",ui->dropdown_canton->currentText());
        query.exec();
        while(query.next()){
            ui->dropdown_distrito->addItem(query.value(0).toString());
        }
        ui->dropdown_distrito->addItem("TODOS");
    }

    return true;
}

MainWindow::~MainWindow(){
    delete ui;
}
