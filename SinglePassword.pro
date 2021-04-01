QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addtokendialog.cpp \
    core/encryptservice.cpp \
    core/util.cpp \
    createdbwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    core/Encryptor.cpp \
    core/Database.cpp

HEADERS += \
    addtokendialog.h \
    core/encryptservice.h \
    core/util.h \
    createdbwindow.h \
    loginwindow.h \
    mainwindow.h \
    core/Encryptor.h \
    core/Database.h

FORMS += \
    addtokendialog.ui \
    createdbwindow.ui \
    loginwindow.ui \
    mainwindow.ui

TRANSLATIONS += \
    SinglePassword_uk_UA.ts

PKGCONFIG += openssl
#PKGCONFIG += boost_serialization
#PKGCONFIG += boost_archive

INCLUDEPATH += /usr/local/opt/boost/include
INCLUDEPATH += /usr/local/opt/openssl/include

OPENSSL_ROOT_DIR =/usr/local/opt/openssl
OPENSSL_LIBRARIES = /usr/local/opt/openssl/lib
OPENSSL_INCLUDE_DIR = /usr/local/opt/openssl/include/openssl
OPENSSL_USE_STATIC_LIBS = TRUE

#DEFINES = BOOST_ALL_NO_LIB
Boost_USE_STATIC_LIBS = OFF
Boost_USE_MULTITHREADED = ON
Boost_USE_STATIC_RUNTIME = OFF

LIBS += -L/usr/local/lib -L/usr/local/opt/openssl/lib -lboost_serialization -lboost_filesystem -lssl -lcrypto

# MACOSX_DEPLOYMENT_TARGET=10.14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
