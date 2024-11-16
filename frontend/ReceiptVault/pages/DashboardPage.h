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
    void navigateToReceipts(); // signal to go to receipts page
    void navigateToAnalytics(); // signal to go to analytics page
    void navigateToBudgets(); // signal to go to budgets page
    void logoutRequested(); // signal for logout

private:
    QPushButton *viewReceiptsButton; // button to view receipts
    QPushButton *viewAnalyticsButton; // button to view analytics
    QPushButton *viewBudgetsButton; // button to view budgets
    QPushButton *logoutButton; // button to log out

    void setupUI(); // sets up the ui
};

#endif
