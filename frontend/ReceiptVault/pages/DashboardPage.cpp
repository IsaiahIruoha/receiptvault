#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QMap>

using namespace Qt;

// Constructor
DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::DashboardPage), currentUserId(-1) // Initialize the UI and set user ID to -1
{
    ui->setupUi(this); // Sets up the UI from the .ui file

    // Correct button connections
    connect(ui->viewReceiptsButton, &QPushButton::clicked, this, &DashboardPage::navigateToReceipts);
    connect(ui->viewAnalyticsButton, &QPushButton::clicked, this, &DashboardPage::navigateToAnalytics);
    connect(ui->viewBudgetsButton, &QPushButton::clicked, this, &DashboardPage::navigateToBudgets);
    connect(ui->logoutButton, &QPushButton::clicked, this, &DashboardPage::logoutRequested);
}

// Destructor
DashboardPage::~DashboardPage()
{
    delete ui; // Free the UI object
}
void DashboardPage::updateDashboard(int totalReceipts, double totalSpending,double avgMonthlySpending, QString topCategory, const QList<QPair<QString, double>> &spendingData)
{
    // Update labels
    ui->label_TotalReceipts->setText(QString("Total Receipts:\n\n%1").arg(totalReceipts));
    ui->label_TotalSpending->setText(QString("Total Spending:\n\n$%1").arg(totalSpending, 0, 'f', 2));
    ui->label_TopCategory->setText(QString("Top Category:\n\n%1").arg(topCategory));
    ui->label_AvgMonthlySpending->setText(QString("Avg Monthly Spending:\n\n$%1").arg(avgMonthlySpending, 0, 'f', 2));

    // Create and populate a pie series
    QPieSeries *series = new QPieSeries();
    for (const auto &data : spendingData) {
        series->append(data.first, data.second);
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Spending Breakdown");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize pie slices
    for (auto slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::white));
        slice->setBrush(QBrush(slice->brush().color()));
    }

    // Configure legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Set the chart to the view
    ui->spendingChartView->setChart(chart);
}

