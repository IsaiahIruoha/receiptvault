#ifndef CREATEACCOUNTPAGE_H
#define CREATEACCOUNTPAGE_H

#include <QWidget>
#include "ui_CreateAccountPage.h"

class CreateAccountPage : public QWidget
{
    Q_OBJECT
public:
    explicit CreateAccountPage(QWidget *parent = nullptr);
    ~CreateAccountPage();

signals:
    void accountCreationRequested(const QString &username, const QString &password);
    void navigateToLogin();

private:
    Ui::CreateAccountPage *ui;
};

#endif
