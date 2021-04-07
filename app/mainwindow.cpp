#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "core/util.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    ui->tokensList_->setModel(model);
    ui->tokensList_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tokensList_->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this,
        SLOT(token_selectionChanged(QItemSelection)));

    showTokens();
}

void MainWindow::showTokens(const QString *filter){
    QStringList tokens;
    std::string normalizedFilter;
    if(filter!=nullptr){
        normalizedFilter = filter->trimmed().toLower().toStdString();
    }
    bool doFilter = filter!=nullptr && normalizedFilter.size() !=0;
    foreach(const Token token, EncryptService::GetInstance()->getTokens()){
        if(token.plain){
            continue;
        }
        DecryptedData decryptedData = EncryptService::GetInstance()->decryptToken(token);
        const char* token_cstr = (char*)decryptedData.data;
        const std::string tokenValueLow = QString(token_cstr).toLower().toStdString();
        if(doFilter && (tokenValueLow.find(normalizedFilter)==std::string::npos)){
            continue;
        }
        tokens << token_cstr;
    }
    model->setStringList(tokens);
}

MainWindow::~MainWindow()
{
    delete model;
    if(addTokenDialog !=nullptr){
        delete addTokenDialog;
    }
    delete ui;

}

bool MainWindow::addToken(const Token &token, const DecryptedData &decryptedData, Mode mode){
    SaveResult saveResult = EncryptService::GetInstance()->saveTokenData(token, decryptedData);
    if(mode == CREATE && saveResult == SAVE_SUCCESS){
        if(model->insertRow(model->rowCount())) {
            QModelIndex index = model->index(model->rowCount() - 1, 0);
            DecryptedData decryptedData = EncryptService::GetInstance()->decryptToken(token);
            model->setData(index, QString((char*)decryptedData.data));
        }
    }
    if(mode == EDIT){
        auto indexes = ui->tokensList_->selectionModel()->selectedIndexes();
        if(!indexes.isEmpty()){
            selectionUpdated(indexes.first());
        }
    }
    return saveResult == SAVE_SUCCESS;
}

void MainWindow::removeToken(const Token &token){

    SaveResult result = EncryptService::GetInstance()->removeToken(token);
    if(result == SAVE_SUCCESS){
        QModelIndex index = ui->tokensList_->selectionModel()->selectedIndexes().at(0);
        model->removeRow(index.row());
        this->ui->tokenValue_->document()->clear();
    }else{
        QMessageBox::information(this, "Remove token", "Cannot remove token");
    }
}

void MainWindow::on_addToken__clicked()
{
    if(addTokenDialog!= nullptr){
        addTokenDialog->setMode(CREATE);

    }else{
        addTokenDialog = new AddTokenDialog(this);
    }
    addTokenDialog->show();
}

void MainWindow::on_deleteToken__clicked()
{
    auto indexes = ui->tokensList_->selectionModel()->selectedIndexes();
    if(!indexes.isEmpty()){

        if (QMessageBox::Yes != QMessageBox::question(this, "Remove token", "Remove Token? This cannot be undone", QMessageBox::Yes | QMessageBox::No))
        {
            return;

        }
        auto index = indexes.first();

        const std::string tokenValue = index.data(Qt::DisplayRole).toString().toStdString();
        Token token{};
        Util::toToken(tokenValue, token);
        removeToken(token);
        ui->tokensList_->clearSelection();
    }else{
        QMessageBox::information(this, "Remove token", "Please select a token");
    }
}

void MainWindow::selectionUpdated(const QModelIndex &index)
{
    const std::string tokenValue = index.data(Qt::DisplayRole).toString().toStdString();
    Token token{};
    Util::toToken(tokenValue, token);
    DecryptedData decryptedData = EncryptService::GetInstance()->decryptData(token);
    const auto value = QString(Util::toStdString(decryptedData).c_str());
    ui->tokenValue_->document()->setPlainText(value);
}

void MainWindow::token_selectionChanged(QItemSelection itemSelection){
    if(!itemSelection.indexes().isEmpty()){
        const auto index = itemSelection.indexes().first();
        selectionUpdated(index);
    }
}

void MainWindow::on_editToken__clicked()
{
    auto indexes = ui->tokensList_->selectionModel()->selectedIndexes();
    if(indexes.isEmpty()){
        QMessageBox::information(this, "Edit token", "Please select a token");
        return;
    }
    auto index = indexes.first();
    const std::string tokenValue = index.data(Qt::DisplayRole).toString().toStdString();
    Token outToken{};
    Util::toToken(tokenValue, outToken);
    DecryptedData decryptedData = EncryptService::GetInstance()->decryptData(outToken);
    if(addTokenDialog == nullptr){
        addTokenDialog = new AddTokenDialog(this);
    }

    addTokenDialog->setData(outToken, decryptedData);
    addTokenDialog->setMode(EDIT);
    addTokenDialog->show();

}

void MainWindow::on_search__textChanged(const QString &text)
{
    showTokens(&text);
}
