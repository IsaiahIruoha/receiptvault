#ifndef ANALYTICSPAGE_H
#define ANALYTICSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QList>
#include <QPair>

// handles the charts for users
class AnalyticsPage : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyticsPage(QWidget *parent = nullptr);

    // updates the chart with new data
    void updateChartData(const QList<QPair<QString, double>> &data);

signals:
    void navigateToDashboard();

private:
    QChartView *pieChartView; // shows the chart
    QPushButton *backToDashboardButton; // back button

    // sets up the ui
    void setupUI();
};

#endif
