#include "ReceiptsPage.h"
#include "pages/CategoryManagerDialog.h"
#include "ui_ReceiptsPage.h" // Include the generated UI header
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialogButtonBox>

// Constructor
ReceiptsPage::ReceiptsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiptsPage),
    currentUserId(-1)
{
    ui->setupUi(this); // Sets up the UI from the .ui file

    // Connect buttons to slots
    connect(ui->button_BackToDashboard, &QPushButton::clicked, this, &ReceiptsPage::navigateToDashboard);
    connect(ui->button_UploadReceipt, &QPushButton::clicked, this, &ReceiptsPage::uploadReceipt);
    connect(ui->button_EditReceipt, &QPushButton::clicked, this, &ReceiptsPage::editSelectedReceipt);

    // Connect the new Edit Categories button
    connect(ui->button_EditCategories, &QPushButton::clicked, this, [this]() {
        // Open the Category Manager Dialog
        CategoryManagerDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            // Refresh the receipts table to update category combo boxes
            loadReceipts(currentUserId);
        }
    });
}

// Destructor
ReceiptsPage::~ReceiptsPage()
{
    delete ui;
}

// Populates the category combo box
void ReceiptsPage::populateCategoryComboBox(QComboBox *comboBox)
{
    QList<QPair<int, QString>> categories = DatabaseManager::instance().getAllCategories();
    for (const QPair<int, QString> &category : categories) {
        comboBox->addItem(category.second, category.first);
    }
}

// Adds a new receipt to the table
void ReceiptsPage::addReceipt(const QString &store, const QString &total, const QString &date, int categoryId, int expenseId)
{
    int currentRow = ui->table_Receipts->rowCount();
    ui->table_Receipts->insertRow(currentRow);

    // Store column
    QTableWidgetItem *storeItem = new QTableWidgetItem(store);
    storeItem->setData(Qt::UserRole, expenseId); // Stores expense ID
    ui->table_Receipts->setItem(currentRow, 0, storeItem);

    // Total column
    QTableWidgetItem *totalItem = new QTableWidgetItem(total);
    ui->table_Receipts->setItem(currentRow, 1, totalItem);

    // Date column
    QTableWidgetItem *dateItem = new QTableWidgetItem(date);
    ui->table_Receipts->setItem(currentRow, 2, dateItem);

    // Category dropdown
    QComboBox *categoryComboBox = new QComboBox(this);
    populateCategoryComboBox(categoryComboBox);

    // Set current category
    int comboBoxIndex = categoryComboBox->findData(categoryId);
    if (comboBoxIndex != -1) {
        categoryComboBox->setCurrentIndex(comboBoxIndex);
    }

    // Connect dropdown changes to update the database
    connect(categoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        QVariant data = categoryComboBox->itemData(index);
        if (data.isValid()) {
            int selectedCategoryId = data.toInt();

            // Updates the category in the database
            int expenseId = storeItem->data(Qt::UserRole).toInt();
            QSqlQuery updateQuery(DatabaseManager::instance().getDatabase());
            updateQuery.prepare("UPDATE expenses SET category_id = :category_id WHERE expense_id = :expense_id AND user_id = :user_id");
            updateQuery.bindValue(":category_id", selectedCategoryId);
            updateQuery.bindValue(":expense_id", expenseId);
            updateQuery.bindValue(":user_id", currentUserId);

            if (!updateQuery.exec()) {
                qDebug() << "Failed to update category:" << updateQuery.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to update receipt category.");
            }
        }
    });

    ui->table_Receipts->setCellWidget(currentRow, 3, categoryComboBox); // Adds the dropdown
}

// Loads receipts for the current user
void ReceiptsPage::loadReceipts(int userId)
{
    ui->table_Receipts->setRowCount(0); // Clears table

    currentUserId = userId; // Store user ID

    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare("SELECT expense_id, store, expense_amount, expense_date, category_id FROM expenses WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        while (query.next()) {
            int expenseId = query.value("expense_id").toInt();
            QString store = query.value("store").toString();
            double amount = query.value("expense_amount").toDouble();
            QString total = QString::number(amount, 'f', 2);
            QString date = query.value("expense_date").toString();
            int categoryId = query.value("category_id").toInt();

            addReceipt(store, total, date, categoryId, expenseId); // Adds receipt to table
        }
    } else {
        qDebug() << "Error loading receipts:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to load receipts from the database.");
    }
}

// Sets the current user ID
void ReceiptsPage::setCurrentUserId(int userId)
{
    currentUserId = userId;
}

// Handles editing the selected receipt
void ReceiptsPage::editSelectedReceipt()
{
    QList<QTableWidgetItem*> selectedItems = ui->table_Receipts->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a receipt to edit.");
        return;
    }

    int selectedRow = ui->table_Receipts->row(selectedItems.first());

    // Retrieves the expense ID from UserRole
    QTableWidgetItem *storeItem = ui->table_Receipts->item(selectedRow, 0);
    int expenseId = storeItem->data(Qt::UserRole).toInt();

    // Emit signal for editing receipt
    emit editReceipt(expenseId);
}
