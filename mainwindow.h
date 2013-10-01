#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Ui::MainWindow *ui;
    ~MainWindow();

public slots:
    bool connect();
    bool graduados_region();
    bool nota_curso_limitada();
    bool fill_list_canton();
    bool fill_list_distrito();
    bool promedio_curso_region();
    bool promedio_estudiante();
    bool ingresados_region();

private:
//    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
