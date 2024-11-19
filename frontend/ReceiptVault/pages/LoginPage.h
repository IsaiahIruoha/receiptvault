#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "ui_LoginPage.h"

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr); // constructor
    ~LoginPage(); // destructor

signals:
    void loginRequested(const QString &username, const QString &password); // signal for login
    void navigateToCreateAccount(); // signal to go to account creation page

private:
    Ui::LoginPage *ui;
};

#endif
