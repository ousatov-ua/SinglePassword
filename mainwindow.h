#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/encryptservice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(EncryptService *encryptService, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    EncryptService *enctyptService;
};

#endif // MAINWINDOW_H
