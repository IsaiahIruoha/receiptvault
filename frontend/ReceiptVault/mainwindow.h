#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
};

private slots:
    void handleLogin();
    void handleCreateAccount();
    void navigateToDashboard();
    void navigateToLogin();

private:
    QStackedWidget *stackedWidget;

    // Pages
    QWidget *loginPage;
    QWidget *createAccountPage;
    QWidget *dashboardPage;
    QWidget *receiptsPage;
    QWidget *analyticsPage;
    QWidget *budgetsPage;

    // Login Page Widgets
    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;
    QPushButton *loginButton;
    QPushButton *toCreateAccountButton;

    // Create Account Page Widgets
    QLineEdit *createUsernameEdit;
    QLineEdit *createPasswordEdit;
    QLineEdit *createConfirmPasswordEdit;
    QPushButton *createAccountButton;
    QPushButton *toLoginButton;

    // Dashboard Page Widgets
    QPushButton *viewReceiptsButton;
    QPushButton *viewAnalyticsButton;
    QPushButton *viewBudgetsButton;
    QPushButton *logoutButton;

    // Receipts Page Widgets
    QTableWidget *receiptsTable;
    QPushButton *backToDashboardButtonReceipts;

    // Analytics Page Widgets
    QChartView *pieChartView;
    QPushButton *backToDashboardButtonAnalytics;

    // Budgets Page Widgets
    QPushButton *backToDashboardButtonBudgets;

    // Methods to set up each page
    void setupLoginPage();
    void setupCreateAccountPage();
    void setupDashboardPage();
    void setupReceiptsPage();
    void setupAnalyticsPage();
    void setupBudgetsPage();

    // Method to apply styles
    void applyStyles();

    // Store current user's username
    QString currentUsername;
};

#endif // MAINWINDOW_H
