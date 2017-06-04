#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <complex>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

using namespace std;


template<typename MATRIX>
MATRIX &real(std::complex <MATRIX> &c) {
    return reinterpret_cast<MATRIX *>(&c)[0];
}

template<typename MATRIX>
MATRIX &imag(std::complex <MATRIX> &c) {
    return reinterpret_cast<MATRIX *>(&c)[1];
}


static const complex<double> mnim1(0, 1.0);
static const double pi = 3.14159265;

//функция, возвращающая двумерный массив - матрицу размерности N*N
complex<double> **calculateMatrix(int n) {
    complex<double> **m = new complex<double> *[n];
    for (int i = 0; i < n; i++) {
        m[i] = new complex<double>[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = exp(2 * pi * mnim1 * (double) (i) * (double) (j) / (double) (n));
        }
    }

    return m;
}

//функция, возращающая обратную матрицу
complex<double> **calculateInverseMatrix(complex<double> **mas, int n) {
    complex<double> **invmatrix = new complex<double> *[n];
    complex<double> nz(n, 0);
    for (int i = 0; i < n; i++) {
        invmatrix[i] = new complex<double>[n];
        for (int j = 0; j < n; j++) {
            invmatrix[i][j] = conj(mas[i][j]) / nz;
        }
    }
    return invmatrix;
}

//пользовательская функция
complex<double> *userfunction(complex<double> *z, int n) {
    complex<double> *f = new complex<double>[n];
    for (int i = 0; i < n; i++) {
        //f[i]=z[i]*z[i];
        f[i] = pow(sin(z[i]), sin(z[i]));
    }
    return f;
}

//функция, возвращающая вектор=матрица*вектор
complex<double> *multypl(complex<double> **mas, complex<double> *v1, int n) {
    complex<double> *v2 = new complex<double>[n];
    for (int i = 0; i < n; i++) {
        v2[i] = 0;
        for (int j = 0; j < n; j++) {
            v2[i] += mas[i][j] * v1[j];
        }
    }
    return v2;
}

//копирование массива
void complexArrayCopy(complex<double> mas1[], complex<double> mas2[], int n) {
    for (int i = 0; i < n; i++) {
        mas2[i] = mas1[i];
    }
}

//сортировка массива
void bubblesortcomplex(complex<double> mas[], int n) {
    complex<double> a;
    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j > i; j--) {
            if (abs(mas[j - 1]) > abs(mas[j])) {
                a = mas[j - 1];
                mas[j - 1] = mas[j];
                mas[j] = a;
            }
        }
    }
}

void bubbleSortDouble(complex<double> mas[], int n) {
    complex<double> a;
    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j > i; j--) {
            if (mas[j - 1].real() > mas[j].real()) {
                a = mas[j - 1];
                mas[j - 1] = mas[j];
                mas[j] = a;
            }
        }
    }
}

int counter = 0;

void czeroedcomplex(complex<double> c[], int k, int n) {
    complex<double> *c0 = new complex<double>[n];
    complexArrayCopy(c, c0, n);
    bubblesortcomplex(c0, n);
    counter = 0;
    for (int i = 0, j = 0; i < n && j < k; i++) {
        if (abs(c[i]) <= abs(c0[k - 1]) && k != 0) {
            //c[i]=0;
            c[i] = 0;
            j++;
            counter++;
        }
    }
}

void czeroeddouble(complex<double> c[], int k, int n) {
    complex<double> *c0 = new complex<double>[n];
    complexArrayCopy(c, c0, n);
    bubblesortcomplex(c0, n);
    counter = 0;
    for (int i = 0, j = 0; i < n && j <= k; i++) {
        if (c[i].real() <= c0[k - 1].real()) {
            //c[i]=0;
            c[i] = 0;
            j++;
            counter++;
        }
    }
}

double BottomBound = -10, TopBound = 10;
complex<double> *z = new complex<double>[1];
complex<double> *f = new complex<double>[1];
complex<double> **MATRIX = new complex<double> *[1];
complex<double> *c = new complex<double>[1];
complex<double> **invT = new complex<double> *[1];

// Load/OnChange
complex<double> *c1 = new complex<double>[1];
complex<double> *f1 = new complex<double>[1];


double max_x = 0, min_x = 0, max_y = 0, min_y = 0, max2_x = 0, min2_x = 0, max2_y = 0, min2_y = 0;

//функция, возвращающая массив N случайных комплексных чисел от a до b
void complexRandomArrayGenerate(complex<double> mas[], int n, double a, double b) {
    for (int i = 0; i < n; i++) {
        mas[i].real((double) (rand()) / RAND_MAX * (b - a) + a);
        mas[i].imag((double) (rand()) / RAND_MAX * (b - a) + a);
    }
}

void doubleRandomArrayGenerate(complex<double> mas[], int n, double a, double b) {
    for (int i = 0; i < n; i++) {
        mas[i].real((double) (rand()) / RAND_MAX * (b - a) + a);
        mas[i].imag(0);
    }
}

void MainWindow::yinOne() {
    int n = ui->spinBox->value();
    f = new complex<double>[n];
    complexRandomArrayGenerate(f, n, ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value());
    MATRIX = new complex<double> *[n];
    MATRIX = calculateMatrix(n);
    c = new complex<double>[n];
    invT = calculateInverseMatrix(MATRIX, n);
    c = multypl(invT, f, n);

    QVector<double> x(n), y(n); //Массивы координат точек
    max_x = f[0].real();
    min_x = f[0].real();
    max_y = f[0].imag();
    min_y = f[0].imag();
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x[i] = f[i].real();
        y[i] = f[i].imag();
        if (f[i].real() > max_x) {
            max_x = f[i].real();
        }
        if (f[i].real() < min_x) {
            min_x = f[i].real();
        }
        if (f[i].imag() > max_y) {
            max_y = f[i].imag();
        }
        if (f[i].imag() < min_y) {
            min_y = f[i].imag();
        }
    }
    ui->widget->graph(1)->setData(x, y);
    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(min_x * 1.05, max_x * 1.05);//Для оси Ox
    ui->widget->yAxis->setRange(min_y * 1.05, max_y * 1.05);//Для оси Oy
    //И перерисуем график на нашем widget
    ui->widget->replot();
    eventYan();
}

void MainWindow::yinTwo() {
    int n = ui->spinBox->value();
    z = new complex<double>[n];
    f = new complex<double>[n];
    doubleRandomArrayGenerate(z, n, ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value());
    f = userfunction(z, n);
    MATRIX = new complex<double> *[n];
    MATRIX = calculateMatrix(n);
    c = new complex<double>[n];
    invT = calculateInverseMatrix(MATRIX, n);
    c = multypl(invT, f, n);

    QVector<double> x(n), y(n); //Массивы координат точек
    max_x = z[0].real();
    min_x = z[0].real();
    max_y = f[0].real();
    min_y = f[0].real();
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x[i] = z[i].real();
        y[i] = f[i].real();
        if (z[i].real() > max_x) {
            max_x = z[i].real();
        }
        if (z[i].real() < min_x) {
            min_x = z[i].real();
        }
        if (f[i].real() > max_y) {
            max_y = f[i].real();
        }
        if (f[i].real() < min_y) {
            min_y = f[i].real();
        }
    }
    ui->widget->graph(1)->setData(x, y);
    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(min_x * 1.05, max_x * 1.05);//Для оси Ox
    ui->widget->yAxis->setRange(min_y * 1.05, max_y * 1.05);//Для оси Oy
    //И перерисуем график на нашем widget
    ui->widget->replot();


    QVector<double> x2(n), y2(n); //Массивы координат точек
    max2_x = f[0].real();
    min2_x = f[0].real();
    max2_y = f[0].imag();
    min2_y = f[0].imag();
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x2[i] = f[i].real();
        y2[i] = f[i].imag();
        if (f[i].real() > max2_x) {
            max2_x = f[i].real();
        }
        if (f[i].real() < min2_x) {
            min2_x = f[i].real();
        }
        if (f[i].imag() > max2_y) {
            max2_y = f[i].imag();
        }
        if (f[i].imag() < min2_y) {
            min2_y = f[i].imag();
        }
    }
    ui->widget_2->graph(0)->setData(x2, y2);
    ui->widget_2->xAxis->setRange(min2_x * 1.05, max2_x * 1.05);//Для оси Ox
    ui->widget_2->yAxis->setRange(min2_y * 1.05, max2_y * 1.05);//Для оси Oy
    ui->widget_2->replot();

    eventYan();
}

void MainWindow::yanOne() {
    int hs = ui->horizontalSlider->value();
    int n = ui->spinBox->value();
    double temp = (double) n / 100.0 * (double) hs;
    int k = int(temp);
    if (hs == 0) {
        k = 0;
    }
    if (hs == 100) {
        k = n;
    }
    c1 = new complex<double>[n];
    complexArrayCopy(c, c1, n);
    czeroedcomplex(c1, k, n);

    ui->spinBox_3->setProperty("value", counter);

    f1 = new complex<double>[n];
    f1 = multypl(MATRIX, c1, n);

    QVector<double> x(n), y(n); //Массивы координат точек
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x[i] = f1[i].real();
        y[i] = f1[i].imag();
        if (f1[i].real() > max_x) {
            max_x = f1[i].real();
        }
        if (f1[i].real() < min_x) {
            min_x = f1[i].real();
        }
        if (f1[i].imag() > max_y) {
            max_y = f1[i].imag();
        }
        if (f1[i].imag() < min_y) {
            min_y = f1[i].imag();
        }
    }

    ui->widget->graph(0)->setData(x, y);

    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(min_x, max_x);//Для оси Ox
    ui->widget->yAxis->setRange(min_y, max_y);//Для оси Oy

    //И перерисуем график на нашем widget
    ui->widget->replot();
}

void MainWindow::yanTwo() {
    int hs = ui->horizontalSlider->value();
    int n = ui->spinBox->value();
    double temp = (double) n / 100.0 * (double) hs;
    int k = int(temp);
    if (hs == 0) {
        k = 0;
    }
    if (hs == 100) {
        k = n;
    }
    c1 = new complex<double>[n];
    complexArrayCopy(c, c1, n);
    czeroedcomplex(c1, k, n);

    ui->spinBox_3->setProperty("value", counter);

    f1 = new complex<double>[n];
    f1 = multypl(MATRIX, c1, n);

    QVector<double> x(n), y(n); //Массивы координат точек
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x[i] = z[i].real();
        y[i] = f1[i].real();
        if (z[i].real() > max_x) {
            max_x = z[i].real();
        }
        if (z[i].real() < min_x) {
            min_x = z[i].real();
        }
        if (f1[i].real() > max_y) {
            max_y = f1[i].real();
        }
        if (f1[i].real() < min_y) {
            min_y = f1[i].real();
        }
    }
    ui->widget->graph(0)->setData(x, y);
    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(min_x, max_x);//Для оси Ox
    ui->widget->yAxis->setRange(min_y, max_y);//Для оси Oy
    //И перерисуем график на нашем widget
    ui->widget->replot();

    QVector<double> x2(n), y2(n); //Массивы координат точек
    max2_x = f1[0].real();
    min2_x = f1[0].real();
    max2_y = f1[0].imag();
    min2_y = f1[0].imag();
    for (int i = 0; i < n; i++)//Пробегаем по всем точкам
    {
        x2[i] = f1[i].real();
        y2[i] = f1[i].imag();
        if (f1[i].real() > max2_x) {
            max2_x = f1[i].real();
        }
        if (f1[i].real() < min2_x) {
            min2_x = f1[i].real();
        }
        if (f1[i].imag() > max2_y) {
            max2_y = f1[i].imag();
        }
        if (f1[i].imag() < min2_y) {
            min2_y = f1[i].imag();
        }
    }
    ui->widget_2->graph(1)->setData(x2, y2);
    //Установим область, которая будет показываться на графике
    ui->widget_2->xAxis->setRange(min2_x, max2_x);//Для оси Ox
    ui->widget_2->yAxis->setRange(min2_y, max2_y);//Для оси Oy
    //И перерисуем график на нашем widget
    ui->widget_2->replot();
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);
    ui->widget->resize(QMainWindow::width(), QMainWindow::height() - 155);
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    srand(time(0));

    ui->setupUi(this);

    initPlots();

    eventYin();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::eventYin() {
    if (ui->radioButton->isChecked()) {
        yinOne();
    } else {
        yinTwo();
    }
}

void MainWindow::eventYan() {
    if (ui->radioButton->isChecked()) {
        yanOne();
    } else {
        yanTwo();
    }
}

void MainWindow::on_spinBox_valueChanged(int value) {
    eventYin();
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    eventYan();
}

void MainWindow::on_doubleSpinBox_valueChanged(double value) {
    ui->doubleSpinBox_2->setProperty("minimum", value);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double value) {
    ui->doubleSpinBox->setProperty("maximum", value);
}

void MainWindow::initPlots() {
    ui->widget->clearGraphs();
    //
    ui->widget->addGraph();
    ui->widget->addGraph();
    //
    ui->widget->graph(0)->setPen(QColor(255, 0, 0, 255));
    ui->widget->graph(1)->setPen(QColor(0, 0, 255, 255));
    //
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->graph(0)->clearData();
    ui->widget->graph(1)->clearData();
    //
    ui->widget_2->clearGraphs();
    ui->widget_2->addGraph();
    ui->widget_2->addGraph();
    //
    ui->widget_2->graph(0)->setPen(QColor(50, 200, 0, 255));
    ui->widget_2->graph(1)->setPen(QColor(255, 100, 50, 255));
    //
    ui->widget_2->xAxis->setLabel("x");
    ui->widget_2->yAxis->setLabel("y");
    //
    ui->widget_2->graph(0)->clearData();
    ui->widget_2->graph(1)->clearData();
    ui->widget_2->hide();
}

void MainWindow::on_radioButton_clicked() {
    initPlots();
    //ui->widget_2->hide();
    eventYin();
}

void MainWindow::on_radioButton_2_clicked() {
    initPlots();
    //ui->widget_2->show();
    eventYin();
}

void MainWindow::on_pushButton_clicked() {
    eventYin();
}
