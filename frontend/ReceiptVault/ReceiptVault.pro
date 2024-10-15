QT += core gui widgets charts sql

CONFIG += sdk_no_version_check
CONFIG += c++17

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

RESOURCES += resources.qrc

TRANSLATIONS += ReceiptVault_en_CA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
