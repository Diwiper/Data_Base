#include "model.h"

using pqxx::operator""_zv;

#include <iostream>
#include <chrono>    
#include <iomanip>   
#include <sstream>   
#include <numeric>   
#include <cstring>   
#include <cmath> // Äëÿ std::ceil

Model::Model(const std::string& connString) : C(connString)
{
    if (!C.is_open())
    {
        throw pqxx::broken_connection("Failed to open DB connection");
    }
    std::cout << "Model: Connection to DB '" << C.dbname() << "' established successfully." << std::endl;
}

// --- GET NEXT ID ---
int Model::getNextOwnerId()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT MAX(owner_id) FROM Owner");
    if (R.empty() || R[0][0].is_null())
    {
        return 1;
    }
    return R[0][0].as<int>() + 1;
}

int Model::getNextCarId()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT MAX(car_id) FROM Car");
    if (R.empty() || R[0][0].is_null())
    {
        return 1;
    }
    return R[0][0].as<int>() + 1;
}

int Model::getNextRegistrationId()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT MAX(registration_id) FROM Registration");
    if (R.empty() || R[0][0].is_null())
    {
        return 1;
    }
    return R[0][0].as<int>() + 1;
}

int Model::getNextTechConditionId()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT MAX(\"TechnicalCondition_id\") FROM \"TechnicalCondition\"");
    if (R.empty() || R[0][0].is_null())
    {
        return 1;
    }
    return R[0][0].as<int>() + 1;
}

int Model::getNextInsuranceId()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT MAX(insurance_id) FROM insurance");
    if (R.empty() || R[0][0].is_null())
    {
        return 1;
    }
    return R[0][0].as<int>() + 1;
}

// --- ADD (CREATE) ---
void Model::addOwner(const std::string& first, const std::string& last, const std::string& email, const std::string& phone)
{
    int new_id = getNextOwnerId();
    pqxx::work W(C);
    W.exec(
        "INSERT INTO Owner (owner_id, first_name, last_name, email, phone_number) VALUES ($1, $2, $3, $4, $5)"_zv,
        pqxx::params{ new_id, first, last, email, phone }
    );
    W.commit();
}

void Model::addCar(const std::string& brand, const std::string& model, const std::string& vin, const std::string& plate, const std::string& year)
{
    int new_id = getNextCarId();
    pqxx::work W(C);
    W.exec(
        "INSERT INTO Car (car_id, brand, model, \"VIN_number\", plate_number, year_of_manufacture) VALUES ($1, $2, $3, $4, $5, $6::timestamp)"_zv,
        pqxx::params{ new_id, brand, model, vin, plate, year }
    );
    W.commit();
}

void Model::addRegistration(int car_id, int owner_id, const std::string& date, int authority)
{
    int new_id = getNextRegistrationId();
    try
    {
        pqxx::work W(C);
        W.exec(
            "INSERT INTO Registration (registration_id, car_id, owner_id, registration_date, registration_authority) VALUES ($1, $2, $3, $4::timestamp, $5)"_zv,
            pqxx::params{ new_id, car_id, owner_id, date, authority }
        );
        W.commit();
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id or owner_id does not exist.");
    }
}

void Model::addTechnicalCondition(int car_id, int mileage, const std::string& inspection_date, int inspection_results)
{
    int new_id = getNextTechConditionId();
    try
    {
        pqxx::work W(C);
        W.exec(
            "INSERT INTO \"TechnicalCondition\" (\"TechnicalCondition_id\", car_id, mileage, inspection_date, inspection_results) VALUES ($1, $2, $3, $4::timestamp, $5)"_zv,
            pqxx::params{ new_id, car_id, mileage, inspection_date, inspection_results }
        );
        W.commit();
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id does not exist.");
    }
}

void Model::addInsurance(int car_id, const std::string& company, int policy_number, const std::string& start_date, const std::string& end_date)
{
    int new_id = getNextInsuranceId();
    try
    {
        pqxx::work W(C);
        W.exec(
            "INSERT INTO insurance (insurance_id, car_id, insurance_company, policy_number, start_date, end_date) VALUES ($1, $2, $3, $4, $5::timestamp, $6::timestamp)"_zv,
            pqxx::params{ new_id, car_id, company, policy_number, start_date, end_date }
        );
        W.commit();
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id does not exist.");
    }
}

// --- GET ALL (READ) ---
std::string Model::getAllOwners()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT * FROM Owner ORDER BY owner_id LIMIT 100"_zv);
    return formatResult(R);
}

std::string Model::getAllCars()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT car_id, brand, model, \"VIN_number\", plate_number, year_of_manufacture::date FROM Car ORDER BY car_id LIMIT 100"_zv);
    return formatResult(R);
}

std::string Model::getAllRegistrations()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT registration_id, car_id, owner_id, registration_date::date, registration_authority FROM Registration ORDER BY registration_id LIMIT 100"_zv);
    return formatResult(R);
}

std::string Model::getAllTechnicalConditions()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT \"TechnicalCondition_id\", car_id, mileage, inspection_date::date, inspection_results FROM \"TechnicalCondition\" ORDER BY \"TechnicalCondition_id\" LIMIT 100"_zv);
    return formatResult(R);
}

std::string Model::getAllInsurances()
{
    pqxx::nontransaction N(C);
    pqxx::result R = N.exec("SELECT insurance_id, car_id, insurance_company, policy_number, start_date::date, end_date::date FROM insurance ORDER BY insurance_id LIMIT 100"_zv);
    return formatResult(R);
}

// --- UPDATE ---
void Model::updateOwner(int owner_id, const std::string& first, const std::string& last, const std::string& email, const std::string& phone)
{
    pqxx::work W(C);
    pqxx::result R = W.exec(
        "UPDATE Owner SET first_name = $1, last_name = $2, email = $3, phone_number = $4 WHERE owner_id = $5"_zv,
        pqxx::params{ first, last, email, phone, owner_id }
    );
    W.commit();
    if (R.affected_rows() == 0)
    {
        throw std::runtime_error("No Owner found with that ID. Update failed.");
    }
}

void Model::updateCar(int car_id, const std::string& brand, const std::string& model, const std::string& vin, const std::string& plate, const std::string& year)
{
    pqxx::work W(C);
    pqxx::result R = W.exec(
        "UPDATE Car SET brand = $1, model = $2, \"VIN_number\" = $3, plate_number = $4, year_of_manufacture = $5::timestamp WHERE car_id = $6"_zv,
        pqxx::params{ brand, model, vin, plate, year, car_id }
    );
    W.commit();
    if (R.affected_rows() == 0)
    {
        throw std::runtime_error("No Car found with that ID. Update failed.");
    }
}

void Model::updateRegistration(int reg_id, int car_id, int owner_id, const std::string& date, int authority)
{
    try
    {
        pqxx::work W(C);
        pqxx::result R = W.exec(
            "UPDATE Registration SET car_id = $1, owner_id = $2, registration_date = $3::timestamp, registration_authority = $4 WHERE registration_id = $5"_zv,
            pqxx::params{ car_id, owner_id, date, authority, reg_id }
        );
        W.commit();
        if (R.affected_rows() == 0)
        {
            throw std::runtime_error("No Registration found with that ID. Update failed.");
        }
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id or owner_id does not exist.");
    }
}

void Model::updateTechnicalCondition(int tech_id, int car_id, int mileage, const std::string& inspection_date, int inspection_results)
{
    try
    {
        pqxx::work W(C);
        pqxx::result R = W.exec(
            "UPDATE \"TechnicalCondition\" SET car_id = $1, mileage = $2, inspection_date = $3::timestamp, inspection_results = $4 WHERE \"TechnicalCondition_id\" = $5"_zv,
            pqxx::params{ car_id, mileage, inspection_date, inspection_results, tech_id }
        );
        W.commit();
        if (R.affected_rows() == 0)
        {
            throw std::runtime_error("No TechnicalCondition found with that ID. Update failed.");
        }
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id does not exist.");
    }
}

void Model::updateInsurance(int ins_id, int car_id, const std::string& company, int policy_number, const std::string& start_date, const std::string& end_date)
{
    try
    {
        pqxx::work W(C);
        pqxx::result R = W.exec(
            "UPDATE insurance SET car_id = $1, insurance_company = $2, policy_number = $3, start_date = $4::timestamp, end_date = $5::timestamp WHERE insurance_id = $6"_zv,
            pqxx::params{ car_id, company, policy_number, start_date, end_date, ins_id }
        );
        W.commit();
        if (R.affected_rows() == 0)
        {
            throw std::runtime_error("No Insurance found with that ID. Update failed.");
        }
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Specified car_id does not exist.");
    }
}

// --- DELETE ---
void Model::deleteOwner(int owner_id)
{
    try
    {
        pqxx::work W(C);
        W.exec("DELETE FROM Owner WHERE owner_id = $1"_zv, pqxx::params{ owner_id });
        W.commit();
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Cannot delete owner, as they have dependent registrations.");
    }
}

void Model::deleteCar(int car_id)
{
    try
    {
        pqxx::work W(C);
        W.exec("DELETE FROM Car WHERE car_id = $1"_zv, pqxx::params{ car_id });
        W.commit();
    }
    catch (const pqxx::foreign_key_violation& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
        throw std::runtime_error("Integrity error: Cannot delete car, as it has dependent records (Registrations, Insurances, TechInspections).");
    }
}

void Model::deleteRegistration(int reg_id)
{
    pqxx::work W(C);
    W.exec("DELETE FROM Registration WHERE registration_id = $1"_zv, pqxx::params{ reg_id });
    W.commit();
}

void Model::deleteTechnicalCondition(int tech_id)
{
    pqxx::work W(C);
    W.exec("DELETE FROM \"TechnicalCondition\" WHERE \"TechnicalCondition_id\" = $1"_zv, pqxx::params{ tech_id });
    W.commit();
}

void Model::deleteInsurance(int ins_id)
{
    pqxx::work W(C);
    W.exec("DELETE FROM insurance WHERE insurance_id = $1"_zv, pqxx::params{ ins_id });
    W.commit();
}

// --- DATA GENERATION ---
std::tuple<std::string, long long> Model::generateRandomData(int count)
{
    int start_owner_id = getNextOwnerId();
    int start_car_id = getNextCarId();
    int start_reg_id = getNextRegistrationId();
    int start_ins_id = getNextInsuranceId();
    int start_tech_id = getNextTechConditionId();

    const int chunk_size = 10000;
    int num_chunks = static_cast<int>(std::ceil(static_cast<double>(count) / chunk_size));

    auto start_time = std::chrono::high_resolution_clock::now();

    try
    {
       

        for (int i = 0; i < num_chunks; i++)
        {
            int current_chunk_size = std::min(chunk_size, count - (i * chunk_size));
            if (current_chunk_size <= 0) break;



            int chunk_start_owner = start_owner_id + (i * chunk_size);
            int chunk_start_car = start_car_id + (i * chunk_size);
            int chunk_start_reg = start_reg_id + (i * chunk_size);
            int chunk_start_ins = start_ins_id + (i * chunk_size);
            int chunk_start_tech = start_tech_id + (i * chunk_size);

            int max_gen_owner_id = chunk_start_owner + current_chunk_size - 1;
            int max_gen_car_id = chunk_start_car + current_chunk_size - 1;


            {
                pqxx::work W_Base(C);
              
                W_Base.exec(
                    "INSERT INTO Owner (owner_id, first_name, last_name, email, phone_number) "
                    "SELECT "
                    "    s.id + $1, "
                    "    'FirstName_' || (s.id + $1), "
                    "    'LastName_' || (s.id + $1), "
                    "    'user_' || (s.id + $1) || '@generated.com', "
                    "    '380' || (100000000 + random() * 900000000)::bigint::text "
                    "FROM generate_series(0, $2 - 1) AS s(id)"_zv,
                    pqxx::params{ chunk_start_owner, current_chunk_size }
                );

             
                W_Base.exec(
                    "INSERT INTO Car (car_id, brand, model, \"VIN_number\", plate_number, year_of_manufacture) "
                    "SELECT "
                    "    s.id + $1, "
                    "    'Brand_' || (s.id + $1), "
                    "    'Model_' || (s.id + $1), "
                    "    'VIN' || (100000000000 + random()*900000000000)::bigint::text, "
                    "    'AA' || (1000 + random()*8999)::int || 'AA', "
                    "    (NOW() - (random() * 365 * 10 || ' days')::interval) "
                    "FROM generate_series(0, $2 - 1) AS s(id)"_zv,
                    pqxx::params{ chunk_start_car, current_chunk_size }
                );

          
                W_Base.commit();
            }

        
            {
                pqxx::work W_Dependent(C);
               
                W_Dependent.exec(
                    "INSERT INTO Registration (registration_id, car_id, owner_id, registration_date, registration_authority) "
                    "SELECT "
                    "    s.id + $1, "
                    "    floor(random() * ($3::numeric - $4::numeric + 1) + $4)::int, " 
                    "    floor(random() * ($5::numeric - $6::numeric + 1) + $6)::int, " 
                    "    (NOW() - (random() * 365 * 2 || ' days')::interval), "
                    "    (1000 + random()*8999)::int "
                    "FROM generate_series(0, $2 - 1) AS s(id)"_zv,
                    pqxx::params{ chunk_start_reg, current_chunk_size, max_gen_car_id, chunk_start_car, max_gen_owner_id, chunk_start_owner }
                );

              
                W_Dependent.exec(
                    "INSERT INTO insurance (insurance_id, car_id, insurance_company, \"policy_number \", \"start_date \", \"end_date \") "
                    "SELECT "
                    "    s.id + $1, "
                    "    floor(random() * ($3::numeric - $4::numeric + 1) + $4)::int, " 
                    "    'Company_' || (random()*5)::int, "
                    "    (100000 + random()*899999)::int, "
                    "    (NOW() - (random() * 300 || ' days')::interval), "
                    "    (NOW() + (random() * 300 || ' days')::interval) "
                    "FROM generate_series(0, $2 - 1) AS s(id)"_zv,
                    pqxx::params{ chunk_start_ins, current_chunk_size, max_gen_car_id, chunk_start_car }
                );

           
                W_Dependent.exec(
                    "INSERT INTO \"TechnicalCondition\" (\"TechnicalCondition_id\", car_id, mileage, inspection_date, inspection_results) "
                    "SELECT "
                    "    s.id + $1, "
                    "    floor(random() * ($3::numeric - $4::numeric + 1) + $4)::int, " 
                    "    (random() * 200000)::int, "
                    "    (NOW() - (random() * 365 || ' days')::interval), "
                    "    (random() > 0.5)::int "
                    "FROM generate_series(0, $2 - 1) AS s(id)"_zv,
                    pqxx::params{ chunk_start_tech, current_chunk_size, max_gen_car_id, chunk_start_car }
                );

                W_Dependent.commit();
            }

        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[DEBUG] ERROR during data generation: " << e.what() << std::endl;
        std::cerr << "[DEBUG] Transaction will be rolled back." << std::endl;
        throw;
    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

    return { "Generated " + std::to_string(count) + " records in each of the 5 tables.", duration.count() };
}


// --- SEARCH ---
std::tuple<std::string, long long> Model::searchCarsByOwnerName(const std::string& name_pattern)
{
    pqxx::nontransaction N(C);
    auto start = std::chrono::high_resolution_clock::now();
    pqxx::result R = N.exec(
        "SELECT c.car_id, c.brand, c.model, c.plate_number, o.first_name, o.last_name "
        "FROM Car c "
        "JOIN Registration r ON c.car_id = r.car_id "
        "JOIN Owner o ON r.owner_id = o.owner_id "
        "WHERE o.first_name ILIKE $1 OR o.last_name ILIKE $1 "
        "LIMIT 100"_zv,
        pqxx::params{ '%' + name_pattern + '%' }
    );
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return { formatResult(R), duration.count() };
}

std::tuple<std::string, long long> Model::searchTechByDate(const std::string& date)
{
    pqxx::nontransaction N(C);
    auto start = std::chrono::high_resolution_clock::now();
    pqxx::result R = N.exec(
        "SELECT t.\"TechnicalCondition_id\", t.inspection_date::date, t.mileage, t.inspection_results, c.brand, c.model, c.plate_number "
        "FROM \"TechnicalCondition\" t "
        "JOIN Car c ON t.car_id = c.car_id "
        "WHERE t.inspection_date >= $1::timestamp AND t.inspection_date < ($1::timestamp + '1 day'::interval) "
        "ORDER BY t.inspection_date DESC "
        "LIMIT 100"_zv,
        pqxx::params{ date }
    );
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return { formatResult(R), duration.count() };
}

std::tuple<std::string, long long> Model::searchOwnerByCarPlate(const std::string& plate_number)
{
    pqxx::nontransaction N(C);
    auto start = std::chrono::high_resolution_clock::now();
    pqxx::result R = N.exec(
        "SELECT "
        "    o.owner_id, o.first_name, o.last_name, o.phone_number, "
        "    c.brand, c.model, c.plate_number "
        "FROM Owner o "
        "JOIN Registration r ON o.owner_id = r.owner_id "
        "JOIN Car c ON r.car_id = c.car_id "
        "WHERE c.plate_number ILIKE $1 "
        "GROUP BY o.owner_id, c.car_id "
        "LIMIT 100"_zv,
        pqxx::params{ '%' + plate_number + '%' }
    );
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return { formatResult(R), duration.count() };
}

std::tuple<std::string, long long> Model::searchOwnersByIdRange(int start_id, int end_id)
{
    pqxx::nontransaction N(C);
    auto start = std::chrono::high_resolution_clock::now();
    pqxx::result R = N.exec(
        "SELECT * FROM Owner "
        "WHERE owner_id BETWEEN $1 AND $2 "
        "ORDER BY owner_id ASC "
        "LIMIT 100"_zv,
        pqxx::params{ start_id, end_id }
    );
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return { formatResult(R), duration.count() };
}

// --- Helper function ---
std::string Model::formatResult(const pqxx::result& R)
{
    if (R.empty())
    {
        return "No records found.\n";
    }
    std::stringstream ss;
    std::vector<int> col_widths;
    for (pqxx::row::size_type i = 0; i < R.columns(); i++)
    {
        int width = static_cast<int>(std::strlen(R.column_name(i)));
        for (const auto& row : R)
        {
            if (row[i].size() > width) width = static_cast<int>(row[i].size());
        }
        col_widths.push_back(width + 2);
        ss << std::left << std::setw(col_widths.back()) << R.column_name(i);
    }
    ss << "\n" << std::string(std::accumulate(col_widths.begin(), col_widths.end(), 0), '-') << "\n";
    for (const auto& row : R)
    {
        for (pqxx::row::size_type i = 0; i < R.columns(); i++)
        {
            ss << std::left << std::setw(col_widths[i]) << row[i].c_str();
        }
        ss << "\n";
    }
    return ss.str();
}