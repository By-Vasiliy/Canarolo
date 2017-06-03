#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <complex>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

using namespace  std;


template<typename T>
T &real(std::complex<T> &c) {
  return reinterpret_cast<T*>(&c)[0];
}

template<typename T>
T &imag(std::complex<T> &c) {
  return reinterpret_cast<T*>(&c)[1];
}


static const complex <double> mnim1(0, 1.0);
static const double pi = 3.14159265;

//функция, возвращающая двумерный массив - матрицу размерности N*N
complex <double> **matrix(int n){
  complex <double> **m = new complex <double> *[n];
  for (int i=0;i<n;i++){
    m[i] = new complex <double> [n];
  }
  for (int i=0;i<n;i++){
      for (int j=0;j<n;j++){
        m[i][j]=exp(2*pi*mnim1*(double)(i)*(double)(j)/(double)(n));
      }
  }

  return m;
}

//функция, возращающая обратную матрицу
complex <double> **inv(complex <double> **mas, int n){
    complex <double> **invmatrix = new complex <double> *[n];
    complex <double> nz (n,0);
    for (int i=0;i<n;i++){
        invmatrix[i] = new complex <double> [n];
        for (int j=0;j<n;j++){
            invmatrix[i][j]=conj(mas[i][j])/nz;
        }
    }
    return invmatrix;
}
//функция, возвращающая вектор=матрица*вектор
complex <double> *multypl (complex <double> **mas, complex <double> *v1, int n){
    complex <double> *v2 = new complex <double> [n];
    for (int i=0;i<n;i++){
        v2[i]=0;
        for (int j=0;j<n;j++){
            v2[i]+=mas[i][j]*v1[j];
        }
    }
    return v2;
}

//копирование массива
void arraycopy (complex <double> mas1[], complex <double> mas2[], int n){
    for (int i=0; i<n; i++){
        mas2[i]=mas1[i];
    }
}

//сортировка массива
void bubblesort(complex <double> mas[], int n){
    complex <double> a;
    for(int i=0; i<n; i++) {
        for(int j=n-1; j>i; j--) {
            if (abs(mas[j-1])>abs(mas[j])) {
                a=mas[j-1];
                mas[j-1]=mas[j];
                mas[j]=a;
            }
        }
    }
}
int counter = 0;
void czeroed (complex <double> c[], int k, int n){
    complex <double> *c0 = new complex <double> [n];
    arraycopy(c, c0, n);
    bubblesort(c0, n);
    counter=0;
    for (int i=0, j=0;i<n && j<=k;i++){
        if (abs(c[i])<=abs(c0[k-1])){
            //c[i]=0;
            c[i]=0;
            j++;
            counter++;
        }
    }
}

double BottomBound = -10, TopBound = 10;
complex <double> *f = new complex <double> [1];
complex <double> **T = new complex <double> *[1];
complex <double> *c = new complex <double> [1];
complex <double> **invT=new complex <double> *[1];

// Load/OnChange
complex <double> *c1 = new complex <double> [1];
complex <double> *f1 = new complex <double> [1];


double max_x=0,min_x=0,max_y=0,min_y=0;




//функция, возвращающая массив N случайных комплексных чисел от a до b
void random_array(complex <double> mas[], int n, double a, double b){
    for (int i=0; i<n; i++){
        mas[i].real((double)(rand())/RAND_MAX * (b-a) + a);
        mas[i].imag((double)(rand())/RAND_MAX * (b-a) + a);
    }
}


void MainWindow::basic(){
    int n = ui->spinBox->value();
    f = new complex <double> [n];
    random_array(f, n, ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value());
    T = new complex <double> *[n];
    T=matrix(n);
    c = new complex <double> [n];
    invT=inv(T, n);
    c = multypl(invT, f, n);

    QVector<double> x(n), y(n); //Массивы координат точек
        max_x = f[0].real();
        min_x = f[0].real();
        max_y=f[0].imag();
        min_y=f[0].imag();
        for (int i = 0; i<n; i++)//Пробегаем по всем точкам
        {
            x[i] = f[i].real();
            y[i] = f[i].imag();
            if (f[i].real() > max_x) {
                max_x = f[i].real();
            }
            if (f[i].real() < min_x) {
                min_x = f[i].real();
            }
            if (f[i].imag() > max_y){
                max_y=f[i].imag();
            }
            if (f[i].imag() < min_y){
                min_y=f[i].imag();
            }
        }

        ui->widget->graph(0)->setData(x, y);

        //Установим область, которая будет показываться на графике
        ui->widget->xAxis->setRange(min_x, max_x);//Для оси Ox
        ui->widget->yAxis->setRange(min_y, max_y);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget->replot();
        various();
}

void MainWindow::various(){
    int hs = ui->horizontalSlider->value();
    int n = ui->spinBox->value();
    double temp = (double)n/100.0*(double)hs;
    int k = (int)temp;



    c1 = new complex <double> [n];
    arraycopy(c, c1, n);
    czeroed(c1, k, n);

    ui->spinBox_3->setProperty("value", counter);

    f1 = new complex <double> [n];
    f1=multypl(T, c1, n);

    QVector<double> x(n), y(n); //Массивы координат точек
        for (int i = 0; i<n; i++)//Пробегаем по всем точкам
        {
            x[i] = f1[i].real();
            y[i] = f1[i].imag();
            if (f1[i].real() > max_x) {
                max_x = f1[i].real();
            }
            if (f1[i].real() < min_x) {
                min_x = f1[i].real();
            }
            if (f1[i].imag() > max_y){
                max_y=f1[i].imag();
            }
            if (f1[i].imag() < min_y){
                min_y=f1[i].imag();
            }
        }

        ui->widget->graph(1)->setData(x, y);

        //Установим область, которая будет показываться на графике
        ui->widget->xAxis->setRange(min_x, max_x);//Для оси Ox
        ui->widget->yAxis->setRange(min_y, max_y);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget->replot();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(time(0));

    ui->setupUi(this);
    //
    ui->widget->addGraph();
    ui->widget->addGraph();
    //
    ui->widget->graph(0)->setPen(QColor(255, 0, 0, 255));
    ui->widget->graph(1)->setPen(QColor(0, 0, 255, 255));
    //
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    //Подписываем оси Ox и Oy
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    basic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Рисуем график y=x*x

        //Сгенерируем данные
        //Для этого создадим два массива точек:
        //один для созранения x координат точек,
        //а второй для y соответственно

        double a = -1; //Начало интервала, где рисуем график по оси Ox
        double b =  1; //Конец интервала, где рисуем график по оси Ox
        double h = 0.01; //Шаг, с которым будем пробегать по оси Ox

        int N=(b-a)/h + 2; //Вычисляем количество точек, которые будем отрисовывать
        QVector<double> x(N), y(N); //Массивы координат точек

        //Вычисляем наши данные
        int i=0;
        for (double X=a; X<=b; X+=h)//Пробегаем по всем точкам
        {
            x[i] = X;
            y[i] = X*X;//Формула нашей функции
            i++;
        }

        ui->widget->clearGraphs();//Если нужно, но очищаем все графики
        //Добавляем один график в widget
        ui->widget->addGraph();
        //Говорим, что отрисовать нужно график по нашим двум массивам x и y
        ui->widget->graph(0)->setData(x, y);

        //Подписываем оси Ox и Oy
        ui->widget->xAxis->setLabel("x");
        ui->widget->yAxis->setLabel("y");

        //Установим область, которая будет показываться на графике
        ui->widget->xAxis->setRange(a, b);//Для оси Ox

        //Для показа границ по оси Oy сложнее, так как надо по правильному
        //вычислить минимальное и максимальное значение в векторах
        double minY = y[0], maxY = y[0];
        for (int i=1; i<N; i++)
        {
            if (y[i]<minY) minY = y[i];
            if (y[i]>maxY) maxY = y[i];
        }
        ui->widget->yAxis->setRange(minY, maxY);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget->replot();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    basic();
    various();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    various();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->doubleSpinBox_2->setProperty("minimum",ui->doubleSpinBox->value());
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    ui->doubleSpinBox->setProperty("maximum",ui->doubleSpinBox_2->value());
}
