QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
#CONFIG += staticlib
CONFIG += static


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app/addtokendialog.cpp \
    app/createdbwindow.cpp \
    app/loginwindow.cpp \
    app/mainwindow.cpp \
    core/encryptservice.cpp \
    core/util.cpp \
    main.cpp \
    core/Encryptor.cpp \
    core/Database.cpp

HEADERS += \
    app/addtokendialog.h \
    app/createdbwindow.h \
    app/loginwindow.h \
    app/mainwindow.h \
    core/encryptservice.h \
    core/util.h \
    core/Encryptor.h \
    core/Database.h

FORMS += \
    ui/addtokendialog.ui \
    ui/createdbwindow.ui \
    ui/loginwindow.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    SinglePassword_uk_UA.ts

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/opt/boost/include
INCLUDEPATH += /usr/local/opt/openssl/include

OPENSSL_ROOT_DIR =/usr/local/opt/openssl
OPENSSL_LIBRARIES = /usr/local/opt/openssl/lib
OPENSSL_INCLUDE_DIR = /usr/local/opt/openssl/include/openssl
OPENSSL_USE_STATIC_LIBS = TRUE

Boost_USE_STATIC_LIBS = ON
Boost_USE_MULTITHREADED = ON
Boost_USE_STATIC_RUNTIME = OFF

LIBS += -L/usr/local/lib -L/usr/local/opt/nlohmann-json/lib -L/usr/local/opt/openssl/lib -lboost_serialization -lboost_filesystem -lssl -lcrypto


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
