#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class DashboardPage;
}

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage(); // Destructor to clean up

    // Method to update dashboard information
    void updateDashboard(int totalReceipts, double totalSpending,double avgMonthlySpending, QString topCategory, const QList<QPair<QString, double>> &spendingData);

    Ui::DashboardPage *ui; // Pointer to UI object generated from .ui file

signals:
    void navigateToReceipts();   // Signal to go to receipts page
    void navigateToAnalytics();  // Signal to go to analytics page
    void navigateToBudgets();    // Signal to go to budgets page
    void logoutRequested();      // Signal for logout

private:
    // New method to update spending frequency heatmap
    void updateSpendingFrequencyHeatmap();

    // Current User ID
    int currentUserId;

public:
    // Setter for currentUserId
    void setCurrentUserId(int userId) { currentUserId = userId; }

};

#endif // DASHBOARDPAGE_H
