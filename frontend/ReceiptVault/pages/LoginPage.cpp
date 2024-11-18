#include "LoginPage.h"
#include <QMessageBox>

// Constructor
LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginPage) // Initialize the UI pointer
{
    ui->setupUi(this); // Sets up the UI from the .ui file

    // Connect login button signal
    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        QString username = ui->loginUsernameEdit->text().trimmed();
        QString password = ui->loginPasswordEdit->text();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
            return;
        }

        emit loginRequested(username, password); // Emit signal for login (handled in main window)
    });

    // Connect create account button signal
    connect(ui->toCreateAccountButton, &QPushButton::clicked, this, &LoginPage::navigateToCreateAccount);
}

// Destructor
LoginPage::~LoginPage()
{
    delete ui; // Clean up the UI object
}
