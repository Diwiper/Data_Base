#pragma once
#include "model.h"
#include "view.h"

class Controller
{
private:
    Model& model;
    View& view;

    // --- Handlers for Sub-Menus ---
    void handleShowDataMenu();
    void handleAddDataMenu();
    void handleUpdateDataMenu();
    void handleDeleteDataMenu();
    void handleGenerateData();

    // --- Handlers for ADD ---
    void handleAddOwner();
    void handleAddCar();
    void handleAddRegistration();
    void handleAddTechCondition();
    void handleAddInsurance();

    // --- Handlers for LIST ---
    void handleListOwners();
    void handleListCars();
    void handleListRegistrations();
    void handleListTechConditions();
    void handleListInsurances();

    // --- Handlers for UPDATE ---
    void handleUpdateOwner();
    void handleUpdateCar();
    void handleUpdateRegistration();
    void handleUpdateTechCondition();
    void handleUpdateInsurance();

    // --- Handlers for DELETE ---
    void handleDeleteOwner();
    void handleDeleteCar();
    void handleDeleteRegistration();
    void handleDeleteTechCondition();
    void handleDeleteInsurance();

    // --- Handlers for SEARCH ---
    void handleSearchQuery1();
    void handleSearchQuery2();
    void handleSearchQuery3();
    void handleSearchQuery4();

public:
    Controller(Model& m, View& v);
    void run();
};