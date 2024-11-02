#include "CreateAccountPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDateTime> // Include for QDateTime
#include "DatabaseManager.h"

CreateAccountPage::CreateAccountPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void CreateAccountPage::setupUI()
{
    QVBoxLayout *createLayout = new QVBoxLayout(this);

    QLabel *createLabel = new QLabel("Create Account", this);
    createLabel->setObjectName("titleLabel");
    createLabel->setAlignment(Qt::AlignCenter);

    // user
    createUsernameEdit = new QLineEdit(this);
    createUsernameEdit->setPlaceholderText("Username");

    // pass
    createPasswordEdit = new QLineEdit(this);
    createPasswordEdit->setPlaceholderText("Password");
    createPasswordEdit->setEchoMode(QLineEdit::Password);

    // confirm pass
    createConfirmPasswordEdit = new QLineEdit(this);
    createConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    createConfirmPasswordEdit->setEchoMode(QLineEdit::Password);

    // create account and back button
    createAccountButton = new QPushButton("Create Account", this);
    toLoginButton = new QPushButton("Back to Login", this);

    // add widgets to layout
    createLayout->addWidget(createLabel);
    createLayout->addWidget(createUsernameEdit);
    createLayout->addWidget(createPasswordEdit);
    createLayout->addWidget(createConfirmPasswordEdit);
    createLayout->addWidget(createAccountButton);
    createLayout->addWidget(toLoginButton);

    // connect button signals
    connect(createAccountButton, &QPushButton::clicked, [this]() {
        QString username = createUsernameEdit->text().trimmed();
        QString password = createPasswordEdit->text();
        QString confirmPassword = createConfirmPasswordEdit->text();

        // input validation
        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        if (password != confirmPassword) {
            QMessageBox::warning(this, "Input Error", "Passwords do not match.");
            return;
        }

        emit accountCreationRequested(username, password); // emit signal for account creation (happens in main wind0w)
    });

    connect(toLoginButton, &QPushButton::clicked, this, &CreateAccountPage::navigateToLogin);
}
