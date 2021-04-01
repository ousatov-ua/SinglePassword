#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/encryptservice.h"
#include <QStringListModel>
#include "addtokendialog.h"
#include <QItemSelection>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(EncryptService *encryptService, QWidget *parent = nullptr);
    ~MainWindow();
    bool addToken(const Token &token, const DecryptedData &decryptedData);
    void removeToken(const Token &token);

private slots:
    void on_addToken__clicked();

    void on_deleteToken__clicked();

    void token_selectionChanged(QItemSelection item);

private:
    Ui::MainWindow *ui;
    EncryptService *encryptService;
    QStringListModel *model= nullptr;
    AddTokenDialog *addTokenDialog = nullptr;

};

#endif // MAINWINDOW_H
