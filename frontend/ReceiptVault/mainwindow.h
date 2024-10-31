#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// Include the new page classes
#include "pages/LoginPage.h"
#include "pages/CreateAccountPage.h"
#include "pages/DashboardPage.h"
#include "pages/ReceiptsPage.h"
#include "pages/AnalyticsPage.h"
#include "pages/BudgetsPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin(const QString &username, const QString &password);
    void handleCreateAccount(const QString &email, const QString &username, const QString &password);
    void navigateToDashboard();
    void navigateToLogin();
    void handleUploadReceipt();

private:
    QStackedWidget *stackedWidget;

    // Page instances
    LoginPage *loginPage;
    CreateAccountPage *createAccountPage;
    DashboardPage *dashboardPage;
    ReceiptsPage *receiptsPage;
    AnalyticsPage *analyticsPage;
    BudgetsPage *budgetsPage;

    // Method to apply styles
    void applyStyles();

    // Store current user's username
    QString currentUsername;
};

#endif // MAINWINDOW_H
