QT += core gui widgets charts sql

CONFIG += sdk_no_version_check
CONFIG += c++17

# Source Files
SOURCES += main.cpp \
           mainwindow.cpp \
           pages/DatabaseManager.cpp \
           pages/LoginPage.cpp \
           pages/CreateAccountPage.cpp \
           pages/DashboardPage.cpp \
           pages/ReceiptsPage.cpp \
           pages/AnalyticsPage.cpp \
           pages/BudgetsPage.cpp

# Header Files
HEADERS += mainwindow.h \
           pages/DatabaseManager.h \
           pages/LoginPage.h \
           pages/CreateAccountPage.h \
           pages/DashboardPage.h \
           pages/ReceiptsPage.h \
           pages/AnalyticsPage.h \
           pages/BudgetsPage.h

# Resource Files
RESOURCES += resources.qrc

# Translation Files
TRANSLATIONS += ReceiptVault_en_CA.ts

# Configuration for translations
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    AnalyticsPage.ui \
    CreateAccountPage.ui \
    DashboardPage.ui \
    LoginPage.ui
