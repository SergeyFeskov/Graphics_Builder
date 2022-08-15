#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include "parser.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pictHeight = 370;
    pictWidth = 540;
    step = 0.1;
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    drawGraph();
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_two, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_e, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_exp, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_abs, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_multiply, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_tg, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_ctg, SIGNAL(clicked()), this, SLOT(func()));
        connect(ui->pushButton_leftBkt, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_rightBkt, SIGNAL(clicked()), this, SLOT(digits_numbers()));
        connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(digits_numbers()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::func(){
    QPushButton *button=(QPushButton*)sender();
    QString new_label;
    new_label=(ui->functionShow->text()+button->text()+'(');


    ui->functionShow->setText(new_label);
}

void MainWindow::digits_numbers(){
    QPushButton *button=(QPushButton*)sender();
    QString new_label;
    new_label=(ui->functionShow->text()+button->text());


    ui->functionShow->setText(new_label);
}

void MainWindow::correct(){
    QString st = "Correct";
    ui->showError->setText(st);
}

double MainWindow::f(double x, QString QExpr)
{
    correct();
    int tokensAm;
    QByteArray ch = QExpr.toLocal8Bit();
    char *expr = ch.data();
    TLexem* tokens;
    tokens = lexer(expr, &tokensAm);
    TTreeNode* tree;

    try{
        tree = parser(tokens, tokensAm);
    }
    catch(Error &exception){
        QString str;
        str = exception.getError();
        ui->showError->setText(str);
    }
    inputVars(x);
    return calcExpr(tree);
}

double MainWindow::f1(double x)
{
    return sin(x)*(-1);
}

double MainWindow::f2(double x)
{
    return cos(x)*(-1);
}

double MainWindow::f3(double x)
{
    return sin(1.0/x)*(-1);
}

void MainWindow::recountPixels()
{
    onePixelX = 540.0/(rightX-leftX);
    onePixelY = 370.0/(rightY-leftY);
    Ox = fabs(leftX); Oy = rightY;
}

void MainWindow::getData()
{
    leftX = ui->inputLeftX->text().toDouble()*(-1);
    rightX = ui->inputRightX->text().toDouble();
    leftY = ui->inputLeftY->text().toDouble()*(-1);
    rightY = ui->inputRightY->text().toDouble();
    step = 1.0/ui->inputAccuracy->text().toDouble();
}

void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(540,370);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,540,370);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX;i<=rightX;i+=1.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=1.0)
        paint.drawPoint(Ox*onePixelX,(Oy-i)*onePixelY);

    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path,p[3];
    bool first[4] = {1,1,1,1};
    QString QExpr = ui->functionShow->text();
    for(double i = (double)leftX+step;i<=(double)rightX;i+=step) {
        if(!isnan(f(i, QExpr))) {
            if(first[0]) {
                path.moveTo((i+Ox)*onePixelX,(Oy-f(i, QExpr))*onePixelY);
                first[0] = false;
            }
            else
                path.lineTo((i+Ox)*onePixelX,(Oy-f(i, QExpr))*onePixelY);
        }
        if(!isnan(f1(i))) {
            if(first[1]) {
                p[0].moveTo((i+Ox)*onePixelX,(f1(i)+Oy)*onePixelY);
                first[1] = false;
            }
            else
                p[0].lineTo((i+Ox)*onePixelX,(f1(i)+Oy)*onePixelY);
        }
        if(!isnan(f2(i))) {
            if(first[2]) {
                p[1].moveTo((i+Ox)*onePixelX,(f2(i)+Oy)*onePixelY);
                first[2] = false;
            }
            else
                p[1].lineTo((i+Ox)*onePixelX,(f2(i)+Oy)*onePixelY);
        }
        if(!isnan(f3(i))) {
            if(first[3]) {
                p[2].moveTo((i+Ox)*onePixelX,(f3(i)+Oy)*onePixelY);
                first[3] = false;
            }
            else
                p[2].lineTo((i+Ox)*onePixelX,(f3(i)+Oy)*onePixelY);
        }
    }
    if(ui->main->isChecked()) {
        paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
        paint.drawPath(path);
    }
    if(ui->sin->isChecked()) {
        paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
        paint.drawPath(p[0]);
    }
    if(ui->cos->isChecked()) {
        paint.setPen(QPen(Qt::red,1,Qt::SolidLine));
        paint.drawPath(p[1]);
    }
    if(ui->tg->isChecked()) {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[2]);
    }
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}

void MainWindow::on_exit_clicked()
{
    this->close();
}



void MainWindow::on_draw_clicked()
{
    getData();
    recountPixels();
    drawGraph(1);
}

void MainWindow::on_save_clicked()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QString name;
   if(date.day()<10)
        name += "0";
    name += QString::number(date.day())+".";
    if(date.month()<10)
        name += "0";
    name += QString::number(date.month())+".";
    name += QString::number(date.year())+"_";
    if(time.hour()<10)
        name += "0";
    name += QString::number(time.hour())+"-";
    if(time.minute()<10)
        name += "0";
    name += QString::number(time.minute())+"-";
    if(time.second()<10)
        name += "0";
    name += QString::number(time.second());
    QFile file(name+".png");
    qDebug() << name;
    file.open(QIODevice::WriteOnly);
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Ok);
    if(ui->outputGraph->pixmap().save(&file,"PNG")) {
        msgBox.setText("Saved to program folder with name: "+name+".png");
        msgBox.setWindowTitle("Saved!");
    }
    else {
        msgBox.setText("Error saving.");
        msgBox.setWindowTitle("Error!");
    }
    msgBox.exec();
}



void MainWindow::on_pushButton_del_clicked()
{
    QString temp;
    temp = ui->functionShow->text();
    int n, j;
    j = temp.length()-1;
    if(temp.isEmpty()!=true){
    do{
        if(temp[temp.length()-1]=='(' && temp[temp.length()-2]>='a'
            && temp[temp.length()-2]<='z'&& temp[temp.length()-2]!='x'&&j>=2){
            n = temp.length()-1;
            temp.remove(n, 1);
            ui->functionShow->setText(temp);
            j = temp.length()-1;
            while(temp[j]>='a' && temp[j]<='z' && j>0){
                n = temp.length()-1;
                temp.remove(n, 1);
                ui->functionShow->setText(temp);
                j--;
            }
            if(temp[j]>='a'&&temp[j]<='z'&&j==0){
                n = temp.length()-1;
                temp.remove(n, 1);
                ui->functionShow->setText(temp);
            }
            break;
        }
        else{
            n = temp.length()-1;
            temp.remove(n, 1);
            ui->functionShow->setText(temp);
            break;
        }
    }while(1);
}
}


void MainWindow::on_pushButton_AC_clicked()
{
    ui->functionShow->setText("");

}


void MainWindow::on_clear_clicked()
{
    recountPixels();
    drawGraph();
}


void MainWindow::on_pushButton_pi_clicked()
{
    ui->functionShow->setText("3.1415");
}

