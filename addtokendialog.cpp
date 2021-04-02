#include "addtokendialog.h"
#include "ui_addtokendialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>

AddTokenDialog::AddTokenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTokenDialog)
{

    ui->setupUi(this);
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
    EncryptService::GetInstance()->createDecryptedData(tokenData, &decryptedData);
    switch (mode) {

    case CREATE:{
        if(EncryptService::GetInstance()->containsToken(token)){
            QMessageBox::information(this, "Error", "The token already exists!");
            return;
        }
    }
    }
    if(mainWindow->addToken(token, decryptedData, mode)){
        closeDialog();
    }else{
        QMessageBox::information(this, "Error", "Cannot save data!");
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

void AddTokenDialog::setMode(Mode mode){
    this->mode = mode;
    bool readOnly = mode == EDIT;
        this->ui->token_->setReadOnly(readOnly);
}

void AddTokenDialog::setData(const Token &token, const DecryptedData &decryptedData){
    this->ui->token_->setText(QString(token.data.c_str()));
    this->ui->data_->document()->setPlainText(QString(EncryptService::GetInstance()->toStdString(decryptedData).c_str()));

}

