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
    ~MainWindow();

public slots:
    bool connect();
    bool graduados_region();
    bool nota_curso_limitada();
    bool fill_list_canton();
    bool fill_list_distrito();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
