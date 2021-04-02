#include "loginwindow.h"

#include <QApplication>
#include "core/Database.h"
#include "createdbwindow.h"
#include "core/encryptservice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!EncryptService::GetInstance()->initialTokenExists()){
        CreateDbWindow  *createDbWindow = new CreateDbWindow();
        createDbWindow ->show();
    }else{
        LoginWindow *loginWindow = new LoginWindow();
        loginWindow->show();
    }
    return a.exec();
}
