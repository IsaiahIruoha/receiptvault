#include "BudgetsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDebug>
#include "DatabaseManager.h"

// constructor for BudgetsPage
BudgetsPage::BudgetsPage(QWidget *parent) : QWidget(parent), currentUserId(-1)
{
    setupUI(); // set up the UI
}

// sets up the page UI
void BudgetsPage::setupUI()
{
    // main layout for the page
    mainLayout = new QVBoxLayout(this);

    // header layout with title and action buttons
    QHBoxLayout *headerLayout = new QHBoxLayout;

    QLabel *budgetsLabel = new QLabel("Budgets", this);
    budgetsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    budgetsLabel->setObjectName("titleLabel");

    // initialize buttons
    addBudgetButton = new QPushButton("Add Budget", this);
    editBudgetButton = new QPushButton("Edit Budget", this);
    deleteBudgetButton = new QPushButton("Delete Budget", this);
    backToDashboardButton = new QPushButton("Back to Dashboard", this);

    // add widgets to header layout
    headerLayout->addWidget(budgetsLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(addBudgetButton);
    headerLayout->addWidget(editBudgetButton);
    headerLayout->addWidget(deleteBudgetButton);
    headerLayout->addWidget(backToDashboardButton);

    // add header layout to main layout
    mainLayout->addLayout(headerLayout);

    // initialize the table to display budgets
    budgetsTable = new QTableWidget(this);
    budgetsTable->setColumnCount(5);
    budgetsTable->setHorizontalHeaderLabels({"Budget ID", "Category", "Amount", "Start Date", "End Date"});
    budgetsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    budgetsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    budgetsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    budgetsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // add table to main layout
    mainLayout->addWidget(budgetsTable);

    // connect signals to slots
    connect(backToDashboardButton, &QPushButton::clicked, this, &BudgetsPage::navigateToDashboard);
    connect(addBudgetButton, &QPushButton::clicked, this, &BudgetsPage::addBudget);
    connect(editBudgetButton, &QPushButton::clicked, this, &BudgetsPage::editBudget);
    connect(deleteBudgetButton, &QPushButton::clicked, this, &BudgetsPage::deleteBudget);
}

// loads budgets from the database for the user
void BudgetsPage::loadBudgets(int userId)
{
    currentUserId = userId;
    budgetsTable->setRowCount(0); // clear existing rows

    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare(R"(
        SELECT
            budgets.budget_id,
            expense_category.category_name,
            budgets.budget_amount,
            budgets.start_date,
            budgets.end_date
        FROM budgets
        JOIN expense_category ON budgets.category_id = expense_category.category_id
        WHERE budgets.user_id = :user_id
    )");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        while (query.next()) {
            int budgetId = query.value("budget_id").toInt();
            QString category = query.value("category_name").toString();
            double amount = query.value("budget_amount").toDouble();
            QString startDate = query.value("start_date").toString();
            QString endDate = query.value("end_date").toString();

            int currentRow = budgetsTable->rowCount();
            budgetsTable->insertRow(currentRow);
            budgetsTable->setItem(currentRow, 0, new QTableWidgetItem(QString::number(budgetId)));
            budgetsTable->setItem(currentRow, 1, new QTableWidgetItem(category));
            budgetsTable->setItem(currentRow, 2, new QTableWidgetItem(QString::number(amount, 'f', 2)));
            budgetsTable->setItem(currentRow, 3, new QTableWidgetItem(startDate));
            budgetsTable->setItem(currentRow, 4, new QTableWidgetItem(endDate));
        }
    } else {
        qDebug() << "Failed to load budgets:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to load budgets from the database.");
    }
}

// handles adding a new budget
void BudgetsPage::addBudget()
{
    // dialog to input budget details
    QDialog dialog(this);
    dialog.setWindowTitle("Add Budget");
    dialog.setModal(true);

    QFormLayout formLayout(&dialog);

    // category dropdown
    QComboBox *categoryComboBox = new QComboBox(&dialog);
    QSqlQuery categoryQuery(DatabaseManager::instance().getDatabase());
    categoryQuery.prepare("SELECT category_id, category_name FROM expense_category");
    if (categoryQuery.exec()) {
        while (categoryQuery.next()) {
            int categoryId = categoryQuery.value("category_id").toInt();
            QString categoryName = categoryQuery.value("category_name").toString();
            categoryComboBox->addItem(categoryName, categoryId);
        }
    } else {
        qDebug() << "Failed to load categories:" << categoryQuery.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to load categories.");
        return;
    }
    formLayout.addRow("Category:", categoryComboBox);

    // other inputs
    QLineEdit *amountEdit = new QLineEdit(&dialog);
    amountEdit->setPlaceholderText("Enter budget amount");
    formLayout.addRow("Amount:", amountEdit);

    QDateEdit *startDateEdit = new QDateEdit(&dialog);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDate(QDate::currentDate());
    formLayout.addRow("Start Date:", startDateEdit);

    QDateEdit *endDateEdit = new QDateEdit(&dialog);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDate(QDate::currentDate().addMonths(1));
    formLayout.addRow("End Date:", endDateEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    formLayout.addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // process dialog result
    if (dialog.exec() == QDialog::Accepted) {
        QString amountText = amountEdit->text().trimmed();
        if (amountText.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Enter a budget amount.");
            return;
        }

        bool ok;
        double amount = amountEdit->text().toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(this, "Input Error", "Enter a valid amount.");
            return;
        }

        int categoryId = categoryComboBox->currentData().toInt();
        QString startDate = startDateEdit->date().toString("yyyy-MM-dd");
        QString endDate = endDateEdit->date().toString("yyyy-MM-dd");

        // add to database
        QSqlQuery insertQuery(DatabaseManager::instance().getDatabase());
        insertQuery.prepare(R"(
            INSERT INTO budgets (user_id, category_id, budget_amount, start_date, end_date)
            VALUES (:user_id, :category_id, :budget_amount, :start_date, :end_date)
        )");
        insertQuery.bindValue(":user_id", currentUserId);
        insertQuery.bindValue(":category_id", categoryId);
        insertQuery.bindValue(":budget_amount", amount);
        insertQuery.bindValue(":start_date", startDate);
        insertQuery.bindValue(":end_date", endDate);

        if (insertQuery.exec()) {
            QMessageBox::information(this, "Success", "Budget added successfully!");
            loadBudgets(currentUserId);
        } else {
            qDebug() << "Failed to add budget:" << insertQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to add budget.");
        }
    }
}

// handles editing a budget
void BudgetsPage::editBudget()
{
    QList<QTableWidgetItem*> selectedItems = budgetsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Select a budget to edit.");
        return;
    }

    int selectedRow = budgetsTable->row(selectedItems.first());
    int budgetId = budgetsTable->item(selectedRow, 0)->text().toInt();
    QString currentCategory = budgetsTable->item(selectedRow, 1)->text();
    double currentAmount = budgetsTable->item(selectedRow, 2)->text().toDouble();
    QString currentStartDate = budgetsTable->item(selectedRow, 3)->text();
    QString currentEndDate = budgetsTable->item(selectedRow, 4)->text();

    // dialog to input new details
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Budget");
    dialog.setModal(true);

    QFormLayout formLayout(&dialog);

    // inputs
    QComboBox *categoryComboBox = new QComboBox(&dialog);
    QSqlQuery categoryQuery(DatabaseManager::instance().getDatabase());
    categoryQuery.prepare("SELECT category_id, category_name FROM expense_category");
    if (categoryQuery.exec()) {
        while (categoryQuery.next()) {
            int categoryId = categoryQuery.value("category_id").toInt();
            QString categoryName = categoryQuery.value("category_name").toString();
            categoryComboBox->addItem(categoryName, categoryId);
            if (categoryName == currentCategory) {
                categoryComboBox->setCurrentText(categoryName);
            }
        }
    } else {
        qDebug() << "Failed to load categories:" << categoryQuery.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to load categories.");
        return;
    }
    formLayout.addRow("Category:", categoryComboBox);

    QLineEdit *amountEdit = new QLineEdit(&dialog);
    amountEdit->setText(QString::number(currentAmount, 'f', 2));
    formLayout.addRow("Amount:", amountEdit);

    QDateEdit *startDateEdit = new QDateEdit(&dialog);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDate(QDate::fromString(currentStartDate, "yyyy-MM-dd"));
    formLayout.addRow("Start Date:", startDateEdit);

    QDateEdit *endDateEdit = new QDateEdit(&dialog);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDate(QDate::fromString(currentEndDate, "yyyy-MM-dd"));
    formLayout.addRow("End Date:", endDateEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    formLayout.addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString amountText = amountEdit->text().trimmed();
        if (amountText.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Enter a budget amount.");
            return;
        }

        bool ok;
        double amount = amountEdit->text().toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(this, "Input Error", "Enter a valid amount.");
            return;
        }

        int categoryId = categoryComboBox->currentData().toInt();
        QString startDate = startDateEdit->date().toString("yyyy-MM-dd");
        QString endDate = endDateEdit->date().toString("yyyy-MM-dd");

        QSqlQuery updateQuery(DatabaseManager::instance().getDatabase());
        updateQuery.prepare(R"(
            UPDATE budgets
            SET category_id = :category_id,
                budget_amount = :budget_amount,
                start_date = :start_date,
                end_date = :end_date
            WHERE budget_id = :budget_id AND user_id = :user_id
        )");
        updateQuery.bindValue(":category_id", categoryId);
        updateQuery.bindValue(":budget_amount", amount);
        updateQuery.bindValue(":start_date", startDate);
        updateQuery.bindValue(":end_date", endDate);
        updateQuery.bindValue(":budget_id", budgetId);
        updateQuery.bindValue(":user_id", currentUserId);

        if (updateQuery.exec()) {
            QMessageBox::information(this, "Success", "Budget updated successfully!");
            loadBudgets(currentUserId);
        } else {
            qDebug() << "Failed to update budget:" << updateQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to update budget.");
        }
    }
}

// handles deleting a budget
void BudgetsPage::deleteBudget()
{
    QList<QTableWidgetItem*> selectedItems = budgetsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Select a budget to delete.");
        return;
    }

    int selectedRow = budgetsTable->row(selectedItems.first());
    int budgetId = budgetsTable->item(selectedRow, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Budget","Are you sure you want to delete the selected budget?",QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery deleteQuery(DatabaseManager::instance().getDatabase());
        deleteQuery.prepare(R"(
            DELETE FROM budgets
            WHERE budget_id = :budget_id AND user_id = :user_id
        )");
        deleteQuery.bindValue(":budget_id", budgetId);
        deleteQuery.bindValue(":user_id", currentUserId);

        if (deleteQuery.exec()) {
            QMessageBox::information(this, "Success", "Budget deleted successfully!");
            loadBudgets(currentUserId);
        } else {
            qDebug() << "Failed to delete budget:" << deleteQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to delete budget.");
        }
    }
}
