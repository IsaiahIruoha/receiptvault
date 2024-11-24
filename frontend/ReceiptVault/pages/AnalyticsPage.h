#ifndef ANALYTICSPAGE_H
#define ANALYTICSPAGE_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QPushButton>
#include <QList>
#include <QPair>

namespace Ui {
class AnalyticsPage;
}

class AnalyticsPage : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyticsPage(QWidget *parent = nullptr);
    ~AnalyticsPage();

    void updateChartData(
        const QList<QPair<QString, double>> &categoryExpenses,
        const QList<QPair<QString, double>> &monthlySpending,
        const QList<QPair<QString, double>> &topStores
        );

signals:
    void navigateToDashboard();

private:
    Ui::AnalyticsPage *ui;

    void updateSpendingBreakdown(const QList<QPair<QString, double>> &data);
    void updateMonthlyTrend(const QList<QPair<QString, double>> &data);
    void updateCategoryComparison(const QList<QPair<QString, double>> &data);
    void updateTopStores(const QList<QPair<QString, double>> &data);
};

#endif
