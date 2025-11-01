# Computational-graphic-work-with-databases
# C++ CRUD Application with PostgreSQL and libpqxx

This is a basic CRUD (Create, Read, Update, Delete) application written in C++ that uses a PostgreSQL database for data storage and the `libpqxx` library to interact with the database.

The application is implemented using the MVC (Model-View-Controller) pattern.

## Prerequisites

Before you begin, ensure you have met the following requirements:
* A C++ compiler (e.g., g++ or MSVC)
* PostgreSQL installed and running
* The `libpqxx` library (including development headers and `libpq`). Installation will vary based on your OS.

## Getting Started

### Step 1: Clone the Repository

Clone this repository using the following command:

```bash
git clone [https://github.com/Diwiper/Databases-computational-graphic-work.git](https://github.com/Diwiper/Databases-computational-graphic-work.git)
cd Databases-computational-graphic-work
Step 2: Configure the Database Connection
Open the main.cpp file and provide your PostgreSQL database details in the connString:

// In main.cpp
const std::string connString = "dbname=postgres user=postgres password=YOUR_PASSWORD host=127.0.0.1 port=5432";
Step 3: Compile and Run the Application
Execute the following command to compile and run the application.

Example compilation (for g++):

g++ main.cpp model.cpp controller.cpp view.cpp -o rgr_db -lpqxx -lpq
(Note: You may need to specify the paths to libpqxx using -I/path/to/include and -L/path/to/lib if the library is not in your compiler's standard paths.)

Running the application:
./rgr_db
