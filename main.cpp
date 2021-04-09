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
    if(qtTranslator.load(QLocale::system(), "qt_", QString(),
                 QLibraryInfo::path(QLibraryInfo::TranslationsPath))){
        app.installTranslator(&qtTranslator);
    }

    QTranslator myappTranslator;
   // myappTranslator.load(QLocale::system(), "SinglePassword_", QString(), ":/translations");
    bool loaded = myappTranslator.load(QLocale(), QLatin1String("SinglePassword_"), QLatin1String(), QLatin1String(":/translations/i18n"));
    if(loaded){
        app.installTranslator(&myappTranslator);
    }

    if(!EncryptService::GetInstance()->initialTokenExists()){
        CreateDbWindow  *createDbWindow = new CreateDbWindow();
        createDbWindow ->show();
    }else{
        LoginWindow *loginWindow = new LoginWindow();
        loginWindow->show();
    }
    return app.exec();
}
