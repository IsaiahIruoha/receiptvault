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

    // Method to add a receipt entry
    void addReceipt(const QString &store, const QString &items, const QString &total);

signals:
    void navigateToDashboard();
    void uploadReceipt();

private:
    QTableWidget *receiptsTable;
    QPushButton *backToDashboardButton;
    QPushButton *uploadReceiptButton;

    void setupUI();
};

#endif // RECEIPTSPAGE_H
