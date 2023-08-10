#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <string>
#include<vector>

using namespace std;

const char DELIMITER = '|';

struct book_record {
    string name;
    string author;
    string publisher;
    string cost;
    int quantity;
};

struct customer_record {
    string id;
    string name;
    string address;
    string contact;
    string bname;
    string cost;
    int quantity;
};

book_record rec[100];
customer_record cust_rec[100];
int total = 0, cust_total = 0;

void add_book() {
    ofstream outFile("books.txt", ios::app);
    if (!outFile) {
        cout << "\nError opening file. Cannot add book.\n";
        return;
    }

    cout << "\nEnter book details:\n";
    cout << "Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, rec[total].name);

    cout << "Author: ";
    getline(cin, rec[total].author);

    cout << "Publisher: ";
    getline(cin, rec[total].publisher);

    cout << "Cost: ";
    getline(cin, rec[total].cost);

    cout << "Quantity: ";
    cin >> rec[total].quantity;

    // Write book details with delimiter to file
    outFile << rec[total].name << DELIMITER
            << rec[total].author << DELIMITER
            << rec[total].publisher << DELIMITER
            << rec[total].cost << DELIMITER
            << rec[total].quantity << "$";

    total++;
    cout << "\nBook added successfully!\n";

    outFile.close();
}

void display_book_details(const book_record& book) {
    cout << "\n*** Book Details ***\n";
    cout << "Name: " << book.name << "\n";
    cout << "Author: " << book.author << "\n";
    cout << "Publisher: " << book.publisher << "\n";
    cout << "Cost: " << book.cost << "\n";
    cout << "Quantity: " << book.quantity << "\n";
}

void retrieve_details() {
    ifstream inFile("books.txt");
    if (!inFile) {
        cout << "\nError opening file. No book records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo book records found.\n";
        inFile.close();
        return;
    }

    string rt_name;
    cout << "\nEnter the book name you want to search for: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, rt_name);

    int found_flag = 0;
    while (inFile) {
        getline(inFile, rec[total].name, DELIMITER);
        getline(inFile, rec[total].author, DELIMITER);
        getline(inFile, rec[total].publisher, DELIMITER);
        getline(inFile, rec[total].cost, DELIMITER);
        inFile >> rec[total].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (rec[total].name == rt_name) {
            display_book_details(rec[total]);
            found_flag = 1;
            break;
        }
        total++;
    }

    inFile.close();

    if (found_flag == 0) {
        cout << "\nBook not found!\n";
    }
}

void display_all_book_details() {
    ifstream inFile("books.txt");
    if (!inFile) {
        cout << "\nError opening file. No book records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo book records found.\n";
        inFile.close();
        return;
    }

    cout << "\n*** All Book Details ***\n";

    total = 0; // Reset the total count

    vector<book_record> bookList; // Store non-empty book records

    while (inFile) {
        getline(inFile, rec[total].name, DELIMITER);
        getline(inFile, rec[total].author, DELIMITER);
        getline(inFile, rec[total].publisher, DELIMITER);
        getline(inFile, rec[total].cost, DELIMITER);
        inFile >> rec[total].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (inFile) {
            bookList.push_back(rec[total]); // Store non-empty book records
        }
        total++; // Increment total after reading book details
    }

    inFile.close();

    // Display non-empty book records
    for (const auto& book : bookList) {
        display_book_details(book);
    }
}


void purchase_book() {
    ifstream inFile("books.txt");
    if (!inFile) {
        cout << "\nError opening file. No book records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo book records found.\n";
        inFile.close();
        return;
    }

    string book_name;
    int quantity;
    display_all_book_details();
    cout << "\nEnter the book name you want to purchase: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, book_name);
    cout << "Enter the quantity: ";
    cin >> quantity;

    int found_flag = 0;
    int i = 0; // Initialize i before using it
    while (inFile) {
        getline(inFile, rec[i].name, DELIMITER);
        getline(inFile, rec[i].author, DELIMITER);
        getline(inFile, rec[i].publisher, DELIMITER);
        getline(inFile, rec[i].cost, DELIMITER);
        inFile >> rec[i].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (rec[i].name == book_name) {
            found_flag = 1;

            if (rec[i].quantity >= quantity) {
                // Update quantity
                rec[i].quantity -= quantity;

                // Capture customer details
                string id = "CUST" + to_string(cust_total + 1);
                cust_rec[cust_total].id = id;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter customer name: ";
                getline(cin, cust_rec[cust_total].name);

                cout << "Enter customer address: ";
                getline(cin, cust_rec[cust_total].address);

                cout << "Enter customer contact: ";
                getline(cin, cust_rec[cust_total].contact);

                cust_rec[cust_total].quantity = quantity;
                cust_rec[cust_total].bname = rec[i].name;
                cust_rec[cust_total].cost = rec[i].cost;
                cust_total++;

                // Save customer details to file
                ofstream outFile("customers.txt", ios::app);
                if (!outFile) {
                    cout << "\nError opening file. Cannot save customer details.\n";
                    return;
                }

                // Write customer details with delimiter to file
                outFile << cust_rec[cust_total - 1].id << DELIMITER
                        << cust_rec[cust_total - 1].name << DELIMITER
                        << cust_rec[cust_total - 1].address << DELIMITER
                        << cust_rec[cust_total - 1].contact << DELIMITER
                        << cust_rec[cust_total - 1].bname << DELIMITER
                        << cust_rec[cust_total - 1].cost << DELIMITER
                        << cust_rec[cust_total - 1].quantity << "$";

                outFile.close();

                // Update book quantity in the file
                inFile.close();
                ofstream updateFile("books.txt");
                if (!updateFile) {
                    cout << "\nError opening file. Cannot update book quantity.\n";
                    return;
                }

                for (int j = 0; j < total; j++) {
                    updateFile << rec[j].name << DELIMITER
                               << rec[j].author << DELIMITER
                               << rec[j].publisher << DELIMITER
                               << rec[j].cost << DELIMITER
                               << rec[j].quantity << "$";
                }

                updateFile.close();

                cout << "\nPurchase successful! Bill details:\n";
                cout << "Customer Name: " << cust_rec[cust_total - 1].name << "\n";
                cout << "Customer Address: " << cust_rec[cust_total - 1].address << "\n";
                cout << "Customer Contact: " << cust_rec[cust_total - 1].contact << "\n";
                cout << "Book Name: " << rec[i].name << "\n";
                cout << "Quantity: " << quantity << "\n";
                cout << "Total Cost: " << stoi(rec[i].cost) * quantity << "\n";
            } else {
                cout << "\nInsufficient quantity of the book!\n";
            }
            break;
        }
        i++; // Increment i after reading book details
    }

    inFile.close();

    if (found_flag == 0) {
        cout << "\nBook not found!\n";
    }
}

void display_customer_details() {
    ifstream inFile("customers.txt");
    if (!inFile) {
        cout << "\nError opening file. No customer records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo customer records found.\n";
        inFile.close();
        return;
    }

    string cust_name;
    cout << "\nEnter the customer name to display the details: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, cust_name);

    int found_flag = 0;
    int i = 0; // Initialize i before using it
    while (inFile) {
        getline(inFile, cust_rec[i].id, DELIMITER);
        getline(inFile, cust_rec[i].name, DELIMITER);
        getline(inFile, cust_rec[i].address, DELIMITER);
        getline(inFile, cust_rec[i].contact, DELIMITER);
        getline(inFile, cust_rec[i].bname, DELIMITER);
        getline(inFile, cust_rec[i].cost, DELIMITER);
        inFile >> cust_rec[i].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (cust_rec[i].name == cust_name) {
            cout << "\n**Customer Details**\n";
            cout << "Customer ID: " << cust_rec[i].id << "\n";
            cout << "Name: " << cust_rec[i].name << "\n";
            cout << "Address: " << cust_rec[i].address << "\n";
            cout << "Contact: " << cust_rec[i].contact << "\n";
            cout << "Book Name: " << cust_rec[i].bname << "\n";
            cout << "Cost: " << cust_rec[i].cost << "\n";
            cout << "Quantity: " << cust_rec[i].quantity << "\n";
            found_flag = 1;
            break;
        }
        i++; // Increment i after reading customer details
    }

    inFile.close();

    if (found_flag == 0) {
        cout << "\nCustomer not found!\n";
    }
}

void delete_book() {
    ifstream inFile("books.txt");
    if (!inFile) {
        cout << "\nError opening file. No book records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo book records found.\n";
        inFile.close();
        return;
    }

    string book_name;
    cout << "\nEnter the book name you want to delete: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, book_name);

    inFile.close();

    inFile.open("books.txt");
    ofstream tempFile("temp.txt");

    int found_flag = 0;

    while (inFile) {
        getline(inFile, rec[total].name, DELIMITER);
        getline(inFile, rec[total].author, DELIMITER);
        getline(inFile, rec[total].publisher, DELIMITER);
        getline(inFile, rec[total].cost, DELIMITER);
        inFile >> rec[total].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (rec[total].name == book_name) {
            found_flag = 1;
        } else {
            tempFile << rec[total].name << DELIMITER
                     << rec[total].author << DELIMITER
                     << rec[total].publisher << DELIMITER
                     << rec[total].cost << DELIMITER
                     << rec[total].quantity << "$";
        }
        total++;
    }

    inFile.close();
    tempFile.close();

    if (found_flag == 0) {
        cout << "\nBook not found!\n";
        remove("temp.txt"); // Delete temporary file
    } else {
        remove("books.txt");             // Delete original file
        rename("temp.txt", "books.txt"); // Rename temporary file
        cout << "\nBook deleted successfully!\n";
    }
}

void update_book() {
    ifstream inFile("books.txt");
    if (!inFile) {
        cout << "\nError opening file. No book records found.\n";
        return;
    }

    if (inFile.peek() == EOF) {
        cout << "\nNo book records found.\n";
        inFile.close();
        return;
    }

    string book_name;
    cout << "\nEnter the book name you want to update: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, book_name);

    inFile.close();

    inFile.open("books.txt");
    ofstream tempFile("temp.txt");

    int found_flag = 0;

    while (inFile) {
        getline(inFile, rec[total].name, DELIMITER);
        getline(inFile, rec[total].author, DELIMITER);
        getline(inFile, rec[total].publisher, DELIMITER);
        getline(inFile, rec[total].cost, DELIMITER);
        inFile >> rec[total].quantity;
        inFile.ignore(numeric_limits<streamsize>::max(), '$');

        if (rec[total].name == book_name) {
            cout << "Enter updated book details:\n";
            cout << "Name: ";
            getline(cin, rec[total].name);
            cout << "Author: ";
            getline(cin, rec[total].author);
            cout << "Publisher: ";
            getline(cin, rec[total].publisher);
            cout << "Cost: ";
            getline(cin, rec[total].cost);
            cout << "Quantity: ";
            cin >> rec[total].quantity;
            found_flag = 1;

            tempFile << rec[total].name << DELIMITER
                     << rec[total].author << DELIMITER
                     << rec[total].publisher << DELIMITER
                     << rec[total].cost << DELIMITER
                     << rec[total].quantity << "$";
        } else {
            tempFile << rec[total].name << DELIMITER
                     << rec[total].author << DELIMITER
                     << rec[total].publisher << DELIMITER
                     << rec[total].cost << DELIMITER
                     << rec[total].quantity << "$";
        }
        total++;
    }

    inFile.close();
    tempFile.close();

    if (found_flag == 0) {
        cout << "\nBook not found!\n";
        remove("temp.txt"); // Delete temporary file
    } else {
        remove("books.txt");             // Delete original file
        rename("temp.txt", "books.txt"); // Rename temporary file
        cout << "\nBook updated successfully!\n";
    }
}

void admin_menu() {
    int choice;
    while (true) {
        cout << "\n===== Admin Menu =====\n";
        cout << "1. Add Book\n";
        cout << "2. Retrieve Book Details\n";
        cout << "3. Display All Books\n";
        cout << "4. Delete Book\n";
        cout << "5. Update Book\n";
        cout << "6. Display All Customer Details\n";
        cout << "7. Exit\n";
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        switch (choice) {
        case 1:
            add_book();
            break;
        case 2:
            retrieve_details();
            break;
        case 3:
            display_all_book_details();
            break;
        case 4:
            delete_book();
            break;
        case 5:
            update_book();
            break;
        case 6:
            display_customer_details();
            break;
        case 7:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void customer_login() {
    int choice;
    while (true) {
        cout << "\n===== Customer Login =====\n";
        cout << "1. Purchase Book\n";
        cout << "2. Display Customer Details\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        switch (choice) {
        case 1:
            purchase_book();
            break;
        case 2:
            display_customer_details();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

int main() {
    int choice;
    while (true) {
        cout << "===== Bookstore Management System =====\n";
        cout << "1. Admin Menu\n";
        cout << "2. Customer Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        switch (choice) {
        case 1:
            admin_menu();
            break;
        case 2:
            customer_login();
            break;
        case 3:
            cout << "Thank you for using the Bookstore Management System. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

