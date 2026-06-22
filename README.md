#  Restaurant Management System (C Language)

##  Overview

This project is a **console-based Restaurant Management System** developed in C. It is designed to automate restaurant operations by providing separate interfaces for **Admin** and **Customer**. The system uses **file handling** to store and manage data persistently.

---

##  Features

###  Admin Mode

* Admin Sign Up & Login
* Add new menu items
* Remove menu items
* View full menu
* View all customer orders
* View customer feedback
* Save system data

###  Customer Mode

* Enter customer details
* Choose order type:

  * Dine-In
  * Takeaway
  * Delivery
* Browse menu
* Add/remove items from cart
* View cart and bill
* Generate receipt
* Leave feedback

---

##  System Functionalities

* File-based data storage:

  * `menu.txt`
  * `customers.txt`
  * `admins.txt`
  * `orders.txt`
  * `feedback.txt`
* Automatic order number tracking
* Receipt generation for each order
* Input validation for better reliability

---

##  Technologies Used

* Programming Language: **C**
* Concepts:

  * Structures
  * File Handling
  * Arrays
  * Functions
  * String Manipulation

---

##  How to Run

1. Compile the code:

   ```bash
   gcc main.c -o restaurant
   ```

2. Run the program:

   ```bash
   ./restaurant
   ```

---

##  Project Structure

* `main.c` → Main program file
* `menu.txt` → Stores menu items
* `customers.txt` → Stores customer data
* `admins.txt` → Stores admin credentials
* `orders.txt` → Stores order logs
* `feedback.txt` → Stores customer feedback

---

##  Notes

* This is a **console-based academic project**
* Admin passwords are stored in plain text (not secure for real-world use)
* Works best in Linux/Mac (for `system("clear")`)

---

##  Future Improvements

* GUI-based interface
* Database integration (MySQL)
* Secure password encryption
* Online ordering system

---

##  Author
 Azka Azhar

Developed as a learning project for Data Structures & Programming fundamentals.
