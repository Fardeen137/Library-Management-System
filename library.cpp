#include <bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    int id;
};

class Book {
private:
    int bookId;
    string title;
    string author;
    bool available;
    Student* borrower;
public:
    Book(int id, string t, string a) : bookId(id), title(t), author(a), available(true), borrower(nullptr) {}

    int getBookId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }
    void setAvailability(bool status) { available = status; }
    void setBorrower(Student* student) { borrower = student; }
    Student* getBorrower() const { return borrower; }
};

class Library {
private:
    vector<Book> inventory;
    int nextBookId;
    queue<pair<Book*, Student*> > borrowedBooks;
public:
    Library() : nextBookId(1) {}

    // Function to add a new book
    void addBook() {
        string title, author;
        cout << "Enter the book title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter the author: ";
        getline(cin, author);
        inventory.emplace_back(nextBookId, title, author);
        cout << "Book added. ID: " << nextBookId << endl;
        nextBookId++;
    }

    // Function to search for a book
    void findBook() {
        string query;
        cout << "Enter book title or ID: ";
        cin.ignore();
        getline(cin, query);
        bool found = false;
        try {
            int id = stoi(query);
            for (const auto& book : inventory) {
                if (book.getBookId() == id) {
                    displayBookInfo(book);
                    found = true;
                    break;
                }
            }
        } catch (invalid_argument&) {
            for (const auto& book : inventory) {
                if (book.getTitle() == query) {
                    displayBookInfo(book);
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "Book not found." << endl;
        }
    }

    // Function to issue a book to a student
    void borrowBook() {
        int bookId;
        cout << "Enter the book ID to borrow: ";
        cin >> bookId;
        for (auto& book : inventory) {
            if (book.getBookId() == bookId) {
                if (book.isAvailable()) {
                    Student* student = new Student();
                    cout << "Enter student name: ";
                    cin.ignore();
                    getline(cin, student->name);
                    cout << "Enter student ID: ";
                    cin >> student->id;
                    book.setAvailability(false);
                    book.setBorrower(student);
                    borrowedBooks.push(make_pair(&book, student));
                    cout << "Book '" << book.getTitle() << "' issued to " << student->name << endl;
                } else {
                    cout << "The book '" << book.getTitle() << "' is currently borrowed." << endl;
                }
                return;
            }
        }
        cout << "Book with ID " << bookId << " not found." << endl;
    }

    // Function to return a borrowed book
    void returnBook() {
        int bookId;
        cout << "Enter the book ID to return: ";
        cin >> bookId;
        for (auto& book : inventory) {
            if (book.getBookId() == bookId) {
                if (!book.isAvailable()) {
                    book.setAvailability(true);
                    Student* borrower = book.getBorrower();
                    book.setBorrower(nullptr);
                    queue<pair<Book*, Student*> > tempQueue;
                    while (!borrowedBooks.empty()) {
                        if (borrowedBooks.front().first != &book) {
                            tempQueue.push(borrowedBooks.front());
                        }
                        borrowedBooks.pop();
                    }
                    borrowedBooks = tempQueue;
                    cout << "Book '" << book.getTitle() << "' returned successfully. Borrowed by " << borrower->name << " (ID: " << borrower->id << ")" << endl;
                    delete borrower;
                } else {
                    cout << "Error: The book '" << book.getTitle() << "' is not currently borrowed." << endl;
                }
                return;
            }
        }
        cout << "Error: Book with ID " << bookId << " not found." << endl;
    }

    // Function to list all books
    void listBooks() {
        if (inventory.empty()) {
            cout << "The library is empty." << endl;
            return;
        }
        int sortOption;
        cout << "Sort by:\n1. ID\n2. Title\nEnter your choice: ";
        cin >> sortOption;
        vector<Book> sortedBooks = inventory;
        if (sortOption == 1) {
            sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& a, const Book& b) { return a.getBookId() < b.getBookId(); });
        } else if (sortOption == 2) {
            sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& a, const Book& b) { return a.getTitle() < b.getTitle(); });
        } else {
            cout << "Invalid choice. Sorting by ID by default." << endl;
            sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& a, const Book& b) { return a.getBookId() < b.getBookId(); });
        }
        cout << "\nLibrary Book List:\n";
        cout << "ID\tTitle\tAuthor\tStatus" << endl;
        cout << "--------------------------------------" << endl;
        for (const auto& book : sortedBooks) {
            cout << book.getBookId() << "\t" << book.getTitle() << "\t" << book.getAuthor() << "\t" << (book.isAvailable() ? "Available" : "Borrowed") << endl;
        }
    }

    // Function to delete a book
    void removeBook() {
        int bookId;
        cout << "Enter the book ID to delete: ";
        cin >> bookId;
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if (it->getBookId() == bookId) {
                if (!it->isAvailable()) {
                    cout << "Error: Cannot delete book '" << it->getTitle() << "' because it is currently borrowed." << endl;
                    return;
                }
                string title = it->getTitle();
                inventory.erase(it);
                queue<pair<Book*, Student*> > tempQueue;
                while (!borrowedBooks.empty()) {
                    if (borrowedBooks.front().first->getBookId() != bookId) {
                        tempQueue.push(borrowedBooks.front());
                    }
                    borrowedBooks.pop();
                }
                borrowedBooks = tempQueue;
                cout << "Book '" << title << "' with ID " << bookId << " deleted from library." << endl;
                return;
            }
        }
        cout << "Error: Book with ID " << bookId << " not found." << endl;
    }
    
private:
    // Helper function to display book details
    void displayBookInfo(const Book& book) const {
        cout << "Book Details:" << endl;
        cout << "ID: " << book.getBookId() << endl;
        cout << "Title: " << book.getTitle() << endl;
        cout << "Author: " << book.getAuthor() << endl;
        cout << "Status: " << (book.isAvailable() ? "Available" : "Borrowed") << endl;
        if (!book.isAvailable() && book.getBorrower()) {
            cout << "Borrowed by: " << book.getBorrower()->name << " (ID: " << book.getBorrower()->id << ")" << endl;
        }
        cout << endl;
    }
};

int main() {
    Library library;
    int choice;
    while (true) {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add a Book" << endl;
        cout << "2. Search for a Book" << endl;
        cout << "3. Borrow a Book" << endl;
        cout << "4. Return a Book" << endl;
        cout << "5. List All Books" << endl;
        cout << "6. Remove a Book" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.findBook();
                break;
            case 3:
                library.borrowBook();
                break;
            case 4:
                library.returnBook();
                break;
            case 5:
                library.listBooks();
                break;
            case 6:
                library.removeBook();
                break;
            case 7:
                cout << "Thank you for using the Library Management System. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
