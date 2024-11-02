#ifndef ANALYTICSPAGE_H
#define ANALYTICSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QList>
#include <QPair>

class AnalyticsPage : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyticsPage(QWidget *parent = nullptr);

    // method to update the chart with new data
    void updateChartData(const QList<QPair<QString, double>> &data);

signals:
    void navigateToDashboard();

private:
    QChartView *pieChartView;
    QPushButton *backToDashboardButton;

    void setupUI();
};

#endif
