#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include "ui_DashboardPage.h" // include the generated UI header

namespace Ui {
class DashboardPage;
}

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage(); // Destructor to clean up

signals:
    void navigateToReceipts(); // signal to go to receipts page
    void navigateToAnalytics(); // signal to go to analytics page
    void navigateToBudgets(); // signal to go to budgets page
    void logoutRequested(); // signal for logout

private:
    Ui::DashboardPage *ui; // Pointer to UI object generated from .ui file
};

#endif
