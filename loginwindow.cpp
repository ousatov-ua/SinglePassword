#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}



void LoginWindow::on_DecryptButton_clicked()
{
    decrypt();
}

void LoginWindow::on_masterPass_returnPressed()
{
    decrypt();
}

void LoginWindow::decrypt(){
    std::string masterPass = ui->masterPass->text().toStdString();
    bool result = false;
    try{
        result = EncryptService::GetInstance()->validatePass(masterPass);
    }catch (std::exception &e) {
        QMessageBox::information(this, "Wrong password", "Wrong password provided!");
        return;
    }
    if(!result){
        QMessageBox::information(this, "Wrong password", "Wrong password provided!");
        return;
    }else{
        hide();
        mainWindow = new MainWindow(this);
        mainWindow->show();
    }
}
