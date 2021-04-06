#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    foreach(const Token token,EncryptService::GetInstance()->getTokens()){
        const char* token_cstr = token.data.c_str();
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
    SaveResult saveResult = EncryptService::GetInstance()->encrypt(token, decryptedData);
    if(mode == CREATE && saveResult == SAVE_SUCCESS){
        if(model->insertRow(model->rowCount())) {
            QModelIndex index = model->index(model->rowCount() - 1, 0);
            model->setData(index, QString(token.data.c_str()));
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
        auto index = indexes.first();
        Token token { .data = index.data(Qt::DisplayRole).toString().toStdString() };
        removeToken(token);
        ui->tokensList_->clearSelection();
    }
}

void MainWindow::selectionUpdated(const QModelIndex &index)
{
    Token token { .data = index.data(Qt::DisplayRole).toString().toStdString() };
    DecryptedData decryptedData{};
    EncryptService::GetInstance()->decrypt(token, decryptedData);
    const auto value = QString(EncryptService::GetInstance()->toStdString(decryptedData).c_str());
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
        QMessageBox::information(this, "Edit token", "Please select token");
        return;
    }
    auto index = indexes.first();
    Token token { .data = index.data(Qt::DisplayRole).toString().toStdString() };
    DecryptedData decryptedData{};
    EncryptService::GetInstance()->decrypt(token, decryptedData);
    if(addTokenDialog == nullptr){
        addTokenDialog = new AddTokenDialog(this);
    }

    addTokenDialog->setData(token, decryptedData);
    addTokenDialog->setMode(EDIT);
    addTokenDialog->show();

}

void MainWindow::on_search__textChanged(const QString &text)
{
    showTokens(&text);
}