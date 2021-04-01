#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(EncryptService * encryptService, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->enctyptService = encryptService;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
