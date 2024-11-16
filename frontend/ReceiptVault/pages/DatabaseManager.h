#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QPair>

class DatabaseManager
{
public:
    static DatabaseManager& instance(); // gets singleton instance

    bool openDatabase(const QString &databasePath); // opens db and creates tables if needed
    QSqlDatabase& getDatabase(); // gets the database object

    // user-related methods
    bool createUser(const QString &username, const QString &hashedPassword, const QString &salt); // creates user
    bool verifyUser(const QString &username, const QString &hashedPassword); // verifies user login
    bool getUserCredentials(const QString &username, QString &hashedPassword, QString &salt); // fetches user creds

    // category-related methods
    bool addCategory(const QString &categoryName); // adds a new category

    // expense-related methods
    bool addExpense(int userId, int categoryId, const QString &store, const QString &date, double amount, const QString &description); // adds an expense
    QList<QPair<QString, double>> getCategoryExpenses(int userId); // gets expense totals by category
    QList<QPair<int, QString>> getAllCategories(); // fetches all categories

private:
    DatabaseManager(); // private constructor
    ~DatabaseManager(); // private destructor

    DatabaseManager(const DatabaseManager&) = delete; // disallow copy
    DatabaseManager& operator=(const DatabaseManager&) = delete; // disallow assignment

    bool createTables(); // creates required tables
    QSqlDatabase db; // the database instance
};

#endif
