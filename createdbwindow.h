#ifndef CREATEDBWINDOW_H
#define CREATEDBWINDOW_H

#include <QMainWindow>
#include "core/Database.h"
#include "loginwindow.h"
#include "core/encryptservice.h"

namespace Ui {
class CreateDbWindow;
}

class CreateDbWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateDbWindow(QWidget *parent = nullptr);
    ~CreateDbWindow();

private slots:
    void on_create__clicked();

private:
    Ui::CreateDbWindow *ui;
    LoginWindow *loginWindow;
};

#endif // CREATEDBWINDOW_H
