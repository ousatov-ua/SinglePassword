#include "addtokendialog.h"
#include "ui_addtokendialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include "core/util.h"

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
    std::string tokenValue = this->ui->token_->text().simplified().toStdString();

    Token token{};
    Util::toToken(tokenValue, token);

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
    DecryptedData decryptedData = EncryptService::GetInstance()->createDecryptedData(tokenData);
    switch (mode) {

    case CREATE:{
        if(EncryptService::GetInstance()->containsToken(token)){
            QMessageBox::information(this, "Error", "The token already exists!");
            return;
        }
    }
    }

    bool result = mainWindow->addToken(token, decryptedData, mode);
    if(result){
        closeDialog();
        return;
    }
    QMessageBox::information(this, "Error", "Cannot save data!");
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
    DecryptedData plainToken{};
    EncryptService::GetInstance()->decrypt(token.data, plainToken);
    this->ui->token_->setText(QString((char*)plainToken.data));
    this->ui->data_->document()->setPlainText(QString(EncryptService::GetInstance()->toStdString(decryptedData).c_str()));

}


void AddTokenDialog::on_data__textChanged()
{

    auto textEdit = ui->data_;
    auto maxLengh = BUFFER_SIZE/2-1;
    if(textEdit->toPlainText().length() > maxLengh)
    {
        int diff = textEdit->toPlainText().length() - maxLengh;
        QString newStr = textEdit->toPlainText();
        newStr.chop(diff);
        textEdit->setPlainText(newStr);
        QTextCursor cursor(textEdit->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        textEdit->setTextCursor(cursor);
    }
}
