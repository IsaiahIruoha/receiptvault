#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// include the new page classes
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Add the slot for editing receipts
private slots:
    void handleLogin(const QString &username, const QString &password);
    void handleCreateAccount(const QString &username, const QString &password);
    void navigateToDashboard();
    void navigateToLogin();
    void handleUploadReceipt();
    void handleNavigateToAnalytics();
    void handleEditReceipt(int expenseId);

private:
    QStackedWidget *stackedWidget;

    // page instances
    LoginPage *loginPage;
    CreateAccountPage *createAccountPage;
    DashboardPage *dashboardPage;
    ReceiptsPage *receiptsPage;
    AnalyticsPage *analyticsPage;
    BudgetsPage *budgetsPage;

    // apply styling qss
    void applyStyles();

    // store current user's username and user ID
    QString currentUsername;
    int currentUserId;

    // method to get current user ID
    int getCurrentUserId();
};

#endif
