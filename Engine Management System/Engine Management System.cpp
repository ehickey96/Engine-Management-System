// Engine Management System.cpp 
// Elijah Hickey
// 2021.12.5

// Simple menu system to act as a concept showcase

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

//The minimum values needed to pass monitor checks
const int G_TURBINE_REQ_RPM = 12500;
const int G_JET_REQ_BAR = 30; //air pressure

class Engine {
protected:
    std::string id_;
    float cost_;
    bool passing_;
public:
    Engine(std::string id, float cost, bool passing) :
        id_(id),
        cost_(cost),
        passing_(passing)
    {
    }
    Engine() = default;

    std::string getId() const { return id_; }
    void setId(const std::string& id) { id_ = id; }

    float getCost() const { return cost_; }
    void setCost(float cost) { cost_ = cost; }

    bool getPassing() const { return passing_; }
    void setPassing(bool passing) { passing_ = passing; }

    virtual void monitor() {
        if (!passing_) {
            std::cout << "Monitoring for part " << id_ << " is disabled " << std::endl;
        }
    }
};

class Turbine : public Engine {

protected:
    int rpm_;
public:
    Turbine(std::string id, float cost, bool passing, int rpm) :
        Engine(id, cost, passing),
        rpm_(rpm)
    {}
    Turbine() = default;

    int getRpm() const { return rpm_; }
    void setRpm(const int& rpm) { rpm_ = rpm; }

    void monitor() {
        passing_ = rpm_ >= G_TURBINE_REQ_RPM ? (true) : (false);
        if (passing_) {
            std::cout << "Turbine Engine: " << id_ << " is a safe RPM of:"
                << rpm_ << std::endl;
        }
        else {
            std::cout << "Turbine Engine: " << id_ << " is " << G_TURBINE_REQ_RPM - rpm_
                << " RPM below accepted levels. " << std::endl;
        }
    }
};
class Jet : public Engine {

protected:
    int bar_; //measurement of air pressure
public:
    Jet(std::string id, float cost, bool passing, int bar) :
        Engine(id, cost, passing),
        bar_(bar)
    {}
    Jet() = default;

    int getBar() const { return bar_; }
    void setBar(const int& bar) { bar_ = bar; }

    void monitor() {
        passing_ = bar_ >= G_JET_REQ_BAR ? (true) : (false);
        if (passing_) {
            std::cout << "Jet Engine: " << id_ << " has a safe BAR measurement of:"
                << bar_ << std::endl;
        }
        else {
            std::cout << "Jet Engine: " << id_ << " is " << G_JET_REQ_BAR - bar_
                << " BAR below accepted levels. " << std::endl;
        }
    }
};

//Container Declarations
std::map<std::string, float> g_cost_map;
std::vector<Turbine> g_turbine_vect;
std::vector<Jet> g_jet_vect;




//creates a map with id as the key and price as the value
//Data comes from turbine_vect and jet_vect
template<typename T, typename J>
void create_price_id_map(std::vector<T> t_vect, std::vector<J> j_vect) {
    for (T t_vector : t_vect)
        g_cost_map.insert(make_pair(t_vector.getId(), t_vector.getCost()));

    for (J j_vector : j_vect)
        g_cost_map.insert(make_pair(j_vector.getId(), j_vector.getCost()));
}

bool reverse_compare(std::pair<std::string, float>& p1, std::pair<std::string, float>& p2) {
    return p1.second > p2.second;
}

//creates a vector pair from the map, sorts by cost (decending), then prints
void order_by_cost_printer(std::map<std::string, float> map) {
    std::vector<std::pair<std::string, float>> v_pair;

    for (auto it : map)
        v_pair.push_back(it);

    sort(v_pair.begin(), v_pair.end(), reverse_compare);
    std::cout << "Engine Unit costs" << std::endl;

    for (auto it : v_pair)
        std::cout << "ID: " << it.first << " Cost: $" << it.second << std::endl;
}

//Following two functions are pointed to by the function pointer
//found in the menu handler (menu::print() & menu::print(char)
void deleteEntry(std::string key) {
    auto iterator = g_cost_map.find(key);
    if (iterator != g_cost_map.end()) {
        std::cout << "Entry deleted successfully" << std::endl;
        g_cost_map.erase(iterator);
    }
}
void findEntry(std::string key) {
    auto iterator = g_cost_map.find(key);
    if (iterator != g_cost_map.end())
        std::cout << "Entry found! ID: " << (*iterator).first
        << " Price $:" << (*iterator).second << std::endl;

}

//prints status of each engine (passing, or failing by a given amount)
//Does not get updated by the functions that affect the map
template<typename T, typename J>
void print_monitor_data(std::vector<T> t_vect, std::vector<J> j_vect) {
    std::cout << "--engine " << typeid(T).name() << "--" << std::endl;
    for (T t_vector : t_vect) { t_vector.monitor(); }

    std::cout << "--engine " << typeid(J).name() << "--" << std::endl;
    for (J j_vector : j_vect) { j_vector.monitor(); }

}



//menu namespace functions include:
//init(), print(), print(char)
//Initializes mock data, and acts as menu printer
namespace menu {
    void init() {


        Turbine turbine1 = Turbine("t001", 80000, false, 17500);
        Turbine turbine2 = Turbine("t002", 45000, false, 12000);
        Turbine turbine3 = Turbine("t003", 75000, false, 22000);

        Turbine t[3] = { turbine1, turbine2, turbine3 };

        g_turbine_vect.insert(g_turbine_vect.end(), std::begin(t), std::end(t));

        Jet jet1 = Jet("j001", 65000, false, 40);
        Jet jet2 = Jet("j002", 105000, false, 28);
        Jet jet3 = Jet("j003", 50000, false, 37);

        Jet j[3] = { jet1, jet2, jet3 };

        g_jet_vect.insert(g_jet_vect.end(), std::begin(j), std::end(j));

        //The derived class vectors are stored seperately, 
        //but are used to create a map of id & price here
        create_price_id_map(g_turbine_vect, g_jet_vect);

    }
    //main menu output
    void print() {
        std::cout << "\nMenu\n" << std::endl;
        std::cout << "A. Monitoring Status of Engines" << std::endl;
        std::cout << "B. Print Engines by Cost" << std::endl;
        std::cout << "C. Delete a Map Entry" << std::endl;
        std::cout << "D. Find a Map Entry" << std::endl;
        std::cout << "E. Exit" << std::endl;
    }
    //menu handler
    void print(char input) {
        char choice = toupper(input);
        std::string foo;
        //currently only finds & deletes
        //pointer function to handle basic crud
        void (*crudHandler)(std::string);

        switch (choice) {

        case 'A':
            std::cout << "\nShow Engine Montior Status\n" << std::endl;
            print_monitor_data(g_turbine_vect, g_jet_vect);
            break;

        case 'B':
            std::cout << "\nShow Engines by Cost\n" << std::endl;
            order_by_cost_printer(g_cost_map);
            break;

        case 'C':
            foo = g_jet_vect[0].getId();
            std::cout << "\nAttempting to delete map entry ~ " << foo << std::endl;
            crudHandler = &deleteEntry;
            (crudHandler)(foo);
            break;
        case 'D':
            foo = g_turbine_vect[1].getId();
            std::cout << "\nAttempting to find map entry ~" << foo << std::endl;
            crudHandler = &findEntry;
            (crudHandler)(foo);
            break;

        case 'E':
            std::cout << "\nExiting.\n" << std::endl;
            return;
            break;

        default:
            std::cout << "\nInvalid entry, try again" << std::endl;
        }
    }
}


int main()
{
    std::cout << "Elijah Hickey - 2021.12.5\n";
    std::cout << "\nEngine Management System.\n" << std::endl;

    menu::init();
    char input = 'z';
    while (toupper(input) != 'E') {
        menu::print();
        std::cin >> input;
        menu::print(toupper(input));
    }
}

