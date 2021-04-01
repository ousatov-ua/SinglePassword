#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "core/encryptservice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(EncryptService *encryptService, QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_DecryptButton_clicked();

    void on_masterPass_returnPressed();

private:
    Ui::LoginWindow *ui;
    MainWindow *mainWindow;
    EncryptService *encryptService;

    void decrypt();
};
#endif // LOGINWINDOW_H
