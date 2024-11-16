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

class BudgetsPage : public QWidget
{
    Q_OBJECT
public:
    explicit BudgetsPage(QWidget *parent = nullptr);

    // loads budgets for the user
    void loadBudgets(int userId);

signals:
    void navigateToDashboard();

private slots:
    void addBudget(); // adds a budget
    void editBudget(); // edits a budget
    void deleteBudget(); // deletes a budget

private:
    int currentUserId; // stores user id

    QTableWidget *budgetsTable; // table for budgets
    QPushButton *backToDashboardButton; // back button
    QPushButton *addBudgetButton; // button to add budget
    QPushButton *editBudgetButton; // button to edit budget
    QPushButton *deleteBudgetButton; // button to delete budget

    QLineEdit *budgetNameEdit; // input for budget name
    QLineEdit *amountEdit; // input for budget amount
    QComboBox *categoryComboBox; // dropdown for category
    QDateEdit *startDateEdit; // start date input
    QDateEdit *endDateEdit; // end date input

    QVBoxLayout *mainLayout; // main layout
    QFormLayout *formLayout; // form layout

    void setupUI(); // sets up the ui
    void refreshBudgetsTable(); // refreshes the table
};

#endif
