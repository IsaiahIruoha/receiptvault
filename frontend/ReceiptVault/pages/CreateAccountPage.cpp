#include "CreateAccountPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

CreateAccountPage::CreateAccountPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void CreateAccountPage::setupUI()
{
    QVBoxLayout *createLayout = new QVBoxLayout(this);

    QLabel *createLabel = new QLabel("Create Account", this);
    createLabel->setAlignment(Qt::AlignCenter);
    createLabel->setObjectName("titleLabel");

    createUsernameEdit = new QLineEdit(this);
    createUsernameEdit->setPlaceholderText("Username");

    createPasswordEdit = new QLineEdit(this);
    createPasswordEdit->setPlaceholderText("Password");
    createPasswordEdit->setEchoMode(QLineEdit::Password);

    createConfirmPasswordEdit = new QLineEdit(this);
    createConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    createConfirmPasswordEdit->setEchoMode(QLineEdit::Password);

    createAccountButton = new QPushButton("Create Account", this);
    toLoginButton = new QPushButton("Back to Login", this);

    createLayout->addWidget(createLabel);
    createLayout->addWidget(createUsernameEdit);
    createLayout->addWidget(createPasswordEdit);
    createLayout->addWidget(createConfirmPasswordEdit);
    createLayout->addWidget(createAccountButton);
    createLayout->addWidget(toLoginButton);

    // Connect button signals to emit custom signals
    connect(createAccountButton, &QPushButton::clicked, [this]() {
        QString username = createUsernameEdit->text();
        QString password = createPasswordEdit->text();
        QString confirmPassword = createConfirmPasswordEdit->text();

        // Emit signal only if passwords match and fields are filled
        if (!username.isEmpty() && !password.isEmpty() && password == confirmPassword) {
            emit accountCreationRequested(username, password);
        } else {
            QMessageBox::warning(this, "Input Error", "Please ensure all fields are filled and passwords match.");
        }
    });

    connect(toLoginButton, &QPushButton::clicked, this, &CreateAccountPage::navigateToLogin);
}
