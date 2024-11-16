#include "DashboardPage.h"
#include <QVBoxLayout>
#include <QLabel>

// constructor for DashboardPage
DashboardPage::DashboardPage(QWidget *parent) : QWidget(parent)
{
    setupUI(); // sets up the dashboard UI
}

// sets up the UI layout
void DashboardPage::setupUI()
{
    QVBoxLayout *dashboardLayout = new QVBoxLayout(this); // main layout

    // title label
    QLabel *dashboardLabel = new QLabel("Dashboard", this);
    dashboardLabel->setAlignment(Qt::AlignCenter);
    dashboardLabel->setObjectName("titleLabel"); // for styling

    // initialize buttons
    viewReceiptsButton = new QPushButton("View Receipts", this);
    viewAnalyticsButton = new QPushButton("View Analytics", this);
    viewBudgetsButton = new QPushButton("View Budgets", this);
    logoutButton = new QPushButton("Logout", this);

    // add widgets to layout
    dashboardLayout->addWidget(dashboardLabel);
    dashboardLayout->addSpacing(20); // adds spacing between title and buttons
    dashboardLayout->addWidget(viewReceiptsButton);
    dashboardLayout->addWidget(viewAnalyticsButton);
    dashboardLayout->addWidget(viewBudgetsButton);
    dashboardLayout->addStretch(); // pushes logout button to bottom
    dashboardLayout->addWidget(logoutButton);

    // connect button signals to emit custom signals
    connect(viewReceiptsButton, &QPushButton::clicked, this, &DashboardPage::navigateToReceipts);
    connect(viewAnalyticsButton, &QPushButton::clicked, this, &DashboardPage::navigateToAnalytics);
    connect(viewBudgetsButton, &QPushButton::clicked, this, &DashboardPage::navigateToBudgets);
    connect(logoutButton, &QPushButton::clicked, this, &DashboardPage::logoutRequested);
}
