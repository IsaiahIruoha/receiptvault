#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget> // for QWidget base class
#include "ui_LoginPage.h" // ui header for managing the ui design

class LoginPage : public QWidget
{
    Q_OBJECT // needed for signals and slots

public:
    explicit LoginPage(QWidget *parent = nullptr); // constructor to set up the widget
    ~LoginPage(); // destructor to clean up resources

signals:
    void loginRequested(const QString &username, const QString &password); // signal to request login with username and password
    void navigateToCreateAccount(); // signal to navigate to the create account page

private:
    Ui::LoginPage *ui; // pointer to ui elements for this page
};

#endif
