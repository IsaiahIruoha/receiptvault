#include "ReceiptsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

ReceiptsPage::ReceiptsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void ReceiptsPage::setupUI()
{
    QVBoxLayout *receiptsLayout = new QVBoxLayout(this);

    // Header Layout with Title and Upload Button
    QHBoxLayout *headerLayout = new QHBoxLayout;

    QLabel *receiptsLabel = new QLabel("Receipts", this);
    receiptsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    receiptsLabel->setObjectName("titleLabel");

    uploadReceiptButton = new QPushButton("Upload Receipt", this);
    // Optionally set an icon: uploadReceiptButton->setIcon(QIcon(":/icons/upload.png"));

    headerLayout->addWidget(receiptsLabel);
    headerLayout->addStretch(); // Push the upload button to the right
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

    // Back to Dashboard Button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);
    receiptsLayout->addWidget(backToDashboardButton);

    // Connect button signals to emit custom signals
    connect(backToDashboardButton, &QPushButton::clicked, this, &ReceiptsPage::navigateToDashboard);
    connect(uploadReceiptButton, &QPushButton::clicked, this, &ReceiptsPage::uploadReceipt);

    // Populate with mock data
    receiptsTable->setRowCount(2); // Example with 2 receipts

    // First receipt entry
    receiptsTable->setItem(0, 0, new QTableWidgetItem("SuperMart"));
    receiptsTable->setItem(0, 1, new QTableWidgetItem("Milk, Bread, Eggs"));
    receiptsTable->setItem(0, 2, new QTableWidgetItem("15.75"));

    // Second receipt entry
    receiptsTable->setItem(1, 0, new QTableWidgetItem("ElectroShop"));
    receiptsTable->setItem(1, 1, new QTableWidgetItem("Headphones"));
    receiptsTable->setItem(1, 2, new QTableWidgetItem("45.00"));
}

void ReceiptsPage::addReceipt(const QString &store, const QString &items, const QString &total)
{
    int currentRow = receiptsTable->rowCount();
    receiptsTable->insertRow(currentRow);
    receiptsTable->setItem(currentRow, 0, new QTableWidgetItem(store));
    receiptsTable->setItem(currentRow, 1, new QTableWidgetItem(items));
    receiptsTable->setItem(currentRow, 2, new QTableWidgetItem(total));
}
