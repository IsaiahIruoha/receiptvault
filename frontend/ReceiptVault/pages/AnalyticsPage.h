#ifndef ANALYTICSPAGE_H
#define ANALYTICSPAGE_H

#include <QWidget>
#include <QtCharts/QChartView> // handles chart display
#include <QPushButton> // for buttons
#include <QList> // for storing lists of data
#include <QPair> // for storing pairs of values

namespace Ui {
class AnalyticsPage; // forward declaration of ui class
}

class AnalyticsPage : public QWidget
{
    Q_OBJECT // needed for signals and slots

public:
    explicit AnalyticsPage(QWidget *parent = nullptr); // constructor to set up the widget
    ~AnalyticsPage(); // destructor to clean up the widget

    // updates the data for all the charts
    void updateChartData(
        const QList<QPair<QString, double>> &categoryExpenses, // data for category expenses
        const QList<QPair<QString, double>> &monthlySpending, // data for monthly trends
        const QList<QPair<QString, double>> &topStores // data for top stores
        );

signals:
    void navigateToDashboard(); // signal to go back to the dashboard

private:
    Ui::AnalyticsPage *ui; // pointer to ui elements

    void updateSpendingBreakdown(const QList<QPair<QString, double>> &data); // updates the spending breakdown chart
    void updateMonthlyTrend(const QList<QPair<QString, double>> &data); // updates the monthly trend chart
    void updateCategoryComparison(const QList<QPair<QString, double>> &data); // updates the category comparison chart
    void updateTopStores(const QList<QPair<QString, double>> &data); // updates the top stores chart
};

#endif
