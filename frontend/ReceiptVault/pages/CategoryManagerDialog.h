#ifndef CATEGORYMANAGERDIALOG_H
#define CATEGORYMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "DatabaseManager.h"

class CategoryManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CategoryManagerDialog(QWidget *parent = nullptr);

private slots:
    void addCategory();
    void renameCategory();
    void deleteCategory();

private:
    QListWidget *listWidgetCategories;
    QPushButton *buttonAdd;
    QPushButton *buttonRename;
    QPushButton *buttonDelete;
    QPushButton *buttonClose;

    void loadCategories();
};

#endif
