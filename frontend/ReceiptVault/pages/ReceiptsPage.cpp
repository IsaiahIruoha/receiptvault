#include "ReceiptsPage.h"
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

ReceiptsPage::ReceiptsPage(QWidget *parent) : QWidget(parent), currentUserId(-1)
{
    setupUI();
}

void ReceiptsPage::setupUI()
{
    QVBoxLayout *receiptsLayout = new QVBoxLayout(this);

    // Header Layout with Title and Upload/Edit Buttons
    QHBoxLayout *headerLayout = new QHBoxLayout;

    QLabel *receiptsLabel = new QLabel("Receipts", this);
    receiptsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    receiptsLabel->setObjectName("titleLabel");

    uploadReceiptButton = new QPushButton("Upload Receipt", this);
    editReceiptButton = new QPushButton("Edit Receipt", this); // Initialize Edit Button

    headerLayout->addWidget(receiptsLabel);
    headerLayout->addStretch(); // Push the buttons to the right
    headerLayout->addWidget(uploadReceiptButton);
    headerLayout->addWidget(editReceiptButton); // Add Edit Button to Layout

    receiptsLayout->addLayout(headerLayout);

    // Table to Display Receipts
    receiptsTable = new QTableWidget(this);
    receiptsTable->setColumnCount(4);
    receiptsTable->setHorizontalHeaderLabels({"Store", "Total", "Date", "Category"});
    receiptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    receiptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    receiptsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    receiptsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    receiptsLayout->addWidget(receiptsTable);

    // Back to Dashboard Button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    receiptsLayout->addWidget(backToDashboardButton);

    // Connect button signals to emit custom signals
    connect(backToDashboardButton, &QPushButton::clicked, this, &ReceiptsPage::navigateToDashboard);
    connect(uploadReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::uploadReceipt);
    connect(editReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::editSelectedReceipt); // Connect Edit Button
}

void ReceiptsPage::populateCategoryComboBox(QComboBox *comboBox)
{
    QList<QPair<int, QString>> categories = DatabaseManager::instance().getAllCategories();
    for (const QPair<int, QString> &category : categories) {
        comboBox->addItem(category.second, category.first);
    }
}

void ReceiptsPage::addReceipt(const QString &store, const QString &total, const QString &date, int categoryId, int expenseId)
{
    int currentRow = receiptsTable->rowCount();
    receiptsTable->insertRow(currentRow);

    // Store Column
    QTableWidgetItem *storeItem = new QTableWidgetItem(store);
    storeItem->setData(Qt::UserRole, expenseId); // Store expense_id for later reference
    receiptsTable->setItem(currentRow, 0, storeItem);

    // Total Column
    QTableWidgetItem *totalItem = new QTableWidgetItem(total);
    receiptsTable->setItem(currentRow, 1, totalItem);

    // Date Column
    QTableWidgetItem *dateItem = new QTableWidgetItem(date);
    receiptsTable->setItem(currentRow, 2, dateItem);

    // Category Column with Dropdown
    QComboBox *categoryComboBox = new QComboBox(this);
    populateCategoryComboBox(categoryComboBox);

    // Set the current category
    int comboBoxIndex = categoryComboBox->findData(categoryId);
    if (comboBoxIndex != -1) {
        categoryComboBox->setCurrentIndex(comboBoxIndex);
    }

    // Connect the ComboBox signal to handle category changes
    connect(categoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        QVariant data = categoryComboBox->itemData(index);
        if (data.isValid()) {
            int selectedCategoryId = data.toInt();

            // Retrieve expense_id stored in UserRole
            int expenseId = storeItem->data(Qt::UserRole).toInt();

            // Update the category in the database
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

    receiptsTable->setCellWidget(currentRow, 3, categoryComboBox);
}

void ReceiptsPage::loadReceipts(int userId)
{
    receiptsTable->setRowCount(0); // clear existing rows

    // store the current user ID
    currentUserId = userId;

    // fetch receipts from the database
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    QSqlQuery query(db);
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

            addReceipt(store, total, date, categoryId, expenseId);
        }
    } else {
        qDebug() << "Error loading receipts:" << query.lastError().text();
    }
}

void ReceiptsPage::setCurrentUserId(int userId)
{
    currentUserId = userId;
}

void ReceiptsPage::editSelectedReceipt()
{
    QList<QTableWidgetItem*> selectedItems = receiptsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a receipt to edit.");
        return;
    }

    int selectedRow = receiptsTable->row(selectedItems.first());

    // retrieve expense_id from UserRole
    QTableWidgetItem *storeItem = receiptsTable->item(selectedRow, 0);
    int expenseId = storeItem->data(Qt::UserRole).toInt();

    // emit the signal
    emit editReceipt(expenseId);
}
