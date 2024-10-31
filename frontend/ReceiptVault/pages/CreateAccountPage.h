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
    void accountCreationRequested(const QString &email, const QString &username, const QString &password);
    void navigateToLogin();

private:
    QLineEdit *createEmailEdit;
    QLineEdit *createUsernameEdit;
    QLineEdit *createPasswordEdit;
    QLineEdit *createConfirmPasswordEdit;
    QPushButton *createAccountButton;
    QPushButton *toLoginButton;

    void setupUI();
};

#endif // CREATEACCOUNTPAGE_H
