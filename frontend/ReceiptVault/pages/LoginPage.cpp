#include "LoginPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include "DatabaseManager.h"

LoginPage::LoginPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LoginPage::setupUI()
{
    QVBoxLayout *loginLayout = new QVBoxLayout(this);

    QLabel *loginLabel = new QLabel("Login", this);
    loginLabel->setObjectName("titleLabel");
    loginLabel->setAlignment(Qt::AlignCenter);

    loginUsernameEdit = new QLineEdit(this);
    loginUsernameEdit->setPlaceholderText("Username");

    loginPasswordEdit = new QLineEdit(this);
    loginPasswordEdit->setPlaceholderText("Password");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    toCreateAccountButton = new QPushButton("Create Account", this);

    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(loginUsernameEdit);
    loginLayout->addWidget(loginPasswordEdit);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(toCreateAccountButton);

    // connect button signals
    connect(loginButton, &QPushButton::clicked, [this]() {
        QString username = loginUsernameEdit->text().trimmed();
        QString password = loginPasswordEdit->text();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
            return;
        }

        emit loginRequested(username, password); // emit signal
    });

    connect(toCreateAccountButton, &QPushButton::clicked, this, &LoginPage::navigateToCreateAccount);
}
