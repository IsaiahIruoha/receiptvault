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

// constructor for ReceiptsPage
ReceiptsPage::ReceiptsPage(QWidget *parent) : QWidget(parent), currentUserId(-1)
{
    setupUI(); // sets up the page layout
}

// sets up the UI layout
void ReceiptsPage::setupUI()
{
    QVBoxLayout *receiptsLayout = new QVBoxLayout(this);

    // header layout with title and buttons
    QHBoxLayout *headerLayout = new QHBoxLayout;

    QLabel *receiptsLabel = new QLabel("Receipts", this);
    receiptsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    receiptsLabel->setObjectName("titleLabel");

    uploadReceiptButton = new QPushButton("Upload Receipt", this);
    editReceiptButton = new QPushButton("Edit Receipt", this); // edit button

    headerLayout->addWidget(receiptsLabel);
    headerLayout->addStretch(); // aligns buttons to the right
    headerLayout->addWidget(uploadReceiptButton);
    headerLayout->addWidget(editReceiptButton);

    receiptsLayout->addLayout(headerLayout);

    // table to display receipts
    receiptsTable = new QTableWidget(this);
    receiptsTable->setColumnCount(4);
    receiptsTable->setHorizontalHeaderLabels({"Store", "Total", "Date", "Category"});
    receiptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    receiptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    receiptsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    receiptsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    receiptsLayout->addWidget(receiptsTable);

    // back to dashboard button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    receiptsLayout->addWidget(backToDashboardButton);

    // connect signals to slots
    connect(backToDashboardButton, &QPushButton::clicked, this, &ReceiptsPage::navigateToDashboard);
    connect(uploadReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::uploadReceipt);
    connect(editReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::editSelectedReceipt); // edit receipt
}

// populates the category combo box
void ReceiptsPage::populateCategoryComboBox(QComboBox *comboBox)
{
    QList<QPair<int, QString>> categories = DatabaseManager::instance().getAllCategories();
    for (const QPair<int, QString> &category : categories) {
        comboBox->addItem(category.second, category.first);
    }
}

// adds a new receipt to the table
void ReceiptsPage::addReceipt(const QString &store, const QString &total, const QString &date, int categoryId, int expenseId)
{
    int currentRow = receiptsTable->rowCount();
    receiptsTable->insertRow(currentRow);

    // store column
    QTableWidgetItem *storeItem = new QTableWidgetItem(store);
    storeItem->setData(Qt::UserRole, expenseId); // stores expense ID
    receiptsTable->setItem(currentRow, 0, storeItem);

    // total column
    QTableWidgetItem *totalItem = new QTableWidgetItem(total);
    receiptsTable->setItem(currentRow, 1, totalItem);

    // date column
    QTableWidgetItem *dateItem = new QTableWidgetItem(date);
    receiptsTable->setItem(currentRow, 2, dateItem);

    // category dropdown
    QComboBox *categoryComboBox = new QComboBox(this);
    populateCategoryComboBox(categoryComboBox);

    // set current category
    int comboBoxIndex = categoryComboBox->findData(categoryId);
    if (comboBoxIndex != -1) {
        categoryComboBox->setCurrentIndex(comboBoxIndex);
    }

    // connect dropdown changes to update the database
    connect(categoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        QVariant data = categoryComboBox->itemData(index);
        if (data.isValid()) {
            int selectedCategoryId = data.toInt();

            // updates the category in the database
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

    receiptsTable->setCellWidget(currentRow, 3, categoryComboBox); // adds the dropdown
}

// loads receipts for the current user
void ReceiptsPage::loadReceipts(int userId)
{
    receiptsTable->setRowCount(0); // clears table

    currentUserId = userId; // store user ID

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

            addReceipt(store, total, date, categoryId, expenseId); // adds receipt to table
        }
    } else {
        qDebug() << "Error loading receipts:" << query.lastError().text();
    }
}

// sets the current user ID
void ReceiptsPage::setCurrentUserId(int userId)
{
    currentUserId = userId;
}

// handles editing the selected receipt
void ReceiptsPage::editSelectedReceipt()
{
    QList<QTableWidgetItem*> selectedItems = receiptsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a receipt to edit.");
        return;
    }

    int selectedRow = receiptsTable->row(selectedItems.first());

    // retrieves the expense ID from UserRole
    QTableWidgetItem *storeItem = receiptsTable->item(selectedRow, 0);
    int expenseId = storeItem->data(Qt::UserRole).toInt();

    // emit signal for editing receipt
    emit editReceipt(expenseId);
}
