#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

// access the singleton instance
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

// private constructor initializes the QSqlDatabase instance
DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

// destructor closes the database connection if open
DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

// opens the database connection and creates tables if they do not exist
bool DatabaseManager::openDatabase(const QString &databasePath)
{
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "Database Connection Error: " << db.lastError().text();
        return false;
    } else {
        qDebug() << "Connected to the SQLite database successfully!";
        return createTables(); // create tables if the database opened successfully
    }
}

// getter for QSqlDatabase instance
QSqlDatabase& DatabaseManager::getDatabase()
{
    return db;
}

// creates necessary tables if they do not exist (if no existing database)
bool DatabaseManager::createTables()
{
    QSqlQuery query(db);

    // create the users table with salt
    QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            salt TEXT NOT NULL
        );
    )";
    if (!query.exec(createUsersTable)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // create the expense_category table
    QString createCategoryTable = R"(
        CREATE TABLE IF NOT EXISTS expense_category (
            category_id INTEGER PRIMARY KEY AUTOINCREMENT,
            category_name TEXT NOT NULL UNIQUE
        );
    )";
    if (!query.exec(createCategoryTable)) {
        qDebug() << "Error creating category table:" << query.lastError().text();
        return false;
    }

    // create the expenses table
    QString createExpensesTable = R"(
        CREATE TABLE IF NOT EXISTS expenses (
            expense_id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            category_id INTEGER,
            store TEXT,                -- Added 'store' field
            items TEXT,                -- Added 'items' field
            expense_date TEXT,
            expense_amount REAL,
            description TEXT,
            FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
            FOREIGN KEY (category_id) REFERENCES expense_category(category_id) ON DELETE SET NULL
        );
    )";
    if (!query.exec(createExpensesTable)) {
        qDebug() << "Error creating expenses table:" << query.lastError().text();
        return false;
    }

    // create the budgets table
    QString createBudgetsTable = R"(
    CREATE TABLE IF NOT EXISTS budgets (
        budget_id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER NOT NULL,
        category_id INTEGER NOT NULL,
        budget_amount REAL NOT NULL,
        start_date TEXT,
        end_date TEXT,
        FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
        FOREIGN KEY (category_id) REFERENCES expense_category(category_id) ON DELETE CASCADE
        );
    )";
    if (!query.exec(createBudgetsTable)) {
        qDebug() << "Error creating budgets table:" << query.lastError().text();
        return false;
    }

    // check if expense_category table is empty
    QString checkCategories = "SELECT COUNT(*) FROM expense_category";
    if (query.exec(checkCategories) && query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            // Insert default categories
            QStringList defaultCategories = {"Groceries", "Utilities", "Entertainment", "Travel", "Health"};
            for (const QString &category : defaultCategories) {
                addCategory(category);
            }
        }
    }

    return true;
}

// user related methods

bool DatabaseManager::createUser(const QString &username, const QString &hashedPassword, const QString &salt)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, salt) VALUES (:username, :password, :salt)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":salt", salt);

    if (!query.exec()) {
        qDebug() << "Error creating user:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<QPair<int, QString>> DatabaseManager::getAllCategories()
{
    QList<QPair<int, QString>> categories;
    QSqlQuery query(db);
    query.prepare("SELECT category_id, category_name FROM expense_category ORDER BY category_name ASC");
    if (query.exec()) {
        while (query.next()) {
            int id = query.value("category_id").toInt();
            QString name = query.value("category_name").toString();
            categories.append(qMakePair(id, name));
        }
    } else {
        qDebug() << "Error fetching categories:" << query.lastError().text();
    }
    return categories;
}


bool DatabaseManager::verifyUser(const QString &username, const QString &hashedPassword)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);

    if (!query.exec()) {
        qDebug() << "Error verifying user:" << query.lastError().text();
        return false;
    }

    return query.next(); // returns true if a record is found
}

bool DatabaseManager::getUserCredentials(const QString &username, QString &hashedPassword, QString &salt)
{
    QSqlQuery query(db);
    query.prepare("SELECT password, salt FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error fetching user credentials:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        hashedPassword = query.value("password").toString();
        salt = query.value("salt").toString();
        return true;
    } else {
        return false; // user not found
    }
}

// category related methods

bool DatabaseManager::addCategory(const QString &categoryName)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO expense_category (category_name) VALUES (:category_name)");
    query.bindValue(":category_name", categoryName);

    if (!query.exec()) {
        qDebug() << "Error adding category:" << query.lastError().text();
        return false;
    }
    return true;
}

// adds an expense to the expenses table
bool DatabaseManager::addExpense(int userId, int categoryId, const QString &store, const QString &date, double amount, const QString &description)
{
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO expenses (user_id, category_id, store, expense_date, expense_amount, description)
        VALUES (:user_id, :category_id, :store, :expense_date, :expense_amount, :description)
    )");
    query.bindValue(":user_id", userId);
    query.bindValue(":category_id", categoryId);
    query.bindValue(":store", store);
    query.bindValue(":expense_date", date);
    query.bindValue(":expense_amount", amount);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Error adding expense:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<QPair<QString, double>> DatabaseManager::getCategoryExpenses(int userId)
{
    QList<QPair<QString, double>> results;
    QSqlQuery query(db);
    query.prepare("SELECT expense_category.category_name, SUM(expenses.expense_amount) "
                  "FROM expenses "
                  "JOIN expense_category ON expenses.category_id = expense_category.category_id "
                  "WHERE expenses.user_id = :user_id "
                  "GROUP BY expense_category.category_name");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        while (query.next()) {
            QString categoryName = query.value(0).toString();
            double totalAmount = query.value(1).toDouble();
            results.append(qMakePair(categoryName, totalAmount));
        }
    } else {
        qDebug() << "Error retrieving category expenses:" << query.lastError().text();
    }
    return results;
}
