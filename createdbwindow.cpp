#include "createdbwindow.h"
#include "ui_createdbwindow.h"
#include <QMessageBox>
#include "core/Encryptor.h"
#include "loginwindow.h"


CreateDbWindow::CreateDbWindow(EncryptService *encryptService, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateDbWindow)
{
    this->encryptService = encryptService;
    ui->setupUi(this);
}

CreateDbWindow::~CreateDbWindow()
{
    delete ui;
}

void CreateDbWindow::on_create__clicked()
{
    QString pass = ui->pass_->text();
    QString r_pass = ui->r_pass_->text();
    if(pass != r_pass){
        QMessageBox::information(this, "Master password", "Enetered password are not the same!");
    }else if(pass.length() < KEY_MIN_LENGTH){
        QMessageBox::information(this, "Master password", "Password is too short!");
    }else{
        hide();
        loginWindow = new LoginWindow(encryptService, this);
        loginWindow->show();
    }
}
