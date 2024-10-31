#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog> // Include QFileDialog
#include <QFile>
#include <QPainter>
#include <QDebug>

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
    setupLoginPage();
    setupCreateAccountPage(); // Add this line
    setupDashboardPage();
    setupReceiptsPage();
    setupAnalyticsPage();
    setupBudgetsPage();

    // add all the pages to the stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(createAccountPage); // Ensure this is added
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(receiptsPage);
    stackedWidget->addWidget(analyticsPage);
    stackedWidget->addWidget(budgetsPage);

    // display the login page first
    stackedWidget->setCurrentWidget(loginPage);

    // apply external styles
    applyStyles();
}


// destructor for mainwindow class
MainWindow::~MainWindow()
{
}

// function to set up the login page
void MainWindow::setupLoginPage()
{
    // create a new widget for the login page
    loginPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *loginLayout = new QVBoxLayout;

    // create a label for the login title
    QLabel *loginLabel = new QLabel("Login");
    // center the label text
    loginLabel->setAlignment(Qt::AlignCenter);
    // set the object name for styling
    loginLabel->setObjectName("titleLabel");

    // create a line edit for the username input
    loginUsernameEdit = new QLineEdit(loginPage);
    // set placeholder text for the username input
    loginUsernameEdit->setPlaceholderText("Username");

    // create a line edit for the password input
    loginPasswordEdit = new QLineEdit(loginPage);
    // set placeholder text for the password input
    loginPasswordEdit->setPlaceholderText("Password");
    // set the echo mode to hide the password characters
    loginPasswordEdit->setEchoMode(QLineEdit::Password);

    // create a button for logging in
    loginButton = new QPushButton("Login", loginPage);
    // create a button to navigate to the create account page
    toCreateAccountButton = new QPushButton("Create Account", loginPage);

    // add the widgets to the layout
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(loginUsernameEdit);
    loginLayout->addWidget(loginPasswordEdit);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(toCreateAccountButton);

    // set the layout for the login page
    loginPage->setLayout(loginLayout);

    // connect the login button to the handleLogin function
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    // connect the create account button to the handleCreateAccount function
    connect(toCreateAccountButton, &QPushButton::clicked, this, &MainWindow::handleCreateAccount);
}

// function to set up the create account page
void MainWindow::setupCreateAccountPage()
{
    // create a new widget for the create account page
    createAccountPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *createLayout = new QVBoxLayout;

    // create a label for the create account title
    QLabel *createLabel = new QLabel("Create Account");
    // center the label text
    createLabel->setAlignment(Qt::AlignCenter);
    // set the object name for styling
    createLabel->setObjectName("titleLabel");

    // create a line edit for the email input
    createEmailEdit = new QLineEdit(createAccountPage);
    // set placeholder text for the email input
    createEmailEdit->setPlaceholderText("Email Address");

    // create a line edit for the new username input
    createUsernameEdit = new QLineEdit(createAccountPage);
    // set placeholder text for the username input
    createUsernameEdit->setPlaceholderText("Username");

    // create a line edit for the new password input
    createPasswordEdit = new QLineEdit(createAccountPage);
    // set placeholder text for the password input
    createPasswordEdit->setPlaceholderText("Password");
    // set the echo mode to hide the password characters
    createPasswordEdit->setEchoMode(QLineEdit::Password);

    // create a line edit for confirming the new password
    createConfirmPasswordEdit = new QLineEdit(createAccountPage);
    // set placeholder text for the confirm password input
    createConfirmPasswordEdit->setPlaceholderText("Confirm Password");
    // set the echo mode to hide the password characters
    createConfirmPasswordEdit->setEchoMode(QLineEdit::Password);

    // create a button to submit the account creation
    createAccountButton = new QPushButton("Create Account", createAccountPage);
    // create a button to navigate back to the login page
    toLoginButton = new QPushButton("Back to Login", createAccountPage);

    // add the widgets to the layout
    createLayout->addWidget(createLabel);
    createLayout->addWidget(createEmailEdit);
    createLayout->addWidget(createUsernameEdit);
    createLayout->addWidget(createPasswordEdit);
    createLayout->addWidget(createConfirmPasswordEdit);
    createLayout->addWidget(createAccountButton);
    createLayout->addWidget(toLoginButton);

    // set the layout for the create account page
    createAccountPage->setLayout(createLayout);

    // connect the create account button to a lambda function for handling account creation
    connect(createAccountButton, &QPushButton::clicked, [this]() {
        // get the input from the user
        QString email = createEmailEdit->text();
        QString username = createUsernameEdit->text();
        QString password = createPasswordEdit->text();
        QString confirmPassword = createConfirmPasswordEdit->text();

        // check if any fields are empty
        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            // show a warning message if any fields are empty
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        // check if the passwords match
        if (password != confirmPassword) {
            // show a warning message if passwords do not match
            QMessageBox::warning(this, "Input Error", "Passwords do not match.");
            return;
        }

        // simulate account creation by showing an information message
        QMessageBox::information(this, "Success", "Account created successfully!");
        // clear the input fields after creation
        createEmailEdit->clear();
        createUsernameEdit->clear();
        createPasswordEdit->clear();
        createConfirmPasswordEdit->clear();
        // navigate back to the login page
        stackedWidget->setCurrentWidget(loginPage);
    });

    // connect the back to login button to the navigateToLogin function
    connect(toLoginButton, &QPushButton::clicked, this, &MainWindow::navigateToLogin);
}

// function to set up the dashboard page
void MainWindow::setupDashboardPage()
{
    // create a new widget for the dashboard page
    dashboardPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *dashboardLayout = new QVBoxLayout;

    // create a label for the dashboard title
    QLabel *dashboardLabel = new QLabel("Dashboard");
    // center the label text
    dashboardLabel->setAlignment(Qt::AlignCenter);
    // set the object name for styling
    dashboardLabel->setObjectName("titleLabel");

    // create buttons for different dashboard functionalities
    viewReceiptsButton = new QPushButton("View Receipts", dashboardPage);
    viewAnalyticsButton = new QPushButton("View Analytics", dashboardPage);
    viewBudgetsButton = new QPushButton("View Budgets", dashboardPage);
    logoutButton = new QPushButton("Logout", dashboardPage);

    // add the widgets to the layout with some spacing
    dashboardLayout->addWidget(dashboardLabel);
    dashboardLayout->addSpacing(20);
    dashboardLayout->addWidget(viewReceiptsButton);
    dashboardLayout->addWidget(viewAnalyticsButton);
    dashboardLayout->addWidget(viewBudgetsButton);
    dashboardLayout->addStretch();
    dashboardLayout->addWidget(logoutButton);

    // set the layout for the dashboard page
    dashboardPage->setLayout(dashboardLayout);

    // connect the view receipts button to navigate to the receipts page
    connect(viewReceiptsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(receiptsPage);
    });
    // connect the view analytics button to navigate to the analytics page
    connect(viewAnalyticsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(analyticsPage);
    });
    // connect the view budgets button to navigate to the budgets page
    connect(viewBudgetsButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(budgetsPage);
    });
    // connect the logout button to handle logging out
    connect(logoutButton, &QPushButton::clicked, [this]() {
        // clear any sensitive information
        loginUsernameEdit->clear();
        loginPasswordEdit->clear();
        currentUsername.clear();
        // navigate back to the login page
        stackedWidget->setCurrentWidget(loginPage);
    });
}

// function to set up the receipts page
void MainWindow::setupReceiptsPage()
{
    // create a new widget for the receipts page
    receiptsPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *receiptsLayout = new QVBoxLayout;

    // Create a horizontal layout for the header (Label and Upload Button)
    QHBoxLayout *headerLayout = new QHBoxLayout;

    // create a label for the receipts title
    QLabel *receiptsLabel = new QLabel("Receipts");
    // center the label text vertically
    receiptsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // set the object name for styling
    receiptsLabel->setObjectName("titleLabel");

    // create the upload receipt button
    uploadReceiptButton = new QPushButton("Upload Receipt", receiptsPage);
    // Optionally, set an icon for the button
    // uploadReceiptButton->setIcon(QIcon(":/icons/upload.png"));

    // add the label and the button to the header layout
    headerLayout->addWidget(receiptsLabel);
    headerLayout->addStretch(); // Push the button to the right
    headerLayout->addWidget(uploadReceiptButton);

    // add the header layout to the main receipts layout
    receiptsLayout->addLayout(headerLayout);

    // create a table widget to display receipts
    receiptsTable = new QTableWidget(receiptsPage);
    // set the number of columns in the table
    receiptsTable->setColumnCount(3);
    // set the headers for each column
    receiptsTable->setHorizontalHeaderLabels({"Store", "Items", "Total"});
    // make the headers stretch to fill the available space
    receiptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // disable editing of the table cells
    receiptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // set the selection behavior to select entire rows
    receiptsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // allow only single row selection at a time
    receiptsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // create a button to go back to the dashboard
    backToDashboardButtonReceipts = new QPushButton("Back to Dashboard", receiptsPage);

    // add the table and back button to the main receipts layout
    receiptsLayout->addWidget(receiptsTable);
    receiptsLayout->addWidget(backToDashboardButtonReceipts);

    // set the layout for the receipts page
    receiptsPage->setLayout(receiptsLayout);

    // connect the back to dashboard button to navigate back to the dashboard
    connect(backToDashboardButtonReceipts, &QPushButton::clicked, this, &MainWindow::navigateToDashboard);

    // connect the upload receipt button to the handler
    connect(uploadReceiptButton, &QPushButton::clicked, this, &MainWindow::handleUploadReceipt);

    // populate the receipts table with mock data
    receiptsTable->setRowCount(2); // example with 2 receipts

    // first receipt entry
    receiptsTable->setItem(0, 0, new QTableWidgetItem("SuperMart"));
    receiptsTable->setItem(0, 1, new QTableWidgetItem("Milk, Bread, Eggs"));
    receiptsTable->setItem(0, 2, new QTableWidgetItem("15.75"));

    // second receipt entry
    receiptsTable->setItem(1, 0, new QTableWidgetItem("ElectroShop"));
    receiptsTable->setItem(1, 1, new QTableWidgetItem("Headphones"));
    receiptsTable->setItem(1, 2, new QTableWidgetItem("45.00"));
}

// New slot implementation
void MainWindow::handleUploadReceipt()
{
    // Open a file dialog to select an image file
    QString fileName = QFileDialog::getOpenFileName(this, "Select Receipt Image", "",
                                                    "Images (*.png *.xpm *.jpg *.jpeg *.bmp);;All Files (*)");
    if (!fileName.isEmpty()) {
        // For now, just display the selected file path in debug
        qDebug() << "Selected file:" << fileName;

        // Placeholder: In future, integrate OCR processing here
        // Example: Send the file path to Python OCR module and get the extracted data

        // For demonstration, let's add a mock entry to the receipts table
        int currentRow = receiptsTable->rowCount();
        receiptsTable->insertRow(currentRow);

        // Mock data - replace this with actual OCR data
        receiptsTable->setItem(currentRow, 0, new QTableWidgetItem("New Store"));
        receiptsTable->setItem(currentRow, 1, new QTableWidgetItem("Item1, Item2"));
        receiptsTable->setItem(currentRow, 2, new QTableWidgetItem("0.00"));

        // Optionally, you can store the file path or display the image
        // For example, you could add another column for the file path or display the image in a separate widget
    } else {
        // User canceled the dialog
        qDebug() << "No file selected.";
    }
}

// function to set up the analytics page
void MainWindow::setupAnalyticsPage()
{
    // create a new widget for the analytics page
    analyticsPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *analyticsLayout = new QVBoxLayout;

    // create a label for the analytics title
    QLabel *analyticsLabel = new QLabel("Analytics");
    // center the label text
    analyticsLabel->setAlignment(Qt::AlignCenter);
    // set the object name for styling
    analyticsLabel->setObjectName("titleLabel");

    // initialize a pie series for the chart with mock data
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Groceries", 150.50);
    pieSeries->append("Entertainment", 75.00);
    pieSeries->append("Utilities", 120.00);

    // create a chart and add the pie series to it
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    // set the title of the chart
    pieChart->setTitle("Spending Breakdown");
    // enable animations for the chart
    pieChart->setAnimationOptions(QChart::SeriesAnimations);

    // create a chart view to display the chart
    pieChartView = new QChartView(pieChart, analyticsPage);
    // enable antialiasing for smoother rendering
    pieChartView->setRenderHint(QPainter::Antialiasing);

    // create a button to go back to the dashboard
    backToDashboardButtonAnalytics = new QPushButton("Back to Dashboard", analyticsPage);

    // add the widgets to the layout
    analyticsLayout->addWidget(analyticsLabel);
    analyticsLayout->addWidget(pieChartView);
    analyticsLayout->addWidget(backToDashboardButtonAnalytics);

    // set the layout for the analytics page
    analyticsPage->setLayout(analyticsLayout);

    // connect the back to dashboard button to navigate back to the dashboard
    connect(backToDashboardButtonAnalytics, &QPushButton::clicked, this, &MainWindow::navigateToDashboard);
}

// function to set up the budgets page
void MainWindow::setupBudgetsPage()
{
    // create a new widget for the budgets page
    budgetsPage = new QWidget(this);
    // create a vertical layout for arranging widgets vertically
    QVBoxLayout *budgetsLayout = new QVBoxLayout;

    // create a label for the budgets title
    QLabel *budgetsLabel = new QLabel("Budgets");
    // center the label text
    budgetsLabel->setAlignment(Qt::AlignCenter);
    // set the object name for styling
    budgetsLabel->setObjectName("titleLabel");

    // create a placeholder label indicating future features
    QLabel *placeholderLabel = new QLabel("Budget management features will be implemented here.");
    // center the placeholder text
    placeholderLabel->setAlignment(Qt::AlignCenter);
    // set the style for the placeholder text
    placeholderLabel->setStyleSheet("color: #666666; font-style: italic;");

    // create a button to go back to the dashboard
    backToDashboardButtonBudgets = new QPushButton("Back to Dashboard", budgetsPage);

    // add the widgets to the layout with stretching
    budgetsLayout->addWidget(budgetsLabel);
    budgetsLayout->addStretch();
    budgetsLayout->addWidget(placeholderLabel);
    budgetsLayout->addStretch();
    budgetsLayout->addWidget(backToDashboardButtonBudgets);

    // set the layout for the budgets page
    budgetsPage->setLayout(budgetsLayout);

    // connect the back to dashboard button to navigate back to the dashboard
    connect(backToDashboardButtonBudgets, &QPushButton::clicked, this, &MainWindow::navigateToDashboard);
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
void MainWindow::handleLogin()
{
    // get the entered username and password
    QString username = loginUsernameEdit->text();
    QString password = loginPasswordEdit->text();

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

// function to handle navigation to the create account page
void MainWindow::handleCreateAccount()
{
    // navigate to the create account page
    stackedWidget->setCurrentWidget(createAccountPage);
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
