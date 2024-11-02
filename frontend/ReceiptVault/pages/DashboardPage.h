#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QPushButton>

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);

signals:
    void navigateToReceipts();
    void navigateToAnalytics();
    void navigateToBudgets();
    void logoutRequested();

private:
    QPushButton *viewReceiptsButton;
    QPushButton *viewAnalyticsButton;
    QPushButton *viewBudgetsButton;
    QPushButton *logoutButton;

    void setupUI();
};

#endif
