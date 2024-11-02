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

    // method to load budgets from the database for a specific user
    void loadBudgets(int userId);

signals:
    void navigateToDashboard();

private slots:
    void addBudget();
    void editBudget();
    void deleteBudget();

private:
    int currentUserId;

    QTableWidget *budgetsTable;
    QPushButton *backToDashboardButton;
    QPushButton *addBudgetButton;
    QPushButton *editBudgetButton;
    QPushButton *deleteBudgetButton;

    QLineEdit *budgetNameEdit;
    QLineEdit *amountEdit;
    QComboBox *categoryComboBox;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;

    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;

    void setupUI();
    void refreshBudgetsTable();
};

#endif
