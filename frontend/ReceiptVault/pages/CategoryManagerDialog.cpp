#include "CategoryManagerDialog.h"

CategoryManagerDialog::CategoryManagerDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Manage Categories");
    setMinimumSize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // List Widget to display categories
    listWidgetCategories = new QListWidget(this);
    loadCategories();
    mainLayout->addWidget(listWidgetCategories);

    // Horizontal layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonAdd = new QPushButton("Add", this);
    buttonRename = new QPushButton("Rename", this);
    buttonDelete = new QPushButton("Delete", this);
    buttonClose = new QPushButton("Close", this);

    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonRename);
    buttonLayout->addWidget(buttonDelete);
    buttonLayout->addStretch(); // Push the Close button to the right
    buttonLayout->addWidget(buttonClose);

    mainLayout->addLayout(buttonLayout);

    // Connect buttons to their respective slots
    connect(buttonAdd, &QPushButton::clicked, this, &CategoryManagerDialog::addCategory);
    connect(buttonRename, &QPushButton::clicked, this, &CategoryManagerDialog::renameCategory);
    connect(buttonDelete, &QPushButton::clicked, this, &CategoryManagerDialog::deleteCategory);
    connect(buttonClose, &QPushButton::clicked, this, &CategoryManagerDialog::accept);

    setObjectName("CategoryManagerDialog");
}

void CategoryManagerDialog::loadCategories()
{
    listWidgetCategories->clear();
    QList<QPair<int, QString>> categories = DatabaseManager::instance().getAllCategories();
    for (const QPair<int, QString> &category : categories) {
        QListWidgetItem *item = new QListWidgetItem(category.second, listWidgetCategories);
        item->setData(Qt::UserRole, category.first); // Store category_id
    }
}

void CategoryManagerDialog::addCategory()
{
    bool ok;
    QString categoryName = QInputDialog::getText(this, "Add Category", "Category Name:", QLineEdit::Normal, "", &ok);
    if (ok && !categoryName.trimmed().isEmpty()) {
        if (DatabaseManager::instance().addCategory(categoryName.trimmed())) {
            QMessageBox::information(this, "Success", "Category added successfully.");
            loadCategories();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add category. It might already exist.");
        }
    }
}

void CategoryManagerDialog::renameCategory()
{
    QListWidgetItem *selectedItem = listWidgetCategories->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Selection Error", "Please select a category to rename.");
        return;
    }

    int categoryId = selectedItem->data(Qt::UserRole).toInt();
    QString currentName = selectedItem->text();

    bool ok;
    QString newName = QInputDialog::getText(this, "Rename Category", "New Category Name:", QLineEdit::Normal, currentName, &ok);
    if (ok && !newName.trimmed().isEmpty()) {
        if (DatabaseManager::instance().renameCategory(categoryId, newName.trimmed())) {
            QMessageBox::information(this, "Success", "Category renamed successfully.");
            loadCategories();
        } else {
            QMessageBox::critical(this, "Error", "Failed to rename category. The new name might already exist.");
        }
    }
}

void CategoryManagerDialog::deleteCategory()
{
    QListWidgetItem *selectedItem = listWidgetCategories->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Selection Error", "Please select a category to delete.");
        return;
    }

    int categoryId = selectedItem->data(Qt::UserRole).toInt();
    QString categoryName = selectedItem->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Category",
                                  QString("Are you sure you want to delete the category '%1'? This will set the category to 'Uncategorized' for any associated receipts.")
                                      .arg(categoryName),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteCategory(categoryId)) {
            QMessageBox::information(this, "Success", "Category deleted successfully.");
            loadCategories();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete category.");
        }
    }
}
