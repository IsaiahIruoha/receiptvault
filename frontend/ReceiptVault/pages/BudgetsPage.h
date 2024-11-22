#ifndef BUDGETSPAGE_H
#define BUDGETSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class BudgetsPage;
}

class BudgetsPage : public QWidget
{
    Q_OBJECT
public:
    explicit BudgetsPage(QWidget *parent = nullptr);
    ~BudgetsPage();

    // Loads budgets for the user
    void loadBudgets(int userId);

signals:
    void navigateToDashboard();

private slots:
    void addBudget();    // Adds a budget
    void editBudget();   // Edits a budget
    void deleteBudget(); // Deletes a budget

private:
    int currentUserId; // Stores user id

    Ui::BudgetsPage *ui; // Pointer to UI object
    void refreshBudgetsTable(); // Refreshes the table
};

#endif
