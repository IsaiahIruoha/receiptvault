#ifndef RECEIPTSPAGE_H
#define RECEIPTSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class ReceiptsPage;
}

class ReceiptsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiptsPage(QWidget *parent = nullptr);
    ~ReceiptsPage();

    void addReceipt(const QString &store, const QString &total, const QString &date, int categoryId, int expenseId); // add a receipt to the table
    void loadReceipts(int userId); // load user receipts
    void setCurrentUserId(int userId); // set the current user ID
    void populateCategoryComboBox(QComboBox *comboBox); // fill category dropdown

signals:
    void navigateToDashboard(); // signal to go back to dashboard
    void uploadReceipt(); // signal to upload a receipt
    void editReceipt(int expenseId); // signal to edit a receipt

public slots:
    void editSelectedReceipt(); // handles editing a selected receipt

private:
    Ui::ReceiptsPage *ui; // Pointer to UI object
    int currentUserId; // stores the user ID

    void setupUI(); // sets up the UI (if needed)
    void refreshReceiptsTable(); // refreshes the table
};

#endif // RECEIPTSPAGE_H
