#include "CreateAccountPage.h"
#include <QMessageBox>

CreateAccountPage::CreateAccountPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateAccountPage)
{
    ui->setupUi(this);

    // Connect create account button click
    connect(ui->createAccountButton, &QPushButton::clicked, [this]() {
        QString username = ui->createUsernameEdit->text().trimmed();
        QString password = ui->createPasswordEdit->text();
        QString confirmPassword = ui->createConfirmPasswordEdit->text();

        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        if (password != confirmPassword) {
            QMessageBox::warning(this, "Input Error", "Passwords do not match.");
            return;
        }

        emit accountCreationRequested(username, password);
    });

    // Connect back to login button click
    connect(ui->toLoginButton, &QPushButton::clicked, this, &CreateAccountPage::navigateToLogin);
}

CreateAccountPage::~CreateAccountPage()
{
    delete ui;
}
