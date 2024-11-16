#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr); // constructor

signals:
    void loginRequested(const QString &username, const QString &password); // signal for login
    void navigateToCreateAccount(); // signal to go to account creation page

private:
    QLineEdit *loginUsernameEdit; // username input
    QLineEdit *loginPasswordEdit; // password input
    QPushButton *loginButton; // button to log in
    QPushButton *toCreateAccountButton; // button to navigate to account creation

    void setupUI(); // sets up the UI
};

#endif
