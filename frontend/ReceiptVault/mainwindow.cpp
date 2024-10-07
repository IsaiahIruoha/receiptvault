#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QHeaderView>

using namespace Qt;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ReceiptVault - Budget Tracking App");
    resize(1000, 600);

    QStackedWidget *stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Login Page
    QWidget *loginPage = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout;
    QLabel *loginLabel = new QLabel("Login", loginPage);
    QLineEdit *usernameEdit = new QLineEdit(loginPage);
    usernameEdit->setPlaceholderText("Username");
    QLineEdit *passwordEdit = new QLineEdit(loginPage);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *loginButton = new QPushButton("Login", loginPage);
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(usernameEdit);
    loginLayout->addWidget(passwordEdit);
    loginLayout->addWidget(loginButton);
    loginPage->setLayout(loginLayout);

    // Dashboard Page
    QWidget *dashboardPage = new QWidget(this);
    QVBoxLayout *dashboardLayout = new QVBoxLayout;
    QLabel *dashboardLabel = new QLabel("Dashboard", dashboardPage);
    QPushButton *viewReceiptsButton = new QPushButton("View Receipts", dashboardPage);
    QPushButton *viewAnalyticsButton = new QPushButton("View Analytics", dashboardPage);
    QPushButton *viewBudgetsButton = new QPushButton("View Budgets", dashboardPage);
    dashboardLayout->addWidget(dashboardLabel);
    dashboardLayout->addWidget(viewReceiptsButton);
    dashboardLayout->addWidget(viewAnalyticsButton);
    dashboardLayout->addWidget(viewBudgetsButton);
    dashboardPage->setLayout(dashboardLayout);

    // Receipts Page
    QWidget *receiptsPage = new QWidget(this);
    QVBoxLayout *receiptsLayout = new QVBoxLayout;
    QLabel *receiptsLabel = new QLabel("Receipts", receiptsPage);
    QTableWidget *receiptsTable = new QTableWidget(10, 3, receiptsPage);
    receiptsTable->setHorizontalHeaderLabels({"Store", "Items", "Total"});
    receiptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    receiptsLayout->addWidget(receiptsLabel);
    receiptsLayout->addWidget(receiptsTable);
    receiptsPage->setLayout(receiptsLayout);

    // Analytics Page
    QWidget *analyticsPage = new QWidget(this);
    QVBoxLayout *analyticsLayout = new QVBoxLayout;
    QLabel *analyticsLabel = new QLabel("Analytics", analyticsPage);
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Groceries", 40);
    pieSeries->append("Rent", 30);
    pieSeries->append("Entertainment", 20);
    pieSeries->append("Other", 10);
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Spending Breakdown");
    QChartView *pieChartView = new QChartView(pieChart, analyticsPage);
    analyticsLayout->addWidget(analyticsLabel);
    analyticsLayout->addWidget(pieChartView);
    analyticsPage->setLayout(analyticsLayout);

    // Budgets Page
    QWidget *budgetsPage = new QWidget(this);
    QVBoxLayout *budgetsLayout = new QVBoxLayout;
    QLabel *budgetsLabel = new QLabel("Budgets", budgetsPage);
    budgetsLayout->addWidget(budgetsLabel);
    // TODO: Add budget setting widgets
    budgetsPage->setLayout(budgetsLayout);

    // Add pages to stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(receiptsPage);
    stackedWidget->addWidget(analyticsPage);
    stackedWidget->addWidget(budgetsPage);

    // Connections
    connect(loginButton, &QPushButton::clicked, [stackedWidget, dashboardPage]() {
        stackedWidget->setCurrentWidget(dashboardPage);
    });
    connect(viewReceiptsButton, &QPushButton::clicked, [stackedWidget, receiptsPage]() {
        stackedWidget->setCurrentWidget(receiptsPage);
    });
    connect(viewAnalyticsButton, &QPushButton::clicked, [stackedWidget, analyticsPage]() {
        stackedWidget->setCurrentWidget(analyticsPage);
    });
    connect(viewBudgetsButton, &QPushButton::clicked, [stackedWidget, budgetsPage]() {
        stackedWidget->setCurrentWidget(budgetsPage);
    });
}

