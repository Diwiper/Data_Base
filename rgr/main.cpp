#include "controller.h"
#include "model.h"
#include "view.h"
#include <iostream>
#include <cstdlib> 

int main()
{
    system("chcp 65001");
    system("cls");

    const std::string connString = "dbname=postgres user=postgres password=12345678901Q host=127.0.0.1 port=5432";

    try
    {
        //Create components
        Model model(connString);
        View view;

        //Connect them via the Controller
        Controller controller(model, view);


        controller.run();
    }
    catch (const pqxx::broken_connection& e)
    {
        std::cerr << "CRITICAL ERROR: Failed to connect to the database." << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        std::cerr << "Check your connString in main.cpp" << std::endl;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An unexpected error was caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}