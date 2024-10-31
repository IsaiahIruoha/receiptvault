#include "DashboardPage.h"
#include <QVBoxLayout>
#include <QLabel>

DashboardPage::DashboardPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void DashboardPage::setupUI()
{
    QVBoxLayout *dashboardLayout = new QVBoxLayout(this);

    QLabel *dashboardLabel = new QLabel("Dashboard", this);
    dashboardLabel->setAlignment(Qt::AlignCenter);
    dashboardLabel->setObjectName("titleLabel");

    viewReceiptsButton = new QPushButton("View Receipts", this);
    viewAnalyticsButton = new QPushButton("View Analytics", this);
    viewBudgetsButton = new QPushButton("View Budgets", this);
    logoutButton = new QPushButton("Logout", this);

    dashboardLayout->addWidget(dashboardLabel);
    dashboardLayout->addSpacing(20);
    dashboardLayout->addWidget(viewReceiptsButton);
    dashboardLayout->addWidget(viewAnalyticsButton);
    dashboardLayout->addWidget(viewBudgetsButton);
    dashboardLayout->addStretch();
    dashboardLayout->addWidget(logoutButton);

    // Connect button signals to emit custom signals
    connect(viewReceiptsButton, &QPushButton::clicked, this, &DashboardPage::navigateToReceipts);
    connect(viewAnalyticsButton, &QPushButton::clicked, this, &DashboardPage::navigateToAnalytics);
    connect(viewBudgetsButton, &QPushButton::clicked, this, &DashboardPage::navigateToBudgets);
    connect(logoutButton, &QPushButton::clicked, this, &DashboardPage::logoutRequested);
}
