#include "ReceiptsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>

ReceiptsPage::ReceiptsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void ReceiptsPage::setupUI()
{
    QVBoxLayout *receiptsLayout = new QVBoxLayout(this);

    // header layout with title and upload button
    QHBoxLayout *headerLayout = new QHBoxLayout;

    QLabel *receiptsLabel = new QLabel("Receipts", this);
    receiptsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    receiptsLabel->setObjectName("titleLabel");

    uploadReceiptButton = new QPushButton("Upload Receipt", this);

    headerLayout->addWidget(receiptsLabel);
    headerLayout->addStretch(); // push the upload button to right side
    headerLayout->addWidget(uploadReceiptButton);

    receiptsLayout->addLayout(headerLayout);

    // Table to Display Receipts
    receiptsTable = new QTableWidget(this);
    receiptsTable->setColumnCount(3);
    receiptsTable->setHorizontalHeaderLabels({"Store", "Items", "Total"});
    receiptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    receiptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    receiptsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    receiptsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    receiptsLayout->addWidget(receiptsTable);

    // backt o dashboard button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    receiptsLayout->addWidget(backToDashboardButton);

    // connect button signals to emit
    connect(backToDashboardButton, &QPushButton::clicked, this, &ReceiptsPage::navigateToDashboard);
    connect(uploadReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::uploadReceipt);


}

void ReceiptsPage::addReceipt(const QString &store, const QString &items, const QString &total)
{
    int currentRow = receiptsTable->rowCount();
    receiptsTable->insertRow(currentRow);
    receiptsTable->setItem(currentRow, 0, new QTableWidgetItem(store));
    receiptsTable->setItem(currentRow, 1, new QTableWidgetItem(items));
    receiptsTable->setItem(currentRow, 2, new QTableWidgetItem(total));
}

void ReceiptsPage::loadReceipts(int userId)
{
    receiptsTable->setRowCount(0); // Clear existing rows

    // fetch receipts from the database
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT store, items, expense_amount FROM expenses WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        while (query.next()) {
            QString store = query.value("store").toString();
            QString items = query.value("items").toString();
            QString total = QString::number(query.value("expense_amount").toDouble(), 'f', 2);
            addReceipt(store, items, total);
        }
    } else {
        qDebug() << "Error loading receipts:" << query.lastError().text();
    }
}


