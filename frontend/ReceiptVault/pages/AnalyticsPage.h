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

    void updateChartData(const QList<QPair<QString, double>> &data);

signals:
    void navigateToDashboard();

private:
    Ui::AnalyticsPage *ui;
};

#endif // ANALYTICSPAGE_H
