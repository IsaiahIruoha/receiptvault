#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QHeaderView>
#include "pages/DatabaseManager.h"
#include <QCryptographicHash>
#include <QInputDialog>

using namespace Qt;

// constructor for MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // use DatabaseManager to open the database
    if (!DatabaseManager::instance().openDatabase("../../../../../../../backend/db/receiptvault.db")) {
        QMessageBox::critical(this, "Database Connection Error", "Unable to connect to the database.");
        exit(EXIT_FAILURE);
    }

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

    // navigate to CreateAccountPage
    connect(loginPage, &LoginPage::navigateToCreateAccount, [this]() {
        stackedWidget->setCurrentWidget(createAccountPage);
    });

    // navigate back to LoginPage
    connect(createAccountPage, &CreateAccountPage::navigateToLogin, [this]() {
        stackedWidget->setCurrentWidget(loginPage);
    });

    // connect signals for LoginPage
    connect(loginPage, &LoginPage::loginRequested, this, &MainWindow::handleLogin);

    // connect signals for CreateAccountPage
    connect(createAccountPage, &CreateAccountPage::accountCreationRequested, this, &MainWindow::handleCreateAccount);

    // connect signals for DashboardPage
    connect(dashboardPage, &DashboardPage::navigateToReceipts, [this]() {
        if (currentUserId != -1) {
            receiptsPage->loadReceipts(currentUserId);
            stackedWidget->setCurrentWidget(receiptsPage);
        } else {
            QMessageBox::warning(this, "Error", "User ID not found. Please log in again.");
        }
    });

    connect(dashboardPage, &DashboardPage::navigateToAnalytics, this, &MainWindow::handleNavigateToAnalytics);

    connect(dashboardPage, &DashboardPage::navigateToBudgets, [this]() {
        if (currentUserId != -1) {
            budgetsPage->loadBudgets(currentUserId);
            stackedWidget->setCurrentWidget(budgetsPage);
        } else {
            QMessageBox::warning(this, "Error", "User ID not found. Please log in again.");
        }
    });

    connect(dashboardPage, &DashboardPage::logoutRequested, [this]() {
        // clear any sensitive information
        loginPage->findChild<QLineEdit*>("loginUsernameEdit")->clear();
        loginPage->findChild<QLineEdit*>("loginPasswordEdit")->clear();
        currentUsername.clear();
        currentUserId = -1; // reset currentUserId
        // navigate back to the login page
        stackedWidget->setCurrentWidget(loginPage);
    });



    // connect signals for ReceiptsPage
    connect(receiptsPage, &ReceiptsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);
    connect(receiptsPage, &ReceiptsPage::uploadReceipt, this, &MainWindow::handleUploadReceipt);

    // connect signals for AnalyticsPage and BudgetsPage
    connect(analyticsPage, &AnalyticsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);
    connect(budgetsPage, &BudgetsPage::navigateToDashboard, this, &MainWindow::navigateToDashboard);

    connect(receiptsPage, &ReceiptsPage::editReceipt, this, &MainWindow::handleEditReceipt);

}


// destructor for MainWindow
MainWindow::~MainWindow()
{
}

// function to apply external styles to the application
void MainWindow::applyStyles()
{
    // load the external stylesheet from resources
    QFile styleFile(":/styles/style.qss"); // Path to the style.qss in resources
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
    // Get the current user ID
    int userId = getCurrentUserId();
    if (userId == -1) {
        QMessageBox::warning(this, "Error", "User not found. Please log in again.");
        return;
    }

    // Open a file dialog to select an image file
    QString fileName = QFileDialog::getOpenFileName(this, "Select Receipt Image", "",
                                                    "Images (*.pdf *.png *.jpg *.jpeg);;All Files (*)");
    if (!fileName.isEmpty()) {
        // Display the selected file path in debug
        qDebug() << "Selected file:" << fileName;

        // Placeholder: In future, integrate OCR processing here to extract store, total, and date
        // For demonstration, we'll use mock data
        QString store = "New Store";            // Replace with OCR-extracted or user-input data
        double totalAmount = 75.50;             // Replace with OCR-extracted or user-input data
        QString description = "Receipt uploaded"; // Can be a default value or extracted
        QString date = QDate::currentDate().toString("yyyy-MM-dd"); // Replace with extracted date

        // Assign a default category or allow the user to select
        // For better flexibility, prompt the user to select a category
        QComboBox *categoryComboBox = new QComboBox(this);
        receiptsPage->populateCategoryComboBox(categoryComboBox);
        bool ok;
        int selectedCategoryId = categoryComboBox->currentData().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Please select a valid category.");
            delete categoryComboBox;
            return;
        }

        // Add the expense to the database with all required arguments
        bool success = DatabaseManager::instance().addExpense(userId, selectedCategoryId, store, date, totalAmount, description);

        if (success) {
            // Retrieve the last inserted expense_id
            QSqlQuery query(DatabaseManager::instance().getDatabase());
            query.exec("SELECT last_insert_rowid()"); // For SQLite. Use appropriate method for other DBs.
            int expenseId = -1;
            if (query.next()) {
                expenseId = query.value(0).toInt();
            }

            if(expenseId != -1){
                QString totalStr = QString::number(totalAmount, 'f', 2);
                receiptsPage->addReceipt(store, totalStr, date, selectedCategoryId, expenseId);

                QMessageBox::information(this, "Success", "Receipt uploaded successfully!");
            } else {
                QMessageBox::critical(this, "Error", "Failed to retrieve expense ID.");
            }
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to upload receipt.");
        }

        delete categoryComboBox; // Clean up
    } else {
        // User canceled the dialog
        qDebug() << "No file selected.";
    }
}

// function to get the current user's ID
int MainWindow::getCurrentUserId()
{
    return currentUserId;
}

void MainWindow::handleNavigateToAnalytics()
{
    int userId = getCurrentUserId();
    if (userId == -1) {
        QMessageBox::warning(this, "Error", "User not found. Please log in again.");
        return;
    }

    // fetch category expenses from the database
    QList<QPair<QString, double>> categoryExpenses = DatabaseManager::instance().getCategoryExpenses(userId);

    // update the AnalyticsPage with the fetched data
    analyticsPage->updateChartData(categoryExpenses);

    // navigate to the AnalyticsPage
    stackedWidget->setCurrentWidget(analyticsPage);
}

void MainWindow::handleLogin(const QString &username, const QString &password)
{
    // check if either the username or password fields are empty
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    // fetch stored hashed password and salt
    QString storedHashedPassword, storedSalt;
    bool userExists = DatabaseManager::instance().getUserCredentials(username, storedHashedPassword, storedSalt);

    if (!userExists) {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
        return;
    }

    // hash the entered password with the stored salt
    QByteArray saltedPassword = password.toUtf8() + storedSalt.toUtf8();
    QByteArray hashedPassword = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256).toHex();

    // verify user
    bool verified = DatabaseManager::instance().verifyUser(username, hashedPassword);

    if (verified) {
        // successful login
        QMessageBox::information(this, "Success", "Login successful!");
        currentUsername = username;

        // retrieve and store user ID
        QSqlQuery query(DatabaseManager::instance().getDatabase());
        query.prepare("SELECT user_id FROM users WHERE username = :username");
        query.bindValue(":username", currentUsername);
        if (query.exec() && query.next()) {
            currentUserId = query.value("user_id").toInt();
        } else {
            qDebug() << "Failed to retrieve user ID:" << query.lastError().text();
            QMessageBox::critical(this, "Error", "Failed to retrieve user information.");
            return;
        }

        stackedWidget->setCurrentWidget(dashboardPage);
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
    }
}

void MainWindow::handleCreateAccount(const QString &username, const QString &password)
{

    // generate a unique salt (salting enhances the security of hashed passwords by adding random data)
    QString salt = QString::number(QDateTime::currentMSecsSinceEpoch());

    // hash the password with the salt using SHA-256
    QByteArray saltedPassword = password.toUtf8() + salt.toUtf8();
    QByteArray hashedPassword = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256).toHex();

    // insert into the database
    bool success = DatabaseManager::instance().createUser(username, hashedPassword, salt);

    if (success) {
        QMessageBox::information(this, "Success", "Account created successfully!");
        stackedWidget->setCurrentWidget(loginPage);
    } else {
        QMessageBox::critical(this, "Registration Error", "Failed to create account. Username might already exist.");
    }
}

void MainWindow::handleEditReceipt(int expenseId)
{
    // Fetch receipt details from the database
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare("SELECT store, expense_amount, expense_date, category_id FROM expenses WHERE expense_id = :expense_id AND user_id = :user_id");
    query.bindValue(":expense_id", expenseId);
    query.bindValue(":user_id", currentUserId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Error", "Failed to retrieve receipt details.");
        return;
    }

    QString store = query.value("store").toString();
    double amount = query.value("expense_amount").toDouble();
    QString date = query.value("expense_date").toString();
    int categoryId = query.value("category_id").toInt();

    // Prompt user to edit details using QInputDialog
    bool ok;
    QString newStore = QInputDialog::getText(this, "Edit Store", "Store:", QLineEdit::Normal, store, &ok);
    if (!ok || newStore.isEmpty()) return;

    QString newAmountStr = QInputDialog::getText(this, "Edit Total", "Total Amount:", QLineEdit::Normal, QString::number(amount, 'f', 2), &ok);
    if (!ok) return;
    double newAmount = newAmountStr.toDouble(&ok);
    if (!ok || newAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid amount.");
        return;
    }

    QString newDate = QInputDialog::getText(this, "Edit Date", "Date (YYYY-MM-DD):", QLineEdit::Normal, date, &ok);
    if (!ok || !QDate::fromString(newDate, "yyyy-MM-dd").isValid()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid date.");
        return;
    }

    // Update the database
    QSqlQuery updateQuery(DatabaseManager::instance().getDatabase());
    updateQuery.prepare(R"(
        UPDATE expenses
        SET store = :store, expense_amount = :amount, expense_date = :date, category_id = :category_id
        WHERE expense_id = :expense_id AND user_id = :user_id
    )");
    updateQuery.bindValue(":store", newStore);
    updateQuery.bindValue(":amount", newAmount);
    updateQuery.bindValue(":date", newDate);
    updateQuery.bindValue(":category_id", categoryId); // Update as needed
    updateQuery.bindValue(":expense_id", expenseId);
    updateQuery.bindValue(":user_id", currentUserId);

    if (updateQuery.exec()) {
        QMessageBox::information(this, "Success", "Receipt updated successfully!");
        receiptsPage->loadReceipts(currentUserId);
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to update receipt.");
        qDebug() << "Update error:" << updateQuery.lastError().text();
    }
}
