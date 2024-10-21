-- Database Initialization Script for Budget Planner

-- Create the budget_planner database
CREATE DATABASE IF NOT EXISTS budget_planner;
USE budget_planner;

-- Create the expense_category table
CREATE TABLE IF NOT EXISTS expense_category (
    category_id INT NOT NULL AUTO_INCREMENT,
    category_name VARCHAR(45) NOT NULL,
    PRIMARY KEY (category_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Create the users table
CREATE TABLE IF NOT EXISTS users (
    user_id INT NOT NULL AUTO_INCREMENT,
    email VARCHAR(255) NOT NULL UNIQUE,
    date_joined VARCHAR(45),
    user_role VARCHAR(45),
    username VARCHAR(45) NOT NULL,
    password VARCHAR(255) NOT NULL,
    PRIMARY KEY (user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Create the expenses table
CREATE TABLE IF NOT EXISTS expenses (
    expense_id INT NOT NULL AUTO_INCREMENT,
    user_id INT NOT NULL,
    category_id INT NOT NULL,
    expense_date DATE,
    expense_amount DECIMAL(10, 2),
    description VARCHAR(255),
    PRIMARY KEY (expense_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES expense_category(category_id) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Add some default categories
INSERT INTO expense_category (category_name) VALUES ('Food'), ('Transport'), ('Utilities');

-- Commit changes
COMMIT;
