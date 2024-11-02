#ifndef RECEIPTSPAGE_H
#define RECEIPTSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>

class ReceiptsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiptsPage(QWidget *parent = nullptr);

    // method to add a receipt entry to the table
    void addReceipt(const QString &store, const QString &items, const QString &total);

    // method to load receipts from the database for a specific user
    void loadReceipts(int userId);

signals:
    void navigateToDashboard();
    void uploadReceipt();

private:
    QTableWidget *receiptsTable;
    QPushButton *backToDashboardButton;
    QPushButton *uploadReceiptButton;

    void setupUI();
};

#endif
