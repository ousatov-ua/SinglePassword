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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool addToken(const Token &token, const DecryptedData &decryptedData, Mode mode);
    void removeToken(const Token &token);


private slots:
    void on_addToken__clicked();

    void on_deleteToken__clicked();

    void token_selectionChanged(QItemSelection item);

    void on_editToken__clicked();

    void on_search__textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QStringListModel *model= nullptr;
    AddTokenDialog *addTokenDialog = nullptr;
    void selectionUpdated(const QModelIndex &qmodelIndex);
    void showTokens(const QString *filter = nullptr);

};

#endif // MAINWINDOW_H
