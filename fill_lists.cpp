#include "fill_lists.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>

fill_lists::fill_lists(){

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->centralWidget->setStyleSheet("background-image: url(/home/juan/lab_progra/prueba1/background.jpg)");
}



bool fill_lists::fill_list_canton(){

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

bool fill_lists::fill_list_distrito(){

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

