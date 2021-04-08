#include "app/loginwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "core/Database.h"
#include "app/createdbwindow.h"
#include "core/encryptservice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    bool loaded = qtTranslator.load("qt_" + QLocale::system().name(),
              QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    loaded = myappTranslator.load("SinglePassword_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    if(!EncryptService::GetInstance()->initialTokenExists()){
        CreateDbWindow  *createDbWindow = new CreateDbWindow();
        createDbWindow ->show();
    }else{
        LoginWindow *loginWindow = new LoginWindow();
        loginWindow->show();
    }
    return app.exec();
}
