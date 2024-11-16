#include "AnalyticsPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDebug>

using namespace Qt;

AnalyticsPage::AnalyticsPage(QWidget *parent) : QWidget(parent)
{
    setupUI(); // setup the ui for the analytics page
}

void AnalyticsPage::setupUI()
{
    QVBoxLayout *analyticsLayout = new QVBoxLayout(this);

    // add the title label
    QLabel *analyticsLabel = new QLabel("Analytics", this);
    analyticsLabel->setAlignment(Qt::AlignCenter);
    analyticsLabel->setObjectName("titleLabel");
    analyticsLayout->addWidget(analyticsLabel);

    // initialize an empty pie chart
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");

    // create and configure the chart
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Spending Breakdown");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    // create the chart view
    pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    analyticsLayout->addWidget(pieChartView);

    // add the back button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    analyticsLayout->addWidget(backToDashboardButton);

    // connect back button to signal
    connect(backToDashboardButton, &QPushButton::clicked, this, &AnalyticsPage::navigateToDashboard);
}

void AnalyticsPage::updateChartData(const QList<QPair<QString, double>> &data)
{
    // validate the chart view
    if (!pieChartView || !pieChartView->chart()) {
        qDebug() << "chart view or chart is not initialized.";
        return;
    }

    QChart *chart = pieChartView->chart();

    // clear existing data
    chart->removeAllSeries();

    // create and populate a new pie series
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");
    for (const auto &pair : data) {
        pieSeries->append(pair.first, pair.second);
    }

    // update the chart with new data
    chart->addSeries(pieSeries);
    chart->setTitle("Spending Breakdown");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // customize pie slices
    for (auto slice : pieSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::white));
        slice->setBrush(QBrush(slice->brush().color()));
    }
}
