#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include <QWidget>
#include "pages/LoginPage.h"
#include "pages/CreateAccountPage.h"
#include "pages/DashboardPage.h"
#include "pages/ReceiptsPage.h"
#include "pages/AnalyticsPage.h"
#include "pages/BudgetsPage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr); // constructor
    ~MainWindow(); // destructor

private slots:
    void handleLogin(const QString &username, const QString &password); // process login
    void handleCreateAccount(const QString &username, const QString &password); // process account creation
    void navigateToDashboard(); // switch to dashboard
    void navigateToLogin(); // switch to login
    void handleUploadReceipt(); // handle receipt upload
    void handleNavigateToAnalytics(); // go to analytics
    void handleEditReceipt(int expenseId); // handle receipt editing
    void toggleTheme(); // toggle dark/light mode

private:
    QStackedWidget *stackedWidget; // manages the page stack

    // page instances
    LoginPage *loginPage;
    CreateAccountPage *createAccountPage;
    DashboardPage *dashboardPage;
    ReceiptsPage *receiptsPage;
    AnalyticsPage *analyticsPage;
    BudgetsPage *budgetsPage;

    void applyStyles(bool darkMode); // apply external styles
    void clearInlineStyles(QWidget* widget); // clear styles for dark mode
    void restoreInlineStyles(QWidget* widget); // restore styles for light mode
    QMap<QWidget*, QString> originalStyles; // store original styles for light mode
    int getCurrentUserId(); // get current user ID

    QString currentUsername; // store current username
    int currentUserId; // store current user ID

    // dark mode
    bool toggleDarkMode;
};

#endif
