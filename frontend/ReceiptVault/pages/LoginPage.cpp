#include "LoginPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>

// constructor for LoginPage
LoginPage::LoginPage(QWidget *parent) : QWidget(parent)
{
    setupUI(); // sets up the login UI
}

// sets up the UI layout
void LoginPage::setupUI()
{
    QVBoxLayout *loginLayout = new QVBoxLayout(this); // main layout

    // title label
    QLabel *loginLabel = new QLabel("Login", this);
    loginLabel->setObjectName("titleLabel"); // for styling
    loginLabel->setAlignment(Qt::AlignCenter);

    // username field
    loginUsernameEdit = new QLineEdit(this);
    loginUsernameEdit->setPlaceholderText("Username");

    // password field
    loginPasswordEdit = new QLineEdit(this);
    loginPasswordEdit->setPlaceholderText("Password");
    loginPasswordEdit->setEchoMode(QLineEdit::Password); // hides the input text

    // buttons
    loginButton = new QPushButton("Login", this);
    toCreateAccountButton = new QPushButton("Create Account", this);

    // add widgets to layout
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(loginUsernameEdit);
    loginLayout->addWidget(loginPasswordEdit);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(toCreateAccountButton);

    // connect login button signal
    connect(loginButton, &QPushButton::clicked, [this]() {
        QString username = loginUsernameEdit->text().trimmed();
        QString password = loginPasswordEdit->text();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
            return;
        }

        emit loginRequested(username, password); // emit signal for login (handled in main window)
    });

    // connect create account button signal
    connect(toCreateAccountButton, &QPushButton::clicked, this, &LoginPage::navigateToCreateAccount);
}
