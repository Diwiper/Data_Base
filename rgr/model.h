#pragma once

#include <pqxx/pqxx>
#include <string>
#include <tuple> 

class Model
{
private:
    pqxx::connection C;
    std::string formatResult(const pqxx::result& res);

    int getNextOwnerId();
    int getNextCarId();
    int getNextRegistrationId();
    int getNextTechConditionId();
    int getNextInsuranceId();

public:
    Model(const std::string& connString);

    // --- ADD (CREATE) ---
    void addOwner(const std::string& first, const std::string& last, const std::string& email, const std::string& phone);
    void addCar(const std::string& brand, const std::string& model, const std::string& vin, const std::string& plate, const std::string& year);
    void addRegistration(int car_id, int owner_id, const std::string& date, int authority);
    void addTechnicalCondition(int car_id, int mileage, const std::string& inspection_date, int inspection_results);
    void addInsurance(int car_id, const std::string& company, int policy_number, const std::string& start_date, const std::string& end_date);

    // --- GET ALL (READ) ---
    std::string getAllOwners();
    std::string getAllCars();
    std::string getAllRegistrations();
    std::string getAllTechnicalConditions();
    std::string getAllInsurances();

    // --- UPDATE ---
    void updateOwner(int owner_id, const std::string& first, const std::string& last, const std::string& email, const std::string& phone);
    void updateCar(int car_id, const std::string& brand, const std::string& model, const std::string& vin, const std::string& plate, const std::string& year);
    void updateRegistration(int reg_id, int car_id, int owner_id, const std::string& date, int authority);
    void updateTechnicalCondition(int tech_id, int car_id, int mileage, const std::string& inspection_date, int inspection_results);
    void updateInsurance(int ins_id, int car_id, const std::string& company, int policy_number, const std::string& start_date, const std::string& end_date);

    // --- DELETE ---
    void deleteOwner(int owner_id);
    void deleteCar(int car_id);
    void deleteRegistration(int reg_id);
    void deleteTechnicalCondition(int tech_id);
    void deleteInsurance(int ins_id);

    // --- Requirement 2: Data Generation ---
    std::tuple<std::string, long long> generateRandomData(int count);

    // --- Requirement 3: Search and Timing ---
    std::tuple<std::string, long long> searchCarsByOwnerName(const std::string& name_pattern);
    std::tuple<std::string, long long> searchTechByDate(const std::string& date);
    std::tuple<std::string, long long> searchOwnerByCarPlate(const std::string& plate_number);
    std::tuple<std::string, long long> searchOwnersByIdRange(int start_id, int end_id);
};