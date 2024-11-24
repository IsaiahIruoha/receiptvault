#include "AnalyticsPage.h"
#include "ui_AnalyticsPage.h"
#include <QtCharts/QPieSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>
#include <QtCharts/qvalueaxis.h>

using namespace Qt;

AnalyticsPage::AnalyticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyticsPage)
{
    ui->setupUi(this);

    // Initialize the Spending Breakdown Pie Chart
    updateSpendingBreakdown(QList<QPair<QString, double>>());

    // Initialize the Monthly Spending Trend Line Chart
    updateMonthlyTrend(QList<QPair<QString, double>>());

    // Initialize the Category-wise Expense Comparison Bar Chart
    updateCategoryComparison(QList<QPair<QString, double>>());

    // Initialize the Top Stores by Spending Bar Chart
    updateTopStores(QList<QPair<QString, double>>());

    // Connect back button to signal
    connect(ui->button_BackToDashboard, &QPushButton::clicked, this, &AnalyticsPage::navigateToDashboard);
}

AnalyticsPage::~AnalyticsPage()
{
    delete ui;
}

void AnalyticsPage::updateChartData(
    const QList<QPair<QString, double>> &categoryExpenses,
    const QList<QPair<QString, double>> &monthlySpending,
    const QList<QPair<QString, double>> &topStores
    )
{
    updateSpendingBreakdown(categoryExpenses);
    updateMonthlyTrend(monthlySpending);
    updateCategoryComparison(categoryExpenses); // Reusing categoryExpenses
    updateTopStores(topStores);
}

void AnalyticsPage::updateSpendingBreakdown(const QList<QPair<QString, double>> &data)
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

    // Adjust the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void AnalyticsPage::updateMonthlyTrend(const QList<QPair<QString, double>> &data)
{
    if (!ui->chartView_MonthlyTrend || !ui->chartView_MonthlyTrend->chart()) {
        qDebug() << "Monthly Trend chart view or chart is not initialized.";
        return;
    }

    QChart *chart = ui->chartView_MonthlyTrend->chart();

    // Clear existing data
    chart->removeAllSeries();

    // Create a new line series
    QLineSeries *lineSeries = new QLineSeries();
    lineSeries->setName("Monthly Spending");

    QStringList categories;
    for (const auto &pair : data) {
        QString month = pair.first; // Format: YYYY-MM
        double total = pair.second;
        categories.append(month);
        lineSeries->append(categories.size(), total); // X-axis as integer index
    }

    // Create and configure the chart
    chart->addSeries(lineSeries);
    chart->setTitle("Monthly Spending Trend");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount ($)");
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    // Customize the line
    lineSeries->setPointsVisible(true);
    lineSeries->setPen(QPen(Qt::blue, 2));
    lineSeries->setBrush(Qt::NoBrush);

    // Adjust the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void AnalyticsPage::updateCategoryComparison(const QList<QPair<QString, double>> &data)
{
    if (!ui->chartView_CategoryComparison || !ui->chartView_CategoryComparison->chart()) {
        qDebug() << "Category Comparison chart view or chart is not initialized.";
        return;
    }

    QChart *chart = ui->chartView_CategoryComparison->chart();

    // Clear existing data
    chart->removeAllSeries();

    // Create a new bar series
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName("Category Expenses");

    // Create a bar set
    QBarSet *barSet = new QBarSet("Expenses");

    for (const auto &pair : data) {
        *barSet << pair.second;
    }

    barSeries->append(barSet);
    chart->addSeries(barSeries);
    chart->setTitle("Category-wise Expense Comparison");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QStringList categories;
    for (const auto &pair : data) {
        categories.append(pair.first);
    }
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount ($)");
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // Customize the bar set
    barSet->setColor(QColor("#42A5F5")); // Blue color

    // Adjust the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void AnalyticsPage::updateTopStores(const QList<QPair<QString, double>> &data)
{
    if (!ui->chartView_TopStores || !ui->chartView_TopStores->chart()) {
        qDebug() << "Top Stores chart view or chart is not initialized.";
        return;
    }

    QChart *chart = ui->chartView_TopStores->chart();

    // Clear existing data
    chart->removeAllSeries();

    // Create a new bar series
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName("Top Stores");

    // Create a bar set
    QBarSet *barSet = new QBarSet("Spending");

    for (const auto &pair : data) {
        *barSet << pair.second;
    }

    barSeries->append(barSet);
    chart->addSeries(barSeries);
    chart->setTitle("Top Stores by Spending");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QStringList stores;
    for (const auto &pair : data) {
        stores.append(pair.first);
    }
    axisX->append(stores);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount ($)");
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // Customize the bar set
    barSet->setColor(QColor("#66BB6A")); // Green color

    // Adjust the legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}
