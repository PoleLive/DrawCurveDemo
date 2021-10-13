#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CurveWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new CurveWidget(this));
    resize(700, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}
