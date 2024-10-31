#include "LoginPage.h"
#include <QVBoxLayout>
#include <QLabel>

LoginPage::LoginPage(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LoginPage::setupUI()
{
    QVBoxLayout *loginLayout = new QVBoxLayout(this);

    QLabel *loginLabel = new QLabel("Login", this);
    loginLabel->setAlignment(Qt::AlignCenter);
    loginLabel->setObjectName("titleLabel");

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

    // Connect button signals to emit custom signals
    connect(loginButton, &QPushButton::clicked, [this]() {
        emit loginRequested(loginUsernameEdit->text(), loginPasswordEdit->text());
    });

    connect(toCreateAccountButton, &QPushButton::clicked, this, &LoginPage::navigateToCreateAccount);
}
