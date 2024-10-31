#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QFileDialog> // For file dialog

// include qt charts modules
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QHeaderView>

using namespace Qt;

// constructor for mainwindow class
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // set the window title and size
    setWindowTitle("ReceiptVault - Budget Tracking App");
    resize(1000, 600);

    // create and set the stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // initialize all the pages
    loginPage = new LoginPage(this);
    createAccountPage = new CreateAccountPage(this);
    dashboardPage = new DashboardPage(this);
    receiptsPage = new ReceiptsPage(this);
    analyticsPage = new AnalyticsPage(this);
    budgetsPage = new BudgetsPage(this);

    // add all the pages to the stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(createAccountPage);
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(receiptsPage);
    stackedWidget->addWidget(analyticsPage);
    stackedWidget->addWidget(budgetsPage);

    // display the login page first
    stackedWidget->setCurrentWidget(loginPage);

    // apply external styles
    applyStyles();

    // Connect signals from LoginPage
    connect(loginPage, &LoginPage::loginRequested, this, &MainWindow::handleLogin);
    connect(loginPage, &LoginPage::navigateToCreateAccount, [this]() {
        stackedWidget->setCurrentWidget(createAccountPage);
    });

    // Connect signals from CreateAccountPage
    connect(createAccountPage, &CreateAccountPage::accountCreationRequested, this, &MainWindow::handleCreateAccount);
    connect(createAccountPage, &CreateAccountPage::navigateToLogin, [this]() {
        stackedWidget->setCurrentWidget(loginPage);
    });

    // Connect signals from DashboardPage
    connect(dashboardPage, &DashboardPage::navigateToReceipts, [this]() {
        stackedWidget->setCurrentWidget(receiptsPage);
    });
    connect(dashboardPage, &DashboardPage::navigateToAnalytics, [this]() {
        stackedWidget->setCurrentWidget(analyticsPage);
    });
    connect(dashboardPage, &DashboardPage::navigateToBudgets, [this]() {
        stackedWidget->setCurrentWidget(budgetsPage);
    });
    connect(dashboardPage, &DashboardPage::logoutRequested, [this]() {
        // Clear any sensitive information
        loginPage->findChild<QLineEdit*>("loginUsernameEdit")->clear();
        loginPage->findChild<QLineEdit*>("loginPasswordEdit")->clear();
        currentUsername.clear();
        // Navigate back to the login page
        stackedWidget->setCurrentWidget(loginPage);
    });

    // Connect signals from ReceiptsPage
    connect(receiptsPage, &ReceiptsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);
    connect(receiptsPage, &ReceiptsPage::uploadReceipt, this, &MainWindow::handleUploadReceipt);

    // Connect signals from AnalyticsPage and BudgetsPage
    connect(analyticsPage, &AnalyticsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);
    connect(budgetsPage, &BudgetsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);
}

// destructor for mainwindow class
MainWindow::~MainWindow()
{
}

// function to apply external styles to the application
void MainWindow::applyStyles()
{
    // load the external stylesheet from resources
    QFile styleFile(":/styles/style.qss"); // path to the style.qss in resources
    if (styleFile.open(QFile::ReadOnly)) {
        // read the stylesheet content
        QString style = QLatin1String(styleFile.readAll());
        // apply the stylesheet to the main window
        this->setStyleSheet(style);
        // close the file
        styleFile.close();
        // print a debug message indicating successful loading
        qDebug() << "Stylesheet loaded successfully.";
    } else {
        // print a debug message if the stylesheet failed to load
        qDebug() << "Failed to load style.qss";
    }
}

// function to handle the login process
void MainWindow::handleLogin(const QString &username, const QString &password)
{
    // check if either the username or password fields are empty
    if (username.isEmpty() || password.isEmpty()) {
        // show a warning message if any fields are empty
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    // define hardcoded credentials for testing
    QString correctUsername = "admin";
    QString correctPassword = "password123";

    // check if the entered credentials match the hardcoded ones
    if (username == correctUsername && password == correctPassword) {
        // show an information message indicating successful login
        // QMessageBox::information(this, "Success", "Login successful!"); // removed login affirmation
        // store the current username
        currentUsername = username;
        // navigate to the dashboard page
        stackedWidget->setCurrentWidget(dashboardPage);
    } else {
        // show a warning message indicating failed login
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
    }
}

// function to handle the create account process
void MainWindow::handleCreateAccount(const QString &username, const QString &password)
{
    // Simulate account creation (e.g., save to database)
    // Here, just show a success message
    QMessageBox::information(this, "Success", "Account created successfully!");

    // Navigate back to the login page
    stackedWidget->setCurrentWidget(loginPage);
}

// function to navigate back to the dashboard
void MainWindow::navigateToDashboard()
{
    // set the current widget to the dashboard page
    stackedWidget->setCurrentWidget(dashboardPage);
}

// function to navigate back to the login page
void MainWindow::navigateToLogin()
{
    // set the current widget to the login page
    stackedWidget->setCurrentWidget(loginPage);
}

// function to handle receipt upload
void MainWindow::handleUploadReceipt()
{
    // Open a file dialog to select an image file
    QString fileName = QFileDialog::getOpenFileName(this, "Select Receipt Image", "",
                                                    "Images (*.pdf *.png *.jpg *.jpeg);;All Files (*)");
    if (!fileName.isEmpty()) {
        // For now, just display the selected file path in debug
        qDebug() << "Selected file:" << fileName;

        // Placeholder: In future, integrate OCR processing here

        // For demonstration, let's add a mock entry to the receipts table
        receiptsPage->addReceipt("New Store", "Item1, Item2", "0.00");

        // Optionally, you can store the file path or display the image
        // For example, you could add another column for the file path or display the image in a separate widget
    } else {
        // User canceled the dialog
        qDebug() << "No file selected.";
    }
}
