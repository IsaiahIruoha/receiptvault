#ifndef CREATEACCOUNTPAGE_H
#define CREATEACCOUNTPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class CreateAccountPage : public QWidget
{
    Q_OBJECT
public:
    explicit CreateAccountPage(QWidget *parent = nullptr);

signals:
    void accountCreationRequested(const QString &username, const QString &password); // signal for account creation
    void navigateToLogin(); // signal to navigate back to login

private:
    QLineEdit *createUsernameEdit; // input for username
    QLineEdit *createPasswordEdit; // input for password
    QLineEdit *createConfirmPasswordEdit; // input to confirm password
    QPushButton *createAccountButton; // button to create account
    QPushButton *toLoginButton; // button to go to login page

    void setupUI(); // sets up the ui
};

#endif
