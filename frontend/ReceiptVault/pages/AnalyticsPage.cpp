#include "AnalyticsPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QtCharts/QPieSeries>   // Added include
#include <QtCharts/QChart>       // Added include
#include <QtCharts/QChartView>   // Added include

using namespace Qt;

AnalyticsPage::AnalyticsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void AnalyticsPage::setupUI()
{
    QVBoxLayout *analyticsLayout = new QVBoxLayout(this);

    QLabel *analyticsLabel = new QLabel("Analytics", this);
    analyticsLabel->setAlignment(Qt::AlignCenter);
    analyticsLabel->setObjectName("titleLabel");

    // Initialize a pie series for the chart with mock data
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Groceries", 150.50);
    pieSeries->append("Entertainment", 75.00);
    pieSeries->append("Utilities", 120.00);

    // Create a chart and add the pie series to it
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Spending Breakdown");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    // Create a chart view to display the chart
    pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    // Create a button to go back to the dashboard
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    analyticsLayout->addWidget(analyticsLabel);
    analyticsLayout->addWidget(pieChartView);
    analyticsLayout->addWidget(backToDashboardButton);

    // Connect button signals to emit custom signals
    connect(backToDashboardButton, &QPushButton::clicked, this, &AnalyticsPage::navigateToDashboard);
}

void AnalyticsPage::updateChartData(const QList<QPair<QString, double>> &data)
{
    // Clear existing series
    pieChartView->chart()->removeAllSeries();

    // Create a new pie series
    QPieSeries *pieSeries = new QPieSeries();
    for (const auto &pair : data) {
        pieSeries->append(pair.first, pair.second);
    }

    // Add series to chart
    pieChartView->chart()->addSeries(pieSeries);
    pieChartView->chart()->setTitle("Spending Breakdown");
    pieChartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
}
