#pragma once
#include <string>
#include <tuple>

class View
{
private:
    void clearInputBuffer();
    std::string getDateString(const std::string& prompt);
    int getNumericInput(const std::string& prompt, bool allowZero = true);
    std::string getEmailString(const std::string& prompt);
    int getBoolInput(const std::string& prompt);

public:
    std::string getString(const std::string& prompt);

    View() = default;

    // Menus (Returns char)
    char showMainMenu();
    char showDataMenu();
    char showAddMenu();
    char showUpdateMenu();
    char showDeleteMenu();

    // Output
    void showMessage(const std::string& msg);
    void showInfoMessage(const std::string& msg);
    void showErrorMessage(const std::string& msg);
    void printResults(const std::string& results);
    void printSearchResult(const std::string& result, long long time_ms);
    void showGenerationResult(const std::string& msg, long long time_ms);

    // Input
    int getGenerationCount();
    int getRecordId(const std::string& record_type);
    int getCarId(const std::string& cars);
    int getOwnerId(const std::string& owners);

    // CRUD data gathering
    std::tuple<std::string, std::string, std::string, std::string> getOwnerData();
    std::tuple<std::string, std::string, std::string, std::string, std::string> getCarData();
    std::tuple<int, int, std::string, int> getRegistrationData(const std::string& cars, const std::string& owners);
    std::tuple<int, int, std::string, int> getTechnicalConditionData(const std::string& cars);
    std::tuple<int, std::string, int, std::string, std::string> getInsuranceData(const std::string& cars);

    // Functions for UPDATE
    std::tuple<std::string, std::string, std::string, std::string> getOwnerDataForUpdate();
    std::tuple<std::string, std::string, std::string, std::string, std::string> getCarDataForUpdate();

    // Search functions
    std::string getSearchPattern(const std::string& prompt);
    std::tuple<std::string, std::string> getDateRange();
    std::string getSingleDate(const std::string& prompt);
    std::tuple<int, int> getIdRange(const std::string& prompt);

};