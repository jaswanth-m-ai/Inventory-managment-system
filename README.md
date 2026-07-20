# Inventory Management System

A professional, console-based **Inventory Management System** built in **C++17**, demonstrating Object-Oriented Programming, STL containers, sorting algorithms, binary search, and CSV-based file persistence.

## Project Overview

The Inventory Management System allows a business to manage its product inventory through a clean, menu-driven console interface. Products are stored in memory using an `std::map<int, Product>` keyed by Product ID for fast lookups, inserts, and deletes, while sorting operations are performed on an `std::vector<Product>` using `std::sort()`. All inventory data is automatically loaded from and saved to a CSV file, so data persists between runs.

## Features

- **Add Product** — Add a new product with ID, name, category, price, and quantity (duplicate IDs and negative values are rejected).
- **Display Products** — View the full inventory in a clean, formatted table.
- **Search Product** — Search by ID (map lookup), Name, or Category.
- **Update Product** — Modify an existing product's details.
- **Delete Product** — Remove a product from inventory.
- **Sell Product** — Decrease stock quantity (prevents negative stock).
- **Restock Product** — Increase stock quantity.
- **Sort by Name / Price / Quantity** — Uses `std::sort()` on a vector copy of the inventory.
- **Binary Search by ID** — Demonstrates a manual binary search implementation over a sorted vector.
- **Low Stock Alert** — Lists all products with quantity below the configured threshold (default: 10).
- **Inventory Report** — Displays analytics: total products, total stock units, total inventory value, most expensive/cheapest product, and highest/lowest quantity product. Can optionally be exported to `report.txt`.
- **Save Inventory** — Manually persist the current inventory to `inventory.csv`.
- **Automatic Load/Save** — Inventory is loaded on startup and saved automatically on exit.

## Technologies Used

- **Language:** C++17
- **Standard Library Components:**
  - `std::map` — primary inventory storage (ID → Product)
  - `std::vector` — used for sorting and search result collections
  - `std::sort` — sorting algorithm (O(n log n))
  - `std::ifstream` / `std::ofstream` — CSV file I/O
  - `std::stringstream` — CSV parsing
  - `<iomanip>` — professional table formatting (`setw`, `left`, `fixed`, `setprecision`)

## Folder Structure

```
Inventory-Management-System/
├── main.cpp        # Complete application source code
├── inventory.csv    # Sample inventory data (auto-created if missing)
├── README.md        # Project documentation
├── .gitignore        # Files/directories excluded from version control
└── LICENSE           # MIT License
```

## How to Run

### Prerequisites
- A C++17-compatible compiler (e.g., `g++` 7+, `clang++` 5+, or MSVC 2017+)

### Compile

```bash
g++ -std=c++17 -Wall -Wextra -o inventory main.cpp
```

### Run

```bash
./inventory
```

On Windows (after compiling with MinGW or MSVC):

```bash
inventory.exe
```

The application will automatically create `inventory.csv` if it does not already exist, and will load any existing data on startup.

## Complexity Notes

| Operation                | Data Structure | Time Complexity |
|---------------------------|-----------------|------------------|
| Add Product (Insert)      | `std::map`      | O(log n)         |
| Search by ID               | `std::map`      | O(log n)         |
| Delete Product              | `std::map`      | O(log n)         |
| Sort (Name/Price/Quantity) | `std::vector`   | O(n log n)       |
| Binary Search by ID         | `std::vector`   | O(log n)         |

## Screenshots Section

_Add terminal screenshots of the menu, product table, and inventory report here once available._

## Future Improvements

- Add a graphical user interface (GUI) using Qt or Dear ImGui.
- Support multiple warehouses/locations per product.
- Add user authentication and role-based access control.
- Integrate a proper database backend (SQLite/MySQL) instead of CSV.
- Add unit tests using a framework such as Catch2 or Google Test.
- Support undo/redo for inventory operations.

## Author

Inventory Management System — developed as a professional college final project.
