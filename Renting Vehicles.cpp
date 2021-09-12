#include <iostream>
#include <iomanip>

using namespace std;

class DayOfYear{
private:
    short unsigned int dayOfYear, year;
    static const short unsigned int days[];
public:
    DayOfYear(short unsigned int day = 1, short unsigned int year = 365) : dayOfYear(day), year(year)
    {}
    DayOfYear(short unsigned int day, short unsigned int month, short unsigned int year): dayOfYear(days[month-1] + day), year(year)
    {}
    short unsigned int get_dayOfYear(){
        return dayOfYear;
    }
    short unsigned int get_year(){
        return year;
    }
    DayOfYear operator++(){
        DayOfYear n;
        if(dayOfYear==365){
            n.dayOfYear=1;
            n.year = ++year;
        }else{
            n.dayOfYear = ++dayOfYear;
            n.year = year;
        }
        return n;
    }
    friend istream& operator>>(istream& in, DayOfYear& D){
        char c;
        short unsigned int month, day;
        in >> D.year >> c >> month >> c >> day;
        D.dayOfYear = days[month-1] + day;
        return in;
    }
    friend ostream& operator<<(ostream& out, const DayOfYear& D){
        short unsigned int month, day;
        for(unsigned int i=0; i<12; ++i){
            if(D.dayOfYear > days[i]){
                month = i+1;
                day = D.dayOfYear - days[i];
            }
        }
        out << D.year << "-" << setw(2) << setfill('0') << month << "-" << setw(2) << setfill('0') << day;
        return out;
    }
};

const short unsigned int DayOfYear::days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

class Vehicle{
private:
    const int no;
    string model;
    float price24h;
public:
    Vehicle(int no, string model, float price24h): no(no), model(model), price24h(price24h)
    {}
    virtual ~Vehicle(){
        delete this;
    }
    int get_no(){
        return no;
    }
    string get_model(){
        return model;
    }
    float get_price(int daysOfRental){
        return price24h * daysOfRental;
    }
    virtual void print() = 0;
};

class Bike : public Vehicle{
public:
    Bike(int no, string model): Vehicle(no, model, 9.99)
    {}
    virtual void print(){
        cout << " " << get_no() << ": " << get_model() << " (Bike)" <<endl;
    }
};

class EMotorVehicle : public Vehicle{
public:
    EMotorVehicle(int no, string model, float price24h): Vehicle(no, model, price24h)
    {}
    virtual bool is_streetLegal() = 0;
    virtual void print(){
        cout << " " << get_no() << ": " << get_model() <<endl;
        if(is_streetLegal() == false){
            cout << " (not street legal)";
        }
    }
};

class EBike : public EMotorVehicle{
public:
    EBike(int no, string model, float price24h = 29.99): EMotorVehicle(no, model, price24h)
    {}
    virtual bool is_streetLegal(){
        return true;
    }
    virtual void print(){
        EMotorVehicle::print();
        cout<< " (EBike)";
    }
};

class EScooter : public EMotorVehicle{
private:
    bool streetLegal;
public:
    EScooter(int no, string model, float price24h = 19.99, bool streetLegal = false): EMotorVehicle(no, model, price24h), streetLegal(streetLegal)
    {}
    virtual bool is_streetLegal(){
        return streetLegal;
    }
    virtual void print(){
        EMotorVehicle::print();
        cout<< " (EScooter)";
    }
};

class Rental{
private:
    const int no;
    DayOfYear from;
    int days;
    string customer;
    static int last_no;
public:
    Rental(string customer, DayOfYear from, int days = 1): customer(customer), from(from), days(days), no(++last_no)
    {}
    int get_days(){
        return days;
    }
    DayOfYear get_from(){
        return from;
    }
    DayOfYear get_until(){
        DayOfYear until(from);
        for(int i=0; i<get_days(); ++i){
             ++until;
        }
        return until;
    }
    void print(){
        cout << get_from() << " to " << get_until() << ", rental no " << this->no << " for " << this->customer;
    }
};

int Rental::last_no = 0;

class Schedule{
private:
    Vehicle* vehicle;
    Rental* year[365];
    Schedule* next;
public:
    Schedule(Vehicle* vehicle): vehicle(vehicle), next(nullptr){
        for(unsigned int i=0; i<365; ++i){
            year[i] = nullptr;
        }
    }
    Vehicle* get_vehicle(){
        return vehicle;
    }
    Schedule* get_next(){
        return next;
    }
    void set_next(Schedule* next){
        this->next = next;
    }
    bool isFree(DayOfYear from, int days){
        for(int i=0; i<days; ++i){
            if( year[from.get_dayOfYear() + i] != nullptr ){
                return false;
            }
        }
        return true;
    }
    float book(string customer, DayOfYear from, int days){
        year[from.get_dayOfYear()] = new Rental(customer, from, days);
        for(int i=0; i<days; ++i){
            year[from.get_dayOfYear() + i + 1]= year[from.get_dayOfYear()];
        }
        return vehicle->get_price(days);
    }
    void print(DayOfYear D){
        get_vehicle()->print();
        if(year[D.get_dayOfYear()]!=nullptr){
            year[D.get_dayOfYear()]->print();
        }
    }
    void print(){
        get_vehicle()->print();
        for(int i=0; i<365; ++i){
            if(year[i]!=nullptr){
                year[i]->print();
                short unsigned int r = year[i]->get_days();
                cout << " , total: " << get_vehicle()->get_price(r) << " EUR" << endl;
                i = i + r;
            }
        }
    }
};

class Agency{
private:
    float profitPercent;
    float profit;
    Schedule* head;
    Schedule* last;
protected:
    Schedule* searchFor(int no){
        Schedule* p(head);
        while(p != nullptr){
            if(p->get_vehicle()->get_no() == no){
                return p;
            }
            p=p->get_next();
        }
        return nullptr;
    }
public:
    Agency(): profitPercent(0.20), profit(0.00), head(nullptr), last(nullptr)
    {}
    float get_profit(){
        return profit;
    }
    void set_profitPercent(float profitPercent){
        this->profitPercent = profitPercent/100;
    }
    void add(Vehicle* vehicle){
        Schedule* temp = new Schedule(vehicle);
        if(head == nullptr){
            head = temp;
            last = temp;
        }else{
            last->set_next(temp);
            last = temp;
        }
    }
    bool isFree(int no, DayOfYear from, int days){
        return this->searchFor(no)->isFree(from, days);
    }
    float book(int no, string customer, DayOfYear from, int days){
        profit += profitPercent * this->searchFor(no)->book(customer, from, days);
        return this->searchFor(no)->get_vehicle()->get_price(days);
    }
    int chooseVehicle(){
        Schedule* temp(head);
        if(head == nullptr){
            cout << "This list is empty!" <<endl;
            return 0;
        }else{
            while(temp != nullptr){
                temp->get_vehicle()->print();
                temp = temp->get_next();
            }
            short unsigned int ans;
            cout << "choose vehicle no: ";
            cin >> ans;
            return ans;
        }
    }
    void print(DayOfYear D){
        Schedule* temp(head);
        if(head == nullptr){
            cout << "This list is empty!" <<endl;
        }else{
            while(temp != nullptr){
                temp->print(D);
                temp = temp->get_next();
            }
        }
    }
    void print(){
        Schedule* temp(head);
        if(head == nullptr){
            cout << "This is empty! There are no bookings." <<endl;
        }else{
            while(temp != nullptr){
                cout << "SCHEDULE FOR " ;
                temp->print();
                temp = temp->get_next();
            }
        }
        cout<< "Profit = " << fixed << setprecision(2) << get_profit() << " EUR" << endl;
    }
};

int main()
{
    Agency agency; // used for creating the agency
    char answer; // used for selection as input buffer
    unsigned int n; // used for how many vehicles to add in simulation as input buffer
    DayOfYear today(01, 01, 2021); // used for default date
    DayOfYear rentalDay; // used for the renting day as input buffer
    DayOfYear RandomDay; // used for randomizing a day
    string renter; // used for the name of customer as input buffer
    int daysOfRenting; // used for the number of days to rent as input buffer
    int chosenVehicle; // used for the selected vehicle to rent
    unsigned int numerationOfBikes=0; // used for the bike number,id
    unsigned int numerationOfEBikes=0; // used for the E-bike number, id
    unsigned int numerationOfEScooters=0; // used for the E-scooter number id
    unsigned int numerationOfVehicles=0; // used for the number of vehicles created
    long unsigned int seed = 3; // used for starting random value generator
    unsigned int numerationOfRandomCustomer=0; // used for numerating the random customers

    cout << "SIMPLIFIED SIMULATION OF A RENTAL AGENCY" << endl
         << "========================================" << endl;

    do{
        cout << endl
             << "----------------------------------------" << endl
             << "MENUE"                                    << endl
             << "A end of simulation"                      << endl
             << "B set new simulation date"                << endl
             << "C new rental manually"                    << endl
             << "D print rental of today " << today        << endl;
        DayOfYear temp(today);
        const DayOfYear tom(++temp);
        cout << "E print rental of tomorrow " << tom       << endl
             << "F print all rentals"                      << endl
             << "G print agency profit"                    << endl
             << "H set agency profit percent"              << endl
             << "I add bikes"                              << endl
             << "J add E-bikes"                            << endl
             << "K add E-scooters"                         << endl
             << "L new rental simulation"                  << endl <<endl
             << "your choice: ";
        cin >> answer;
        switch(answer){
            case 'a':
            case 'A':
                break;
            case 'b':
            case 'B':
                cout << "input date of today: ";
                cin >> temp;
                today=temp;
                break;
            case 'c':
            case 'C':
                chosenVehicle = agency.chooseVehicle();
                if(chosenVehicle == 0){
                    cerr << "No Vehicles were added!" <<endl;
                }else{
                    cout << "day of rental: ";
                    cin >> rentalDay;
                    if( ( (rentalDay.get_dayOfYear() < today.get_dayOfYear()) && (rentalDay.get_year() == today.get_year()) ) || (rentalDay.get_year() < today.get_year()) ) {
                        cerr << "This data is already in the past! Please input a date starting from today!" <<endl;
                        break;
                    }
                    cout << "number of days: ";
                    cin >> daysOfRenting;
                    if(daysOfRenting > 28){
                        daysOfRenting = 28;
                        cerr << "Maximum days of rental allowed is 28. Hence, it is set to 28 days instead." <<endl;
                    }
                    if(agency.isFree(chosenVehicle, rentalDay, daysOfRenting) == true){
                        cout << "input data vehicle renter name: ";
                        cin >> renter;
                        cout << "booked, price for this rental: " << agency.book(chosenVehicle, renter, rentalDay, daysOfRenting) << " EUR" <<endl;
                    }else
                        cerr << "sorry vehicle already booked" << endl;
                }
                break;
            case 'd':
            case 'D':
                cout << "SCHEDULE FOR " << today << endl;
                agency.print(today);
                break;
            case 'e':
            case 'E':
                cout << "SCHEDULE FOR " << tom << endl;
                agency.print(tom);
                break;
            case 'f':
            case 'F':
                agency.print();
                break;
            case 'g':
            case 'G':
                cout << "agency profit: " << fixed << setprecision(2) << agency.get_profit() << " EUR" <<endl;
                break;
            case 'h':
            case 'H':
                cout << "agency profit percent: ";
                float a;
                cin >> a;
                agency.set_profitPercent(a);
                break;
            case 'i':
            case 'I':
                cout << "how many Bikes in this simulation? ";
                cin >> n;
                for(unsigned int i=0; i<n; ++i)
                    agency.add(new Bike(++numerationOfVehicles, "City"+to_string(++numerationOfBikes)));
                break;
            case 'j':
            case 'J':
                cout << "how many EBikes in this simulation? ";
                cin >> n;
                for(unsigned int i=0; i<n; ++i)
                    agency.add(new EBike(++numerationOfVehicles, "Trekky"+to_string(++numerationOfEBikes)));
                break;
            case 'k':
            case 'K':
                cout << "how many EScooters in this simulation? ";
                cin >> n;
                for(unsigned int i=0; i<n; ++i)
                    agency.add(new EScooter(++numerationOfVehicles, "Scooty"+to_string(++numerationOfEScooters)));
                break;
            case 'l':
            case 'L':
                cout << "how many reservations to simulate? ";
                cin >> seed;
                srand(seed);
                for(long unsigned int i=0; i<seed; ++i){
                    DayOfYear RandomDay( today.get_dayOfYear() + (rand() % (365-today.get_dayOfYear()) ), 2021);
                    int RandomPeriod = 1 + (rand() % 7);
                    int numeration = 1 + (rand() % numerationOfVehicles);

                    if(agency.isFree(numeration, RandomDay, RandomPeriod) == true){
                        cout << "rental vehicle " << numeration << " from " << RandomDay << " for " << RandomPeriod << " days " << endl;
                        agency.book(numeration, "customer"+to_string(++numerationOfRandomCustomer), RandomDay, RandomPeriod);
                    }else
                        cout << "rental vehicle " << numeration << " from " << RandomDay << " for " << RandomPeriod << " days sorry vehicle already booked" << endl;
                }
                break;
        }
    }while( answer != 'a' && answer != 'A' );

    return 0;
}
