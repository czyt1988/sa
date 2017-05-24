#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SAValueManager.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    saValueManager->getInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}
