#ifndef BUDGETSPAGE_H
#define BUDGETSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class BudgetsPage : public QWidget
{
    Q_OBJECT
public:
    explicit BudgetsPage(QWidget *parent = nullptr);

signals:
    void navigateToDashboard();

private:
    QPushButton *backToDashboardButton;
    QLabel *placeholderLabel;

    void setupUI();
};

#endif // BUDGETSPAGE_H
