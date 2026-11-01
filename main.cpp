#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cstdlib>

const std::string INVENTORY_FILE = "inventory.csv";
const std::string REPORT_FILE = "report.txt";
const int LOW_STOCK_THRESHOLD = 10;

class Product {
private:
    int id;
    std::string name;
    std::string category;
    double price;
    int quantity;

public:

    Product() : id(0), name(""), category(""), price(0.0), quantity(0) {}

    Product(int productId, const std::string &productName,
            const std::string &productCategory, double productPrice,
            int productQuantity)
        : id(productId), name(productName), category(productCategory),
          price(productPrice), quantity(productQuantity) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setName(const std::string &newName) { name = newName; }
    void setCategory(const std::string &newCategory) { category = newCategory; }
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    void increaseQuantity(int amount) { quantity += amount; }
    bool decreaseQuantity(int amount) {
        if (amount > quantity) {
            return false;
        }
        quantity -= amount;
        return true;
    }

    double getTotalValue() const { return price * quantity; }

    std::string toCSV() const {
        std::ostringstream oss;
        oss << id << "," << name << "," << category << ","
            << std::fixed << std::setprecision(2) << price << "," << quantity;
        return oss.str();
    }
};

class InventoryManager {
private:

    std::map<int, Product> inventory;

public:

    void loadFromFile(const std::string &filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {

            std::ofstream newFile(filename);
            newFile.close();
            std::cout << "[INFO] No existing inventory found. Created new file: "
                      << filename << "\n";
            return;
        }

        std::string line;
        int loadedCount = 0;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string idStr, name, category, priceStr, quantityStr;

            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, category, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, quantityStr, ',');

            if (idStr.empty() || priceStr.empty() || quantityStr.empty()) continue;

            try {
                int id = std::stoi(idStr);
                double price = std::stod(priceStr);
                int quantity = std::stoi(quantityStr);

                Product p(id, name, category, price, quantity);
                inventory[id] = p;
                loadedCount++;
            } catch (const std::exception &e) {

                continue;
            }
        }

        file.close();
        std::cout << "[INFO] Loaded " << loadedCount << " product(s) from "
                  << filename << "\n";
    }

    void saveToFile(const std::string &filename) const {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cout << "[ERROR] Unable to open file for saving: " << filename << "\n";
            return;
        }

        for (const auto &pair : inventory) {
            file << pair.second.toCSV() << "\n";
        }

        file.close();
        std::cout << "[INFO] Inventory saved successfully to " << filename << "\n";
    }

    bool addProduct(int id, const std::string &name, const std::string &category,
                     double price, int quantity) {

        if (inventory.find(id) != inventory.end()) {
            std::cout << "[ERROR] Product ID " << id << " already exists.\n";
            return false;
        }
        if (price < 0) {
            std::cout << "[ERROR] Price cannot be negative.\n";
            return false;
        }
        if (quantity < 0) {
            std::cout << "[ERROR] Quantity cannot be negative.\n";
            return false;
        }

        Product newProduct(id, name, category, price, quantity);
        inventory[id] = newProduct;
        std::cout << "[SUCCESS] Product added successfully.\n";
        return true;
    }

    void displayAllProducts() const {
        if (inventory.empty()) {
            std::cout << "[INFO] Inventory is empty.\n";
            return;
        }

        printTableHeader();
        for (const auto &pair : inventory) {
            printProductRow(pair.second);
        }
        printTableFooter();
    }

    Product* searchById(int id) {
        auto it = inventory.find(id);
        if (it != inventory.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    std::vector<Product> searchByName(const std::string &name) const {
        std::vector<Product> results;
        for (const auto &pair : inventory) {
            if (pair.second.getName().find(name) != std::string::npos) {
                results.push_back(pair.second);
            }
        }
        return results;
    }

    std::vector<Product> searchByCategory(const std::string &category) const {
        std::vector<Product> results;
        for (const auto &pair : inventory) {
            if (pair.second.getCategory().find(category) != std::string::npos) {
                results.push_back(pair.second);
            }
        }
        return results;
    }

    bool updateProduct(int id, const std::string &name, const std::string &category,
                        double price, int quantity) {
        auto it = inventory.find(id);
        if (it == inventory.end()) {
            std::cout << "[ERROR] Product with ID " << id << " not found.\n";
            return false;
        }
        if (price < 0) {
            std::cout << "[ERROR] Price cannot be negative.\n";
            return false;
        }
        if (quantity < 0) {
            std::cout << "[ERROR] Quantity cannot be negative.\n";
            return false;
        }

        it->second.setName(name);
        it->second.setCategory(category);
        it->second.setPrice(price);
        it->second.setQuantity(quantity);

        std::cout << "[SUCCESS] Product updated successfully.\n";
        return true;
    }

    bool deleteProduct(int id) {
        auto it = inventory.find(id);
        if (it == inventory.end()) {
            std::cout << "[ERROR] Product with ID " << id << " not found.\n";
            return false;
        }
        inventory.erase(it);
        std::cout << "[SUCCESS] Product deleted successfully.\n";
        return true;
    }

    bool sellProduct(int id, int amount) {
        auto it = inventory.find(id);
        if (it == inventory.end()) {
            std::cout << "[ERROR] Product with ID " << id << " not found.\n";
            return false;
        }
        if (amount <= 0) {
            std::cout << "[ERROR] Sell quantity must be positive.\n";
            return false;
        }
        if (!it->second.decreaseQuantity(amount)) {
            std::cout << "[ERROR] Insufficient stock. Available: "
                      << it->second.getQuantity() << "\n";
            return false;
        }
        std::cout << "[SUCCESS] Sold " << amount << " unit(s) of "
                  << it->second.getName() << ".\n";
        return true;
    }

    bool restockProduct(int id, int amount) {
        auto it = inventory.find(id);
        if (it == inventory.end()) {
            std::cout << "[ERROR] Product with ID " << id << " not found.\n";
            return false;
        }
        if (amount <= 0) {
            std::cout << "[ERROR] Restock quantity must be positive.\n";
            return false;
        }
        it->second.increaseQuantity(amount);
        std::cout << "[SUCCESS] Restocked " << amount << " unit(s) of "
                  << it->second.getName() << ".\n";
        return true;
    }

    std::vector<Product> getProductVector() const {
        std::vector<Product> products;
        products.reserve(inventory.size());
        for (const auto &pair : inventory) {
            products.push_back(pair.second);
        }
        return products;
    }

    std::vector<Product> sortByName() const {
        std::vector<Product> products = getProductVector();

        std::sort(products.begin(), products.end(),
                  [](const Product &a, const Product &b) {
                      return a.getName() < b.getName();
                  });
        return products;
    }

    std::vector<Product> sortByPrice() const {
        std::vector<Product> products = getProductVector();

        std::sort(products.begin(), products.end(),
                  [](const Product &a, const Product &b) {
                      return a.getPrice() < b.getPrice();
                  });
        return products;
    }

    std::vector<Product> sortByQuantity() const {
        std::vector<Product> products = getProductVector();

        std::sort(products.begin(), products.end(),
                  [](const Product &a, const Product &b) {
                      return a.getQuantity() < b.getQuantity();
                  });
        return products;
    }

    Product* binarySearchById(int id) {

        std::vector<Product> products = getProductVector();

        int left = 0;
        int right = static_cast<int>(products.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (products[mid].getId() == id) {

                auto it = inventory.find(products[mid].getId());
                if (it != inventory.end()) {
                    return &(it->second);
                }
                return nullptr;
            } else if (products[mid].getId() < id) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return nullptr;
    }

    std::vector<Product> getLowStockProducts() const {
        std::vector<Product> lowStock;
        for (const auto &pair : inventory) {
            if (pair.second.getQuantity() < LOW_STOCK_THRESHOLD) {
                lowStock.push_back(pair.second);
            }
        }
        return lowStock;
    }

    void generateReport(bool exportToFile = false) const {
        if (inventory.empty()) {
            std::cout << "[INFO] Inventory is empty. No report to generate.\n";
            return;
        }

        int totalProducts = static_cast<int>(inventory.size());
        int totalStockUnits = 0;
        double totalInventoryValue = 0.0;

        Product mostExpensive = inventory.begin()->second;
        Product cheapest = inventory.begin()->second;
        Product highestQuantity = inventory.begin()->second;
        Product lowestQuantity = inventory.begin()->second;

        for (const auto &pair : inventory) {
            const Product &p = pair.second;
            totalStockUnits += p.getQuantity();
            totalInventoryValue += p.getTotalValue();

            if (p.getPrice() > mostExpensive.getPrice()) mostExpensive = p;
            if (p.getPrice() < cheapest.getPrice()) cheapest = p;
            if (p.getQuantity() > highestQuantity.getQuantity()) highestQuantity = p;
            if (p.getQuantity() < lowestQuantity.getQuantity()) lowestQuantity = p;
        }

        std::ostringstream report;
        report << std::fixed << std::setprecision(2);
        report << "\n\n";
        report << "                    INVENTORY REPORT\n";
        report << "\n";
        report << std::left << std::setw(28) << "Total Products:" << totalProducts << "\n";
        report << std::left << std::setw(28) << "Total Stock Units:" << totalStockUnits << "\n";
        report << std::left << std::setw(28) << "Total Inventory Value:" << "$" << totalInventoryValue << "\n";
        report << "------------------------------------------------------------\n";
        report << "Most Expensive Product : " << mostExpensive.getName()
               << " (ID: " << mostExpensive.getId() << ", $" << mostExpensive.getPrice() << ")\n";
        report << "Cheapest Product        : " << cheapest.getName()
               << " (ID: " << cheapest.getId() << ", $" << cheapest.getPrice() << ")\n";
        report << "Highest Quantity Product: " << highestQuantity.getName()
               << " (ID: " << highestQuantity.getId() << ", Qty: " << highestQuantity.getQuantity() << ")\n";
        report << "Lowest Quantity Product : " << lowestQuantity.getName()
               << " (ID: " << lowestQuantity.getId() << ", Qty: " << lowestQuantity.getQuantity() << ")\n";
        report << "\n";

        std::cout << report.str();

        if (exportToFile) {
            std::ofstream outFile(REPORT_FILE);
            if (outFile.is_open()) {
                outFile << report.str();
                outFile.close();
                std::cout << "[INFO] Report exported to " << REPORT_FILE << "\n";
            } else {
                std::cout << "[ERROR] Unable to export report.\n";
            }
        }
    }

    static void printTableHeader() {
        std::cout << "\n" << std::string(90, '-') << "\n";
        std::cout << std::left
                   << std::setw(6)  << "ID"
                   << std::setw(22) << "Name"
                   << std::setw(18) << "Category"
                   << std::setw(12) << "Price"
                   << std::setw(10) << "Quantity"
                   << std::setw(14) << "Total Value" << "\n";
        std::cout << std::string(90, '-') << "\n";
    }

    static void printProductRow(const Product &p) {
        std::cout << std::left
                   << std::setw(6)  << p.getId()
                   << std::setw(22) << p.getName()
                   << std::setw(18) << p.getCategory()
                   << std::fixed << std::setprecision(2)
                   << std::setw(12) << p.getPrice()
                   << std::setw(10) << p.getQuantity()
                   << std::setw(14) << p.getTotalValue() << "\n";
    }

    static void printTableFooter() {
        std::cout << std::string(90, '-') << "\n";
    }

    static void printProductVector(const std::vector<Product> &products) {
        if (products.empty()) {
            std::cout << "[INFO] No products to display.\n";
            return;
        }
        printTableHeader();
        for (const auto &p : products) {
            printProductRow(p);
        }
        printTableFooter();
    }

    size_t size() const { return inventory.size(); }
};

int readInt(const std::string &prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input. Please enter a valid integer.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

double readDouble(const std::string &prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input. Please enter a valid number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string readLine(const std::string &prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

void displayMenu() {
    std::cout << "\n\n";
    std::cout << "            INVENTORY MANAGEMENT SYSTEM\n";
    std::cout << "\n";
    std::cout << " 1.  Add Product\n";
    std::cout << " 2.  Display Products\n";
    std::cout << " 3.  Search Product\n";
    std::cout << " 4.  Update Product\n";
    std::cout << " 5.  Delete Product\n";
    std::cout << " 6.  Sell Product\n";
    std::cout << " 7.  Restock Product\n";
    std::cout << " 8.  Sort by Product Name\n";
    std::cout << " 9.  Sort by Price\n";
    std::cout << "10.  Sort by Quantity\n";
    std::cout << "11.  Binary Search Product by ID\n";
    std::cout << "12.  Low Stock Alert\n";
    std::cout << "13.  Inventory Report\n";
    std::cout << "14.  Save Inventory\n";
    std::cout << "15.  Exit\n";
    std::cout << "\n";
}

void handleSearchMenu(InventoryManager &manager) {
    std::cout << "\n--- Search Product ---\n";
    std::cout << "1. Search by ID\n";
    std::cout << "2. Search by Name\n";
    std::cout << "3. Search by Category\n";
    int choice = readInt("Enter choice: ");

    switch (choice) {
        case 1: {
            int id = readInt("Enter Product ID: ");
            Product *p = manager.searchById(id);
            if (p != nullptr) {
                InventoryManager::printTableHeader();
                InventoryManager::printProductRow(*p);
                InventoryManager::printTableFooter();
            } else {
                std::cout << "[ERROR] Product not found.\n";
            }
            break;
        }
        case 2: {
            std::string name = readLine("Enter Product Name (or part of it): ");
            std::vector<Product> results = manager.searchByName(name);
            InventoryManager::printProductVector(results);
            break;
        }
        case 3: {
            std::string category = readLine("Enter Category (or part of it): ");
            std::vector<Product> results = manager.searchByCategory(category);
            InventoryManager::printProductVector(results);
            break;
        }
        default:
            std::cout << "[ERROR] Invalid choice.\n";
    }
}

int main() {
    InventoryManager manager;

    manager.loadFromFile(INVENTORY_FILE);

    bool running = true;

    while (running) {
        displayMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: {
                std::cout << "\n--- Add Product ---\n";
                int id = readInt("Enter Product ID: ");
                std::string name = readLine("Enter Product Name: ");
                std::string category = readLine("Enter Category: ");
                double price = readDouble("Enter Price: ");
                int quantity = readInt("Enter Quantity: ");
                manager.addProduct(id, name, category, price, quantity);
                break;
            }
            case 2: {
                std::cout << "\n--- All Products ---\n";
                manager.displayAllProducts();
                break;
            }
            case 3: {
                handleSearchMenu(manager);
                break;
            }
            case 4: {
                std::cout << "\n--- Update Product ---\n";
                int id = readInt("Enter Product ID to update: ");
                if (manager.searchById(id) == nullptr) {
                    std::cout << "[ERROR] Product with ID " << id << " not found.\n";
                    break;
                }
                std::string name = readLine("Enter New Name: ");
                std::string category = readLine("Enter New Category: ");
                double price = readDouble("Enter New Price: ");
                int quantity = readInt("Enter New Quantity: ");
                manager.updateProduct(id, name, category, price, quantity);
                break;
            }
            case 5: {
                std::cout << "\n--- Delete Product ---\n";
                int id = readInt("Enter Product ID to delete: ");
                manager.deleteProduct(id);
                break;
            }
            case 6: {
                std::cout << "\n--- Sell Product ---\n";
                int id = readInt("Enter Product ID: ");
                int amount = readInt("Enter Quantity to Sell: ");
                manager.sellProduct(id, amount);
                break;
            }
            case 7: {
                std::cout << "\n--- Restock Product ---\n";
                int id = readInt("Enter Product ID: ");
                int amount = readInt("Enter Quantity to Restock: ");
                manager.restockProduct(id, amount);
                break;
            }
            case 8: {
                std::cout << "\n--- Products Sorted by Name ---\n";
                std::vector<Product> sorted = manager.sortByName();
                InventoryManager::printProductVector(sorted);
                break;
            }
            case 9: {
                std::cout << "\n--- Products Sorted by Price ---\n";
                std::vector<Product> sorted = manager.sortByPrice();
                InventoryManager::printProductVector(sorted);
                break;
            }
            case 10: {
                std::cout << "\n--- Products Sorted by Quantity ---\n";
                std::vector<Product> sorted = manager.sortByQuantity();
                InventoryManager::printProductVector(sorted);
                break;
            }
            case 11: {
                std::cout << "\n--- Binary Search by ID ---\n";
                int id = readInt("Enter Product ID: ");
                Product *p = manager.binarySearchById(id);
                if (p != nullptr) {
                    InventoryManager::printTableHeader();
                    InventoryManager::printProductRow(*p);
                    InventoryManager::printTableFooter();
                } else {
                    std::cout << "[ERROR] Product not found.\n";
                }
                break;
            }
            case 12: {
                std::cout << "\n--- Low Stock Alert (Quantity < " << LOW_STOCK_THRESHOLD << ") ---\n";
                std::vector<Product> lowStock = manager.getLowStockProducts();
                InventoryManager::printProductVector(lowStock);
                break;
            }
            case 13: {
                std::cout << "\n--- Inventory Report ---\n";
                std::string exportChoice = readLine("Export report to report.txt? (y/n): ");
                bool exportToFile = (!exportChoice.empty() &&
                                      (exportChoice[0] == 'y' || exportChoice[0] == 'Y'));
                manager.generateReport(exportToFile);
                break;
            }
            case 14: {
                manager.saveToFile(INVENTORY_FILE);
                break;
            }
            case 15: {
                std::cout << "\n[INFO] Saving inventory before exit...\n";
                manager.saveToFile(INVENTORY_FILE);
                std::cout << "Thank you for using the Inventory Management System.\n";
                running = false;
                break;
            }
            default:
                std::cout << "[ERROR] Invalid choice. Please select a number between 1 and 15.\n";
        }
    }

    return 0;
}
