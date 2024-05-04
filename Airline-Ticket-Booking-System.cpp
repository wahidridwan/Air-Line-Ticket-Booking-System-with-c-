#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// User class
class User {
public:
    std::string username;
    std::string password;

    User(const std::string& uname, const std::string& pass) : username(uname), password(pass) {}
};

// Booking class
class Booking {
public:
    std::string username;
    std::string bookingDetails;
    std::string status;

    Booking(const std::string& uname, const std::string& details) : username(uname), bookingDetails(details), status("Pending") {}
};

// FileManager class for handling file operations
class FileManager {
public:
    static void writeUsersToFile(const std::vector<User>& users) {
        std::ofstream file("users.csv");
        for (const auto& user : users) {
            file << user.username << "," << user.password << std::endl;
        }
        file.close();
    }

    static std::vector<User> readUsersFromFile() {
        std::vector<User> users;
        std::ifstream file("users.csv");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                size_t pos = line.find(",");
                std::string username = line.substr(0, pos);
                std::string password = line.substr(pos + 1);
                users.emplace_back(username, password);
            }
            file.close();
        }
        return users;
    }

    static void writeBookingsToFile(const std::vector<Booking>& bookings) {
        std::ofstream file("bookings.csv");
        for (const auto& booking : bookings) {
            file << booking.username << "," << booking.bookingDetails << "," << booking.status << std::endl;
        }
        file.close();
    }

    static std::vector<Booking> readBookingsFromFile() {
        std::vector<Booking> bookings;
        std::ifstream file("bookings.csv");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);
                std::string username = line.substr(0, pos1);
                std::string details = line.substr(pos1 + 1, pos2 - pos1 - 1);
                std::string status = line.substr(pos2 + 1);
                bookings.emplace_back(username, details);
                bookings.back().status = status;
            }
            file.close();
        }
        return bookings;
    }
};

// Application class
class Application {
private:
    std::vector<User> users;
    std::vector<Booking> bookings;

public:
    Application() {
        users = FileManager::readUsersFromFile();
        bookings = FileManager::readBookingsFromFile();
    }

    void registerUser(const std::string& username, const std::string& password) {
        for (const auto& user : users) {
            if (user.username == username) {
                std::cout << "Username already exists." << std::endl;
                return;
            }
        }
        User newUser(username, password);
        users.push_back(newUser);
        FileManager::writeUsersToFile(users);
        std::cout << "User registered successfully." << std::endl;
    }

    bool loginUser(const std::string& username, const std::string& password) {
        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                std::cout << "Login successful." << std::endl;
                return true;
            }
        }
        std::cout << "Invalid username or password." << std::endl;
        return false;
    }

    void createBooking(const std::string& username, const std::string& bookingDetails) {
        Booking newBooking(username, bookingDetails);
        bookings.push_back(newBooking);
        FileManager::writeBookingsToFile(bookings);
        std::cout << "Booking created successfully." << std::endl;
    }

    void viewBooking(const std::string& username) {
        std::cout << "Your bookings:" << std::endl;
        for (const auto& booking : bookings) {
            if (booking.username == username) {
                std::cout << "Details: " << booking.bookingDetails << ", Status: " << booking.status << std::endl;
            }
        }
    }

    void listAllBookings() {
        std::cout << "All bookings:" << std::endl;
        for (const auto& booking : bookings) {
            std::cout << "Username: " << booking.username << ", Details: " << booking.bookingDetails << ", Status: " << booking.status << std::endl;
        }
    }
};
int main() {
    Application app;
    std::string choice;
    std::string username, password, bookingDetails;
    bool isLoggedIn = false;

    while (true) {
        std::cout << "Please choose an option:\n";

        if (!isLoggedIn) {
            std::cout << "1. Register\n2. Login\n";
        } else {
            std::cout << "3. Create Booking\n4. View Booking\n5. List All Bookings\n6. Logout\n";
        }

        std::cout << "7. Exit\n";
        std::cin >> choice;

        if (choice == "1" && !isLoggedIn) {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            app.registerUser(username, password);
        } else if (choice == "2" && !isLoggedIn) {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            isLoggedIn = app.loginUser(username, password);
        } else if (choice == "3" && isLoggedIn) {
            std::cout << "Enter booking details: ";
            std::cin.ignore();
            std::getline(std::cin, bookingDetails);
            app.createBooking(username, bookingDetails);
        } else if (choice == "4" && isLoggedIn) {
            app.viewBooking(username);
        } else if (choice == "5" && isLoggedIn) {
            app.listAllBookings();
        } else if (choice == "6" && isLoggedIn) {
            std::cout << "Logged out successfully.\n";
            isLoggedIn = false;
            username = "";
        } else if (choice == "7") {
            break;
        } else {
            std::cout << "Invalid choice or unauthorized action. Please try again." << std::endl;
        }
    }

    return 0;
}
