#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(EncryptService * encryptService, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->encryptService = encryptService;

    model = new QStringListModel(this);
    QStringList tokens;

    foreach(const Token token,encryptService->getTokens()){
        const std::string str = token.data;
        tokens << str.c_str();
    }
    model->setStringList(tokens);
    ui->tokensList_->setModel(model);
    ui->tokensList_->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    if(model != nullptr){
        delete model;
    }
    if(addTokenDialog !=nullptr){
        delete addTokenDialog;
    }
    delete encryptService;
    delete ui;

}

bool MainWindow::addToken(const Token &token, const DecryptedData &decryptedData){
    SaveResult saveResult = encryptService->encrypt(token, decryptedData);
    if(saveResult == SAVE_SUCCESS){
        if(model->insertRow(model->rowCount())) {
            QModelIndex index = model->index(model->rowCount() - 1, 0);
            model->setData(index, QString(token.data.c_str()));
        }
    }
    return true;
}

void MainWindow::removeToken(const Token &token){

    SaveResult result = encryptService->removeToken(token);
    if(result == SAVE_SUCCESS){
        QModelIndex index = ui->tokensList_->selectionModel()->selectedIndexes().at(0);
        model->removeRow(index.row());
    }else{
        QMessageBox::information(this, "Remove token", "Cannot remove token!");
    }
}

void MainWindow::on_addToken__clicked()
{
    if(addTokenDialog!= nullptr){
        addTokenDialog->show();
    }else{
        addTokenDialog = new AddTokenDialog(this->encryptService, this);
        addTokenDialog->show();
    }
}

void MainWindow::on_deleteToken__clicked()
{

}
