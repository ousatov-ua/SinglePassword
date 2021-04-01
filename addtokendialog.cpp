#include "addtokendialog.h"
#include "ui_addtokendialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>

AddTokenDialog::AddTokenDialog(EncryptService *encryptService, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTokenDialog)
{

    ui->setupUi(this);
    this->encryptService = encryptService;
}

AddTokenDialog::~AddTokenDialog()
{
    delete ui;
}

void AddTokenDialog::on_saveButton__clicked()
{

    MainWindow *mainWindow = (MainWindow*)this->parent();
    QString tokenValue = this->ui->token_->text().simplified();
    const Token token = Token{.data = tokenValue.toStdString()};
    if(encryptService->containsToken(token)){
        QMessageBox::information(this, "Error", "The token already exists!");
        return;
    }

    QString data = this->ui->data_->toPlainText();
    if(tokenValue.size() == 0){
        QMessageBox::information(this, "Error", "Wrong token name!");
        return;
    }
    if(data.size() == 0){
        QMessageBox::information(this, "Error", "No data to encrypt!");
        return;
    }
    const std::string tokenData = data.toStdString();
    DecryptedData decryptedData{};
    encryptService->createDecryptedData(tokenData, &decryptedData);
    if(mainWindow->addToken(token, decryptedData)){
        closeDialog();
    }else{
        QMessageBox::information(this, "Error", "Cannot add token!");
    }
}

void AddTokenDialog::on_cancelButton__clicked()
{
    closeDialog();
}

void AddTokenDialog::closeDialog(){
    this->ui->token_->clear();
    this->ui->data_->clear();
    hide();

}

void AddTokenDialog::closeEvent(QCloseEvent * event){
    closeDialog();
}

