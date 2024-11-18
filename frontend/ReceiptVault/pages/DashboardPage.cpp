#include "DashboardPage.h"

// constructor for DashboardPage
DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::DashboardPage) // initialize the ui object
{
    ui->setupUi(this); // sets up the UI from the .ui file

    // connect button signals to emit custom signals
    connect(ui->viewReceiptsButton, &QPushButton::clicked, this, &DashboardPage::navigateToReceipts);
    connect(ui->viewAnalyticsButton, &QPushButton::clicked, this, &DashboardPage::navigateToAnalytics);
    connect(ui->viewBudgetsButton, &QPushButton::clicked, this, &DashboardPage::navigateToBudgets);
    connect(ui->logoutButton, &QPushButton::clicked, this, &DashboardPage::logoutRequested);
}

// destructor for DashboardPage
DashboardPage::~DashboardPage()
{
    delete ui; // free the ui object
}
