#include "BudgetsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

BudgetsPage::BudgetsPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void BudgetsPage::setupUI()
{
    QVBoxLayout *budgetsLayout = new QVBoxLayout(this);

    QLabel *budgetsLabel = new QLabel("Budgets", this);
    budgetsLabel->setAlignment(Qt::AlignCenter);
    budgetsLabel->setObjectName("titleLabel");

    // Placeholder label indicating future features
    placeholderLabel = new QLabel("Budget management features will be implemented here.", this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setStyleSheet("color: #666666; font-style: italic;");

    // Back to Dashboard Button
    backToDashboardButton = new QPushButton("Back to Dashboard", this);

    // Add widgets to layout with stretching
    budgetsLayout->addWidget(budgetsLabel);
    budgetsLayout->addStretch();
    budgetsLayout->addWidget(placeholderLabel);
    budgetsLayout->addStretch();
    budgetsLayout->addWidget(backToDashboardButton);

    // Connect button signals to emit custom signals
    connect(backToDashboardButton, &QPushButton::clicked, this, &BudgetsPage::navigateToDashboard);
}
