#include "AnalyticsPage.h"
#include "ui_AnalyticsPage.h"
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDebug>

using namespace Qt;

AnalyticsPage::AnalyticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyticsPage)
{
    ui->setupUi(this);

    // Initialize an empty pie chart
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Spending Breakdown");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    ui->chartPlaceholder->setChart(pieChart);
    ui->chartPlaceholder->setRenderHint(QPainter::Antialiasing);

    // Connect back button to signal
    connect(ui->button_BackToDashboard, &QPushButton::clicked, this, &AnalyticsPage::navigateToDashboard);
}

AnalyticsPage::~AnalyticsPage()
{
    delete ui;
}

void AnalyticsPage::updateChartData(const QList<QPair<QString, double>> &data)
{
    if (!ui->chartPlaceholder || !ui->chartPlaceholder->chart()) {
        qDebug() << "Chart view or chart is not initialized.";
        return;
    }

    QChart *chart = ui->chartPlaceholder->chart();

    // Clear existing data
    chart->removeAllSeries();

    // Create and populate a new pie series
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Spending Breakdown");
    for (const auto &pair : data) {
        pieSeries->append(pair.first, pair.second);
    }

    // Update the chart with new data
    chart->addSeries(pieSeries);
    chart->setTitle("Spending Breakdown");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize pie slices
    for (auto slice : pieSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::white));
        slice->setBrush(QBrush(slice->brush().color()));
    }

    // Optionally, adjust the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}
