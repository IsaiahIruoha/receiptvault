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
    setupUI();
}

void AnalyticsPage::setupUI()
{
    QVBoxLayout *analyticsLayout = new QVBoxLayout(this);

    // title label
    QLabel *analyticsLabel = new QLabel("Analytics", this);
    analyticsLabel->setAlignment(Qt::AlignCenter);
    analyticsLabel->setObjectName("titleLabel");
    analyticsLayout->addWidget(analyticsLabel);

    // initialize pie series with no data initially
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");

    // create chart and chartView
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Spending Breakdown");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    analyticsLayout->addWidget(pieChartView);

    // back to dash
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    analyticsLayout->addWidget(backToDashboardButton);

    // connect signal
    connect(backToDashboardButton, &QPushButton::clicked, this, &AnalyticsPage::navigateToDashboard);
}

void AnalyticsPage::updateChartData(const QList<QPair<QString, double>> &data)
{
    if (!pieChartView || !pieChartView->chart()) {
        qDebug() << "chart view or chart is not initialized.";
        return;
    }

    QChart *chart = pieChartView->chart();

    // remove existing series
    chart->removeAllSeries();

    // create a new pie series
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");

    // pPopulate the pie series with data
    for (const auto &pair : data) {
        pieSeries->append(pair.first, pair.second);
    }

    // add the new series to the chart
    chart->addSeries(pieSeries);
    chart->setTitle("Spending Breakdown");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // customize the slice options
    for (auto slice : pieSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::white));
        slice->setBrush(QBrush(slice->brush().color()));
    }
}
