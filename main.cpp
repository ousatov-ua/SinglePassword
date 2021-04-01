#include "loginwindow.h"

#include <QApplication>
#include "core/Database.h"
#include "createdbwindow.h"
#include "core/encryptservice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EncryptService encService;
    if(!encService.initialTokenExists()){
        CreateDbWindow  *createDbWindow = new CreateDbWindow(&encService);
        createDbWindow ->show();
    }else{
        LoginWindow *loginWindow = new LoginWindow(&encService);
        loginWindow->show();
    }
    return a.exec();
}
