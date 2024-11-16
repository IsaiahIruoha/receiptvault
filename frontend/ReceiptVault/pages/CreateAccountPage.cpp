#include "CreateAccountPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDateTime> // for timestamp generation

// constructor for CreateAccountPage
CreateAccountPage::CreateAccountPage(QWidget *parent) : QWidget(parent)
{
    setupUI(); // setup the page's UI
}

// sets up the UI layout
void CreateAccountPage::setupUI()
{
    QVBoxLayout *createLayout = new QVBoxLayout(this); // main layout

    // title label
    QLabel *createLabel = new QLabel("Create Account", this);
    createLabel->setObjectName("titleLabel"); // for styling
    createLabel->setAlignment(Qt::AlignCenter);

    // username input field
    createUsernameEdit = new QLineEdit(this);
    createUsernameEdit->setPlaceholderText("Username");

    // password input field
    createPasswordEdit = new QLineEdit(this);
    createPasswordEdit->setPlaceholderText("Password");
    createPasswordEdit->setEchoMode(QLineEdit::Password); // hide input

    // confirm password input field
    createConfirmPasswordEdit = new QLineEdit(this);
    createConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    createConfirmPasswordEdit->setEchoMode(QLineEdit::Password); // hide input

    // buttons for creating account and navigating back to login
    createAccountButton = new QPushButton("Create Account", this);
    toLoginButton = new QPushButton("Back to Login", this);

    // add widgets to the layout
    createLayout->addWidget(createLabel);
    createLayout->addWidget(createUsernameEdit);
    createLayout->addWidget(createPasswordEdit);
    createLayout->addWidget(createConfirmPasswordEdit);
    createLayout->addWidget(createAccountButton);
    createLayout->addWidget(toLoginButton);

    // handle create account button click
    connect(createAccountButton, &QPushButton::clicked, [this]() {
        QString username = createUsernameEdit->text().trimmed(); // get username
        QString password = createPasswordEdit->text();           // get password
        QString confirmPassword = createConfirmPasswordEdit->text(); // get confirmation

        // check if fields are filled
        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        // check if passwords match
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Input Error", "Passwords do not match.");
            return;
        }

        emit accountCreationRequested(username, password); // signal account creation (handled in main window)
    });

    // handle back to login button click
    connect(toLoginButton, &QPushButton::clicked, this, &CreateAccountPage::navigateToLogin);
}
