#include "controller.h"
#include <stdexcept>

Controller::Controller(Model& m, View& v) : model(m), view(v) {}

// --- MAIN LOOP ---
void Controller::run()
{
    bool running = true;
    while (running)
    {
        char choice = view.showMainMenu();
        try
        {
            switch (choice)
            {
            case '1': handleShowDataMenu(); break;
            case '2': handleAddDataMenu(); break;
            case '3': handleUpdateDataMenu(); break;
            case '4': handleDeleteDataMenu(); break;
            case '5': handleGenerateData(); break;
            case '6': running = false; break;
            default: view.showErrorMessage("Unknown command. Please try again."); break;
            }
        }
        catch (const std::exception& e)
        {
            view.showErrorMessage(e.what());
        }
    }
}

// --- SUB-MENU HANDLERS ---

void Controller::handleAddDataMenu()
{
    bool running = true;
    while (running)
    {
        char choice = view.showAddMenu();
        switch (choice)
        {
        case '1': handleAddOwner(); break;
        case '2': handleAddCar(); break;
        case '3': handleAddRegistration(); break;
        case '4': handleAddTechCondition(); break;
        case '5': handleAddInsurance(); break;
        case '0': running = false; break;
        default: view.showErrorMessage("Unknown command."); break;
        }
    }
}

void Controller::handleShowDataMenu()
{
    bool running = true;
    while (running)
    {
        char choice = view.showDataMenu();
        switch (choice)
        {
        case '1': handleListOwners(); break;
        case '2': handleListCars(); break;
        case '3': handleListRegistrations(); break;
        case '4': handleListTechConditions(); break;
        case '5': handleListInsurances(); break;
        case '6': handleSearchQuery1(); break;
        case '7': handleSearchQuery2(); break;
        case '8': handleSearchQuery3(); break;
        case '9': handleSearchQuery4(); break;
        case '0': running = false; break;
        default: view.showErrorMessage("Unknown command."); break;
        }
    }
}

void Controller::handleDeleteDataMenu()
{
    bool running = true;
    while (running)
    {
        char choice = view.showDeleteMenu();
        switch (choice)
        {
        case '1': handleDeleteOwner(); break;
        case '2': handleDeleteCar(); break;
        case '3': handleDeleteRegistration(); break;
        case '4': handleDeleteTechCondition(); break;
        case '5': handleDeleteInsurance(); break;
        case '0': running = false; break;
        default: view.showErrorMessage("Unknown command."); break;
        }
    }
}

void Controller::handleUpdateDataMenu()
{
    bool running = true;
    while (running)
    {
        char choice = view.showUpdateMenu();
        switch (choice)
        {
        case '1': handleUpdateOwner(); break;
        case '2': handleUpdateCar(); break;
        case '3': handleUpdateRegistration(); break;
        case '4': handleUpdateTechCondition(); break;
        case '5': handleUpdateInsurance(); break;
        case '0': running = false; break;
        default: view.showErrorMessage("Unknown command."); break;
        }
    }
}

// --- ADD HANDLERS ---

void Controller::handleAddOwner()
{
    auto [first, last, email, phone] = view.getOwnerData();
    model.addOwner(first, last, email, phone);
    view.showMessage("Owner added successfully.");
}

void Controller::handleAddCar()
{
    auto [brand, car_model, vin, plate, year] = view.getCarData();
    model.addCar(brand, car_model, vin, plate, year);
    view.showMessage("Car added successfully.");
}

void Controller::handleAddRegistration()
{
    std::string cars = model.getAllCars();
    std::string owners = model.getAllOwners();
    auto [car_id, owner_id, date, auth] = view.getRegistrationData(cars, owners);
    model.addRegistration(car_id, owner_id, date, auth);
    view.showMessage("Registration added successfully.");
}

void Controller::handleAddTechCondition()
{
    std::string cars = model.getAllCars();
    auto [car_id, mileage, date, results] = view.getTechnicalConditionData(cars);
    model.addTechnicalCondition(car_id, mileage, date, results);
    view.showMessage("Technical Condition added successfully.");
}

void Controller::handleAddInsurance()
{
    std::string cars = model.getAllCars();
    auto [car_id, company, policy, start, end] = view.getInsuranceData(cars);
    model.addInsurance(car_id, company, policy, start, end);
    view.showMessage("Insurance added successfully.");
}

// --- LIST HANDLERS ---
void Controller::handleListOwners()
{
    std::string results = model.getAllOwners();
    view.printResults(results);
}

void Controller::handleListCars()
{
    std::string results = model.getAllCars();
    view.printResults(results);
}

void Controller::handleListRegistrations()
{
    std::string results = model.getAllRegistrations();
    view.printResults(results);
}

void Controller::handleListTechConditions()
{
    std::string results = model.getAllTechnicalConditions();
    view.printResults(results);
}

void Controller::handleListInsurances()
{
    std::string results = model.getAllInsurances();
    view.printResults(results);
}

// --- UPDATE HANDLERS ---
void Controller::handleUpdateOwner()
{
    view.showMessage("Loading owners list...");
    handleListOwners();
    int id = view.getRecordId("Owner");
    auto [first, last, email, phone] = view.getOwnerDataForUpdate();
    model.updateOwner(id, first, last, email, phone);
    view.showMessage("Owner updated successfully.");
}

void Controller::handleUpdateCar()
{
    view.showMessage("Loading cars list...");
    handleListCars();
    int id = view.getRecordId("Car");
    auto [brand, car_model, vin, plate, year] = view.getCarDataForUpdate();
    model.updateCar(id, brand, car_model, vin, plate, year);
    view.showMessage("Car updated successfully.");
}

void Controller::handleUpdateRegistration()
{
    view.showMessage("Loading registrations list...");
    handleListRegistrations();
    int id = view.getRecordId("Registration");
    std::string cars = model.getAllCars();
    std::string owners = model.getAllOwners();
    auto [car_id, owner_id, date, auth] = view.getRegistrationData(cars, owners);
    model.updateRegistration(id, car_id, owner_id, date, auth);
    view.showMessage("Registration updated successfully.");
}

void Controller::handleUpdateTechCondition()
{
    view.showMessage("Loading technical conditions list...");
    handleListTechConditions();
    int id = view.getRecordId("TechnicalCondition");
    std::string cars = model.getAllCars();
    auto [car_id, mileage, date, results] = view.getTechnicalConditionData(cars);
    model.updateTechnicalCondition(id, car_id, mileage, date, results);
    view.showMessage("Technical Condition updated successfully.");
}

void Controller::handleUpdateInsurance()
{
    view.showMessage("Loading insurances list...");
    handleListInsurances();
    int id = view.getRecordId("Insurance");
    std::string cars = model.getAllCars();
    auto [car_id, company, policy, start, end] = view.getInsuranceData(cars);
    model.updateInsurance(id, car_id, company, policy, start, end);
    view.showMessage("Insurance updated successfully.");
}

// --- DELETE HANDLERS ---
void Controller::handleDeleteOwner()
{
    handleListOwners();
    int id = view.getRecordId("Owner");
    model.deleteOwner(id);
    view.showMessage("Owner deleted successfully.");
}

void Controller::handleDeleteCar()
{
    handleListCars();
    int id = view.getRecordId("Car");
    model.deleteCar(id);
    view.showMessage("Car deleted successfully.");
}

void Controller::handleDeleteRegistration()
{
    handleListRegistrations();
    int id = view.getRecordId("Registration");
    model.deleteRegistration(id);
    view.showMessage("Registration deleted successfully.");
}

void Controller::handleDeleteTechCondition()
{
    handleListTechConditions();
    int id = view.getRecordId("TechnicalCondition");
    model.deleteTechnicalCondition(id);
    view.showMessage("Technical Condition deleted successfully.");
}

void Controller::handleDeleteInsurance()
{
    handleListInsurances();
    int id = view.getRecordId("Insurance");
    model.deleteInsurance(id);
    view.showMessage("Insurance deleted successfully.");
}

// --- GENERATION HANDLER ---
void Controller::handleGenerateData()
{
    int count = view.getGenerationCount();

    if (count == 0)
    {
        return;
    }

    view.showInfoMessage("Starting generation of " + std::to_string(count) + " records... This may take a moment.");

    try
    {
        auto [result, time] = model.generateRandomData(count);

        view.showGenerationResult(result, time);
    }
    catch (const std::exception& e)
    {
        view.showErrorMessage("Generation failed: " + std::string(e.what()));
    }
    catch (...)
    {
        view.showErrorMessage("Generation failed due to an unknown error.");
    }
}

// --- SEARCH HANDLERS ---
void Controller::handleSearchQuery1()
{
    std::string pattern = view.getSearchPattern("Enter owner's first or last name");
    auto [result, time] = model.searchCarsByOwnerName(pattern);
    view.printSearchResult(result, time);
}

void Controller::handleSearchQuery2()
{
    std::string date = view.getSingleDate("Enter inspection date (YYYY-MM-DD): ");
    auto [result, time] = model.searchTechByDate(date);
    view.printSearchResult(result, time);
}

void Controller::handleSearchQuery3()
{
    std::string plate = view.getSearchPattern("Enter car plate number (or part of it)");
    auto [result, time] = model.searchOwnerByCarPlate(plate);
    view.printSearchResult(result, time);
}

void Controller::handleSearchQuery4()
{
    auto [start_id, end_id] = view.getIdRange("Search Owners by ID Range");
    auto [result, time] = model.searchOwnersByIdRange(start_id, end_id);
    view.printSearchResult(result, time);
}