#ifndef RECEIPTSPAGE_H
#define RECEIPTSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>

class ReceiptsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiptsPage(QWidget *parent = nullptr);

    // Method to add a receipt entry to the table
    void addReceipt(const QString &store, const QString &total, const QString &date, int categoryId, int expenseId);

    // Method to load receipts from the database for a specific user
    void loadReceipts(int userId);

    // Set the current user ID
    void setCurrentUserId(int userId);

    void populateCategoryComboBox(QComboBox *comboBox);

signals:
    void navigateToDashboard();
    void uploadReceipt();
    void editReceipt(int expenseId);

public slots:
    void editSelectedReceipt();

private:
    QTableWidget *receiptsTable;
    QPushButton *backToDashboardButton;
    QPushButton *uploadReceiptButton;
    QPushButton *editReceiptButton; // New Edit Receipt Button

    int currentUserId; // To store the current user ID

    void setupUI();
};

#endif // RECEIPTSPAGE_H
