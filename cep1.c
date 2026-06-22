#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================== Macros & Constants ==================================
#define MAX_MENU_ITEMS 50
#define MAX_CUSTOMERS 100
#define MAX_ADMINS 10
#define MAX_CART_ITEMS 20
#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 256

#define MENU_FILE "menu.txt"
#define CUSTOMER_FILE "customers.txt"
#define ADMIN_FILE "admins.txt"
#define ORDERS_LOG "orders.txt"
#define FEEDBACK_FILE "feedback.txt"
#define ORDER_NUM_FILE "order_number.txt"

// =============================== Structures ==================================

typedef struct {
    char name[MAX_NAME_LENGTH];
    char category[MAX_NAME_LENGTH];
    float price;
} MenuItem;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[20];
} Customer;

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
} Admin;

typedef struct {
    int menuIndex;
    int quantity;
} CartItem;

// =============================== Global Variables ==================================
MenuItem menu[MAX_MENU_ITEMS];
int itemCount = 0;

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

Admin admins[MAX_ADMINS];
int adminCount = 0;

int orderNumber;
char loggedInAdmin[MAX_NAME_LENGTH];

// ============================== Function Prototypes ================================

// --- Helper Functions ---
void clearInputBuffer();
void clearScreen();
void pressEnterToContinue();
void createDefaultMenu();

// --- Load/Save Functions ---
void loadMenuFromFile();
void loadCustomersFromFile();
void loadAdminsFromFile();
void loadOrderNumber();
void loadAllData();
void saveMenuToFile();
void saveCustomersToFile();
void saveAdminsToFile();
void saveOrderNumber();
void saveAllData();
void saveToOrdersLog(char* orderType, Customer* cust, float totalBill);
void saveReceipt(CartItem cart[], int cartCount, char* orderType, char* customerName, float totalBill);

// --- Admin Functions ---
void adminLoginMenu();
void adminSignUp();
int adminSignIn();
void adminMenu();
void addItem();
void removeItemFromMenu();
void displayMenu();
void viewAllOrders();
void viewAllFeedbacks();

// --- Customer Functions ---
void customerMode();
Customer* getCustomerDetails();
void addCustomer(Customer* newCust);
void handleOrder(char* orderType, Customer* cust);
void addItemToCart(CartItem cart[], int* cartCount);
void removeItemFromCart(CartItem cart[], int* cartCount);
void viewCart(CartItem cart[], int cartCount);
float calculateTotal(CartItem cart[], int cartCount);
void leaveFeedback(Customer* cust);

// =============================== MAIN ========================================

int main() {
    loadAllData();
    
    int choice = 0;
    while (choice != 3) {
        clearScreen();
        printf("=====================================\n");
        printf("   WELCOME TO MULTI BITES RESTAURANT \n");
        printf("=====================================\n\n");
        printf("Please select your mode:\n");
        printf("1. Customer Mode\n");
        printf("2. Admin Mode\n");
        printf("3. Exit\n");
        printf("-------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            choice = 0;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                customerMode();
                break;
            case 2:
                adminLoginMenu();
                break;
            case 3:
                saveAllData();
                printf("\nThank you for visiting. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pressEnterToContinue();
                break;
        }
    }
    return 0;
}

// ========================== HELPER FUNCTIONS =================================

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
    // Note: system("clear") works on Linux/Mac. 
    // For Windows, you would use system("cls")
    system("clear"); // <-- FIX: Added comment
}

void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    getchar();
}

void createDefaultMenu() {
    printf("menu.txt not found. Creating a default menu...\n");
    
    strcpy(menu[0].name, "Garlic Bread");
    strcpy(menu[0].category, "Starters");
    menu[0].price = 5.99;

    strcpy(menu[1].name, "Dynamite Prawn");
    strcpy(menu[1].category, "Starters");
    menu[1].price = 7.50;

    strcpy(menu[2].name, "Hummus");
    strcpy(menu[2].category, "Main Course");
    menu[2].price = 15.99;

    strcpy(menu[3].name, "Pasta Carbonara");
    strcpy(menu[3].category, "Main Course");
    menu[3].price = 13.50;

    strcpy(menu[4].name, "Molten Lava");
    strcpy(menu[4].category, "Desserts");
    menu[4].price = 6.99;

    itemCount = 5;
    saveMenuToFile();
    printf("Default menu created.\n");
    pressEnterToContinue();
}

// ========================== LOAD/SAVE FUNCTIONS ==============================

void loadMenuFromFile() {
    FILE *filePtr = fopen(MENU_FILE, "r");
    if (filePtr == NULL) {
        createDefaultMenu();
        return;
    }
    itemCount = 0; 
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), filePtr) && itemCount < MAX_MENU_ITEMS) {
        char *token;
        
        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(menu[itemCount].name, token, MAX_NAME_LENGTH - 1);
            menu[itemCount].name[MAX_NAME_LENGTH - 1] = '\0'; // <-- FIX: Ensure null-termination
        } else continue;
        
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(menu[itemCount].category, token, MAX_NAME_LENGTH - 1);
            menu[itemCount].category[MAX_NAME_LENGTH - 1] = '\0'; // <-- FIX: Ensure null-termination
        } else continue;
        
        token = strtok(NULL, "\n");
        if (token != NULL) {
            menu[itemCount].price = (float)atof(token);
        } else continue;
        
        itemCount++;
    }
    fclose(filePtr);
}

void loadCustomersFromFile() {
    FILE *filePtr = fopen(CUSTOMER_FILE, "r");
    if (filePtr == NULL) { return; }
    customerCount = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), filePtr) && customerCount < MAX_CUSTOMERS) {
        char *token;
        
        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(customers[customerCount].name, token, MAX_NAME_LENGTH - 1);
            customers[customerCount].name[MAX_NAME_LENGTH - 1] = '\0'; // <-- FIX: Ensure null-termination
        } else continue;
        
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strncpy(customers[customerCount].phone, token, 19);
            customers[customerCount].phone[19] = '\0'; // <-- FIX: Ensure null-termination (for 20-char array)
        } else continue;
        
        customerCount++;
    }
    fclose(filePtr);
}

void loadAdminsFromFile() {
    FILE *filePtr = fopen(ADMIN_FILE, "r");
    if (filePtr == NULL) { return; }
    adminCount = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), filePtr) && adminCount < MAX_ADMINS) {
        char *token;
        
        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(admins[adminCount].username, token, MAX_NAME_LENGTH - 1);
            admins[adminCount].username[MAX_NAME_LENGTH - 1] = '\0'; // <-- FIX: Ensure null-termination
        } else continue;
        
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strncpy(admins[adminCount].password, token, MAX_NAME_LENGTH - 1);
            admins[adminCount].password[MAX_NAME_LENGTH - 1] = '\0'; // <-- FIX: Ensure null-termination
        } else continue;
        
        adminCount++;
    }
    fclose(filePtr);
}

void loadOrderNumber() {
    FILE *f = fopen(ORDER_NUM_FILE, "r");
    if (f == NULL) {
        orderNumber = 101; 
    } else {
        fscanf(f, "%d", &orderNumber);
        fclose(f);
    }
}

void loadAllData() {
    loadMenuFromFile();
    loadCustomersFromFile();
    loadAdminsFromFile();
    loadOrderNumber();
}

void saveMenuToFile() {
    FILE *filePtr = fopen(MENU_FILE, "w");
    if (filePtr == NULL) { printf("Error: Could not save menu file.\n"); return; }
    for (int i = 0; i < itemCount; i++) {
        fprintf(filePtr, "%s,%s,%.2f\n", menu[i].name, menu[i].category, menu[i].price);
    }
    fclose(filePtr);
}

void saveCustomersToFile() {
    FILE *filePtr = fopen(CUSTOMER_FILE, "w");
    if (filePtr == NULL) { printf("Error: Could not save customer file.\n"); return; }
    for (int i = 0; i < customerCount; i++) {
        fprintf(filePtr, "%s,%s\n", customers[i].name, customers[i].phone);
    }
    fclose(filePtr);
}

void saveAdminsToFile() {
    FILE *filePtr = fopen(ADMIN_FILE, "w");
    if (filePtr == NULL) { printf("Error: Could not save admin file.\n"); return; }
    for (int i = 0; i < adminCount; i++) {
        fprintf(filePtr, "%s,%s\n", admins[i].username, admins[i].password);
    }
    fclose(filePtr);
}

void saveOrderNumber() {
    FILE *f = fopen(ORDER_NUM_FILE, "w");
    if (f == NULL) {
        printf("Error: Could not save order number.\n");
        return;
    }
    fprintf(f, "%d", orderNumber);
    fclose(f);
}

void saveAllData() {
    saveMenuToFile();
    saveCustomersToFile();
    saveAdminsToFile();
    saveOrderNumber();
    printf("\nAll data has been saved successfully.\n");
}

void saveToOrdersLog(char* orderType, Customer* cust, float totalBill) {
    FILE *filePtr = fopen(ORDERS_LOG, "a");
    if (filePtr == NULL) { return; }
    
    fprintf(filePtr, "Order# %-5d | Type: %-10s | Cust: %-25s | Phone: %-15s | Total: $%.2f\n",
            orderNumber, 
            orderType, 
            cust->name, 
            cust->phone, 
            totalBill);
    
    fclose(filePtr);
}

void saveReceipt(CartItem cart[], int cartCount, char* orderType, char* customerName, float totalBill) {
    char receiptFilename[50];
    sprintf(receiptFilename, "Order_%d.txt", orderNumber); 

    FILE *receiptFile = fopen(receiptFilename, "w");
    if (receiptFile == NULL) { return; }

    fprintf(receiptFile, "=================================\n");
    fprintf(receiptFile, "          RECEIPT\n");
    fprintf(receiptFile, "=================================\n");
    fprintf(receiptFile, "Order Number: %d\n", orderNumber);
    fprintf(receiptFile, "Order Type:   %s\n", orderType);
    fprintf(receiptFile, "Customer:     %s\n", customerName);
    fprintf(receiptFile, "---------------------------------\n");
    fprintf(receiptFile, "Items:\n");
    
    for (int i = 0; i < cartCount; i++) {
        int index = cart[i].menuIndex;
        fprintf(receiptFile, "  %-25s (x%d) $%.2f\n", 
                menu[index].name, 
                cart[i].quantity,
                menu[index].price * cart[i].quantity);
    }
    
    fprintf(receiptFile, "---------------------------------\n");
    fprintf(receiptFile, "TOTAL: $%.2f\n", totalBill);
    fprintf(receiptFile, "\nThank you for your order!\n");
    
    fclose(receiptFile);
    printf("Receipt saved to %s\n", receiptFilename);
}

// ========================== ADMIN MODE FUNCTIONS =============================

void adminLoginMenu() {
    int choice = 0;
    while(choice != 3) {
        clearScreen();
        printf("==== Admin Mode ====\n");
        printf("1. Sign In\n");
        printf("2. Sign Up (New Admin)\n");
        printf("3. Back to Main Menu\n");
        printf("--------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) { choice = 0; }
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (adminSignIn() == 1) {
                    adminMenu();
                } else {
                    printf("\nLogin failed. Invalid username or password.\n");
                    pressEnterToContinue();
                }
                break;
            case 2:
                adminSignUp();
                break;
            case 3:
                return;
            default:
                printf("\nInvalid choice.\n");
                pressEnterToContinue();
                break;
        }
    }
}

void adminSignUp() {
    if (adminCount >= MAX_ADMINS) {
        printf("\nMaximum number of admins reached.\n");
        pressEnterToContinue();
        return;
    }
    
    clearScreen();
    printf("--- New Admin Sign Up ---\n");
    printf("Enter new username: ");
    fgets(admins[adminCount].username, MAX_NAME_LENGTH, stdin);
    admins[adminCount].username[strcspn(admins[adminCount].username, "\n")] = 0;

    for(int i = 0; i < adminCount; i++) {        //check admin already exist
        if(strcmp(admins[i].username, admins[adminCount].username) == 0) {
            printf("\nUsername already exists. Please try again.\n");
            pressEnterToContinue();
            return;
        }
    }

    printf("Enter new password: ");
    fgets(admins[adminCount].password, MAX_NAME_LENGTH, stdin);
    admins[adminCount].password[strcspn(admins[adminCount].password, "\n")] = 0;

    printf("\nAdmin account '%s' created successfully!\n", admins[adminCount].username);
    adminCount++;
    saveAdminsToFile();
    pressEnterToContinue();
}

int adminSignIn() {
    char user[MAX_NAME_LENGTH], pass[MAX_NAME_LENGTH];
    
    clearScreen();
    printf("--- Admin Sign In ---\n");
    printf("Username: ");
    fgets(user, MAX_NAME_LENGTH, stdin);
    user[strcspn(user, "\n")] = 0;
    
    printf("Password: ");
    fgets(pass, MAX_NAME_LENGTH, stdin);
    pass[strcspn(pass, "\n")] = 0;

    // ---
    // SECURITY NOTE: In a real-world app, passwords should be "hashed"
    // and not stored in plain text. For this project, this is okay.
    // ---
    for (int i = 0; i < adminCount; i++) {           //saved admin check
        if (strcmp(admins[i].username, user) == 0 && 
            strcmp(admins[i].password, pass) == 0) 
        {
            strcpy(loggedInAdmin, admins[i].username); //login admin name global variable m save
            printf("\nLogin successful. Welcome, %s!\n", loggedInAdmin);
            pressEnterToContinue();
            return 1;
        }
    }
    return 0;
}

void adminMenu() {
    int choice = 0;
    while(choice != 7) {
        clearScreen();
        printf("===========================================\n");
        printf("   Admin Dashboard (Logged in as: %s)\n", loggedInAdmin);
        printf("===========================================\n");
        printf("1. Display Full Menu\n");
        printf("2. Add New Menu Item\n");
        printf("3. Remove Menu Item\n");
        printf("4. View All Orders Log\n");
        printf("5. View All Feedbacks\n");
        printf("6. Save All Data\n");
        printf("7. Logout\n");
        printf("-------------------------------------------\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) { choice = 0; }
        clearInputBuffer();
        
        switch (choice) {
            case 1: displayMenu(); pressEnterToContinue(); break;
            case 2: addItem(); pressEnterToContinue(); break;
            case 3: removeItemFromMenu(); pressEnterToContinue(); break;
            case 4: viewAllOrders(); pressEnterToContinue(); break;
            case 5: viewAllFeedbacks(); pressEnterToContinue(); break;
            case 6: saveAllData(); pressEnterToContinue(); break;
            case 7:
                printf("\nLogging out...\n"); 
                strcpy(loggedInAdmin, "");
                pressEnterToContinue();
                return;
            default: printf("\nInvalid choice.\n"); pressEnterToContinue(); break;
        }
    }
}

void addItem() {
    if (itemCount >= MAX_MENU_ITEMS) { 
        printf("\nError: Maximum menu items reached.\n"); 
        return; 
    }

    clearScreen();
    printf("--- Add New Menu Item ---\n");
    printf("Enter name: ");
    fgets(menu[itemCount].name, MAX_NAME_LENGTH, stdin);
    menu[itemCount].name[strcspn(menu[itemCount].name, "\n")] = 0;
    
    printf("Enter category (Starters, Main Course, etc.): ");
    fgets(menu[itemCount].category, MAX_NAME_LENGTH, stdin);
    menu[itemCount].category[strcspn(menu[itemCount].category, "\n")] = 0;
    
    printf("Enter price: $");
    scanf("%f", &menu[itemCount].price);
    clearInputBuffer();
    
    printf("\nItem '%s' added!\n", menu[itemCount].name);
    itemCount++;
    saveMenuToFile();
}

void removeItemFromMenu() {
    if (itemCount == 0) {
        printf("\nMenu is already empty.\n");
        return;
    }

    clearScreen();
    printf("--- Remove Menu Item ---\n");
    displayMenu();
    
    int itemNum;
    printf("\nEnter the ID number of the item to remove: ");
    if (scanf("%d", &itemNum) != 1 || itemNum < 1 || itemNum > itemCount) {
        printf("\nInvalid ID. No item removed.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int indexToRemove = itemNum - 1;  //itemNum ko array m convert
    char removedName[MAX_NAME_LENGTH];  //removeditem ko print krwana hai
    strcpy(removedName, menu[indexToRemove].name);

    for (int i = indexToRemove; i < itemCount - 1; i++) {  //array continuous rahe
        menu[i] = menu[i + 1];
    }

    itemCount--;

    saveMenuToFile();
    
    printf("\nItem '%s' has been successfully removed.\n", removedName);
}


void displayMenu() {
    clearScreen();
    printf("=================================================================\n");
    printf("                       MULTI BITES Restaurant Menu\n");
    printf("=================================================================\n");
    if (itemCount == 0) { 
        printf("\nMenu is empty.\n"); 
        return; 
    }
    
    printf("\n %-3s | %-25s | %-15s | %s\n", "ID", "Name", "Category", "Price");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < itemCount; i++) {
        printf(" %-3d | %-25s | %-15s | $%.2f\n", 
                i + 1, menu[i].name, menu[i].category, menu[i].price); // arr0 se itemnum 1 se
    }
    printf("-----------------------------------------------------------------\n");
}

void viewAllOrders() {
    FILE *filePtr = fopen(ORDERS_LOG, "r");
    clearScreen();
    printf("==================================================================================================\n");
    printf("                                         Master Orders Log\n");
    printf("==================================================================================================\n");

    if (filePtr == NULL) {
        printf("\nNo orders have been logged yet.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), filePtr)) {  // file se aik item read krke screen m print
        printf("%s", line);
    }
    fclose(filePtr);
    printf("==================================================================================================\n");
}

void viewAllFeedbacks() {
    FILE *filePtr = fopen(FEEDBACK_FILE, "r");
    clearScreen();
    printf("=================================================\n");
    printf("               Customer Feedback Log\n");
    printf("=================================================\n");

    if (filePtr == NULL) {
        printf("\nNo feedback has been received yet.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), filePtr)) {
        printf("%s", line);
    }
    fclose(filePtr);
    printf("\n=================================================\n");
}

// ========================== CUSTOMER MODE FUNCTIONS ==========================

void customerMode() {
    int choice = 0;
    char orderTypeStr[20];
    
    clearScreen();
    printf("--- Welcome, Customer! ---\n");
    
    Customer* cust = getCustomerDetails();
    
    if (strcmp(cust->name, "Temp") == 0) {
        printf("Returning to main menu.\n");
        pressEnterToContinue();
        return;
    }

    while(choice != 4) {
        printf("\nPlease select your order type:\n");
        printf("1. Dine-In\n");
        printf("2. Takeaway\n");
        printf("3. Delivery\n");
        printf("4. Back to Main Menu\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) { 
            choice = 0;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: 
                strcpy(orderTypeStr, "Dine-In"); 
                break;
            case 2: 
                strcpy(orderTypeStr, "Takeaway"); 
                break;
            case 3: 
                strcpy(orderTypeStr, "Delivery"); 
                break;
            case 4:
                printf("\nReturning to main menu...\n");
                pressEnterToContinue();
                return;
            default:
                printf("\nInvalid choice. Please select 1-4.\n");
                pressEnterToContinue();
                continue;
        }
        
        handleOrder(orderTypeStr, cust); //customer se order leta hai total bill cal orderlog m save
        leaveFeedback(cust);
        pressEnterToContinue();
        return;
    }
}

Customer* getCustomerDetails() {
    if (customerCount >= MAX_CUSTOMERS) {
        printf("\nSorry, our customer database is full. Cannot proceed.\n");
        static Customer tempGuest = {"Temp", "N/A"};
        return &tempGuest; 
    }
    
    addCustomer(&customers[customerCount]); 
    return &customers[customerCount++]; 
}

void addCustomer(Customer* newCust) {
    printf("\n--- Customer Details ---\n");
    
    do {
        printf("Please enter your name (cannot be blank): ");
        fgets(newCust->name, MAX_NAME_LENGTH, stdin);
        newCust->name[strcspn(newCust->name, "\n")] = 0;

        if (newCust->name[0] == '\0') {
            printf("\nError: Name is mandatory. Please try again.\n\n");
        }
    } while (newCust->name[0] == '\0'); 
    
    do {
        printf("Please enter your 11-digit phone number: ");
        fgets(newCust->phone, 20, stdin);
        newCust->phone[strcspn(newCust->phone, "\n")] = 0; // <-- FIX: Corrected variable name

        if (strlen(newCust->phone) != 11) {
            printf("\nError: Phone number must be exactly 11 digits. Please try again.\n\n");
        }
    } while (strlen(newCust->phone) != 11);
    
    
    printf("\nDetails saved. Welcome, %s!\n", newCust->name);
    saveCustomersToFile();
    pressEnterToContinue();
}


void handleOrder(char* orderType, Customer* cust) {
    CartItem cart[MAX_CART_ITEMS];
    int cartCount = 0;
    int choice = 0;

    while (choice != 5) {
        clearScreen();
        printf("===========================================\n");
        printf("   Placing Order (%s) for %s\n", orderType, cust->name);
        printf("===========================================\n");
        printf("1. Display Full Menu\n");
        printf("2. Add Item to Your Order\n");
        printf("3. Remove Item from Your Order\n");
        printf("4. View Your Current Order\n");
        printf("5. Checkout\n");
        printf("-------------------------------------------\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) { choice = 0; }
        clearInputBuffer();

        switch (choice) {
            case 1: displayMenu(); pressEnterToContinue(); break;
            case 2: addItemToCart(cart, &cartCount); break;
            case 3: removeItemFromCart(cart, &cartCount); break;
            case 4: viewCart(cart, cartCount); pressEnterToContinue(); break;
            case 5:
                if(cartCount == 0) {
                    printf("\nYour cart is empty. Cannot checkout.\n");
                    choice = 0;
                    pressEnterToContinue();
                } else {
                    float totalBill = calculateTotal(cart, cartCount);
                    clearScreen();
                    printf("--- Checkout Summary ---\n");
                    viewCart(cart, cartCount);
                    printf("-------------------------------------------------\n");
                    printf("Total Bill: $%.2f\n", totalBill);
                    printf("Processing payment... (dummy)\n");
                    
                    saveReceipt(cart, cartCount, orderType, cust->name, totalBill);
                    saveToOrdersLog(orderType, cust, totalBill); 
                    
                    orderNumber++;
                    saveOrderNumber(); 
                    
                    printf("\nOrder Confirmed! Thank you!\n");
                }
                break;
            default: printf("\nInvalid choice.\n"); pressEnterToContinue(); break;
        }
    }
}

void addItemToCart(CartItem cart[], int* cartCount) { 
    displayMenu();
    int itemNum, quantity;
    
    printf("\nEnter item ID number to add: ");
    if (scanf("%d", &itemNum) != 1 || itemNum < 1 || itemNum > itemCount) {
        printf("\nInvalid item number.\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    int menuIndex = itemNum - 1; 
    
    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity < 1) {
        printf("\nInvalid quantity.\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < *cartCount; i++) {    //Check if item already exist in cart 
        if (cart[i].menuIndex == menuIndex) { // same item exist in cart
            cart[i].quantity += quantity;     // existing item quantity add
            printf("\nAdded %d more '%s'.\n", quantity, menu[menuIndex].name);
            pressEnterToContinue();
            return;
        }
    }

    if (*cartCount < MAX_CART_ITEMS) {
        cart[*cartCount].menuIndex = menuIndex;
        cart[*cartCount].quantity = quantity;
        (*cartCount)++;
        printf("\nAdded %d x '%s' to your order.\n", quantity, menu[menuIndex].name);
    } else {
        printf("\nSorry, your order cart is full.\n");
    }
    
    pressEnterToContinue();
}

void removeItemFromCart(CartItem cart[], int* cartCount) {
    if (*cartCount == 0) {
        printf("\nYour cart is already empty.\n");
        pressEnterToContinue();
        return;
    }
    
    viewCart(cart, *cartCount);
    
    int cartItemNum;
    printf("\nEnter item number (from list above) to remove: ");
    if (scanf("%d", &cartItemNum) != 1 || cartItemNum < 1 || cartItemNum > *cartCount) {
        printf("\nInvalid cart item number.\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();
    
    int cartIndex = cartItemNum - 1;
    int currentQty = cart[cartIndex].quantity;
    int qtyToRemove;

    // Ask how many to remove
    printf("Current quantity of '%s' is %d.\n", menu[cart[cartIndex].menuIndex].name, currentQty);
    printf("Enter quantity to remove (1 to %d): ", currentQty);
    if (scanf("%d", &qtyToRemove) != 1 || qtyToRemove < 1 || qtyToRemove > currentQty) {
        printf("\nInvalid quantity to remove.\n");
        clearInputBuffer();
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    if (qtyToRemove < currentQty) {
        cart[cartIndex].quantity -= qtyToRemove;
        printf("\nRemoved %d of '%s'. New quantity: %d\n", qtyToRemove, menu[cart[cartIndex].menuIndex].name, cart[cartIndex].quantity);
    } else {
        // remove entire cart line
        char removedName[MAX_NAME_LENGTH];
        strcpy(removedName, menu[cart[cartIndex].menuIndex].name);
        for (int i = cartIndex; i < *cartCount - 1; i++) {
            cart[i] = cart[i + 1];
        }
        (*cartCount)--;
        printf("\nRemoved all of '%s' from your order.\n", removedName);
    }
    
    pressEnterToContinue();
}

void viewCart(CartItem cart[], int cartCount) {
    clearScreen();
    printf("=================================================\n");
    printf("                Your Current Order\n");
    printf("=================================================\n");

    if (cartCount == 0) {
        printf("\nYour order is currently empty.\n");
        return;
    }
    
    printf("\n %-3s | %-25s | %-8s | %s\n", "No.", "Name", "Quantity", "Total");
    printf("-------------------------------------------------\n");
    
    float subtotal = 0.0;
    for (int i = 0; i < cartCount; i++) {
        int index = cart[i].menuIndex;
        float itemTotal = menu[index].price * cart[i].quantity;
        printf(" %-3d | %-25s | %-8d | $%.2f\n",  i + 1, menu[index].name, cart[i].quantity, itemTotal);
        subtotal += itemTotal;
    }
    printf("-------------------------------------------------\n");
    printf("Subtotal: $%.2f\n", subtotal);
}

float calculateTotal(CartItem cart[], int cartCount) {
    float total = 0.0;
    for (int i = 0; i < cartCount; i++) {
        int index = cart[i].menuIndex;
        total += menu[index].price * cart[i].quantity;
    }
    return total;
}

void leaveFeedback(Customer* cust) {
    int rating = 0;
    char comment[MAX_LINE_LENGTH];

    printf("\n--- We Value Your Feedback! ---\n");
    
    do {
        printf("Please rate your experience (1-5 stars): ");
        if (scanf("%d", &rating) != 1) {
            rating = 0;
            clearInputBuffer();
        }
    } while (rating < 1 || rating > 5);
    clearInputBuffer();
    
    printf("Enter any comments (press Enter to skip):\n");
    fgets(comment, MAX_LINE_LENGTH, stdin);

    FILE *fbFile = fopen(FEEDBACK_FILE, "a");
    if (fbFile == NULL) { return; }

    fprintf(fbFile, "Customer: %s\n", cust->name);
    fprintf(fbFile, "Rating: %d stars\n", rating);
    fprintf(fbFile, "Comment: %s", comment);
    fprintf(fbFile, "--------------------\n");
    fclose(fbFile);

    printf("\nThank you for your feedback!\n");
}