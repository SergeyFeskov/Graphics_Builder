#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtGui>

namespace Ui {
class MainWindow;
class Graph;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(bool notEmpty = 0);
    void recountPixels();
    void getData();
    void correct();
    double f(double x, QString expr);
    double f1(double x);
    double f2(double x);
    double f3(double x);
    
private slots:
    void digits_numbers();
    void func();
    void on_exit_clicked();
    void on_draw_clicked();
    void on_save_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_AC_clicked();
    void on_clear_clicked();
    void on_pushButton_pi_clicked();

private:
    Ui::MainWindow *ui;
    //QString userFunction;
    double leftX,rightX;
    double leftY,rightY;
    int pictWidth,pictHeight;
    double step;
    double onePixelX,onePixelY;
    double Ox,Oy;
};


#endif // MAINWINDOW_H
