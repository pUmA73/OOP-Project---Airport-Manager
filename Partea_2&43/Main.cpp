#include <iostream>
#include <iomanip>
#include <conio.h>
#include <memory>
#include <utility>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>

#define COLOR_RESET "\033[0m"
#define COLOR_SELECTED "\033[1;32m" // green color for selected option
#define COLOR_DEFAULT "\033[1;37m" // white color for default options

class Menu	///Singleton design pattern
{
private:
	Menu() {}	///constructor set to private

	~Menu() {}
public:
	Menu(const Menu& obj) = delete;		///deleting the copy constructor

	void operator= (const Menu&) = delete;

	static Menu& getInstance()
	{
		static Menu* instance = new Menu;	///static pointer to the only instance of the class
		return *instance;
	}

	void showMenu(const std::vector<std::string> options, const std::string menuName, int& choice)
	{
		int position = 0;	///the position of the arrow
		int keyPressed = 0;

		while (keyPressed != 13)	///while enter is not pressed
		{
			system("cls");
			std::cout << std::setw(64) << menuName << "\n\n\n";

			for (int i = 0; i < options.size(); ++i)
			{
				if (i == position)
					std::cout << COLOR_SELECTED << " > " << options[i] << COLOR_RESET << "\n";
				else
					std::cout << COLOR_DEFAULT << "   " << options[i] << COLOR_RESET << "\n";

			}

			keyPressed = _getch();
			if (keyPressed == 80 && position < options.size() - 1)		///for arrow down
				position++;
			else if (keyPressed == 72 && position > 0)	///for arrow up
				position--;
		}
		choice = position;
	}
};

class Ticket
{
protected:
	double price; ///in euro
public:
	Ticket() : price(0) {}

	Ticket(double pr) : price(pr) {}

	virtual ~Ticket() {}

	virtual double getPrice()
	{
		return price;
	}
};

class TicketOnSale : virtual public Ticket
{
protected:
	double percentage;
public:
	TicketOnSale() : percentage(5), Ticket() {}

	TicketOnSale(double pr) : percentage(5), Ticket(pr) {}

	TicketOnSale(double pr, double per) : percentage(per), Ticket(pr) {}

	void setPercentage(double pr)
	{
		percentage = pr;
	}

	double getPrice()
	{
		return price - double(percentage * price / 100);
	}
};

class CloseDateTicket : virtual public Ticket
{
protected:
	bool closeDate;
public:
	CloseDateTicket() : closeDate(1), Ticket() {}

	CloseDateTicket(double pr) : closeDate(1), Ticket(pr) {}

	CloseDateTicket(double pr, bool close) : closeDate(close), Ticket(pr) {}

	void setClose(bool close)
	{
		closeDate = close;
	}

	double getPrice()
	{
		return price - double(closeDate * 10 * price / 100);
	}
};

class CloseDateTicketOnSale : public CloseDateTicket, public TicketOnSale
{
public:
	CloseDateTicketOnSale() : TicketOnSale(), CloseDateTicket() {}

	CloseDateTicketOnSale(double pr) : TicketOnSale(pr), CloseDateTicket(pr) {}

	CloseDateTicketOnSale(double pr, double per, bool close) : TicketOnSale(pr, per), CloseDateTicket(pr, close) {}

	double getPrice()
	{
		double totalPercentage = percentage + closeDate * 10;
		return price - double(totalPercentage * price / 100);
	}
};

template <class T>
class ObjectPool		///Object Pool design pattern
{
private:
	std::vector <std::shared_ptr<T>> pool;
public:
	ObjectPool(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			pool.push_back(std::make_shared<T>());
		}
	}

	std::shared_ptr<T> acquireObject()
	{
		if (pool.empty())
		{
			std::shared_ptr<T> newObject = std::make_shared<T>();
			///pool.push_back(newObject);
			return newObject;
		}

		std::shared_ptr<T> obj = pool.back();
		pool.pop_back();
		return obj;
	}

	void releaseObject(std::shared_ptr<T> object)
	{
		pool.push_back(object);
	}
};

class Plane
{
private:
	std::string make;
	std::string model;
	std::string registration;
	std::string type;
	int seatCount;
	int cargoSpace;
	double fuelConsumption; ///per 100 miles or km
public:
	Plane() : make(""), model(""), registration(""), type(""), seatCount(0), cargoSpace(0), fuelConsumption(0) {}

	Plane(const std::string make, const std::string model, const std::string registration, const std::string type,
		int seatCount, int cargoSpace, double fuelConsumption)
	{
		this->make = make;
		this->model = model;
		this->registration = registration;
		this->type = type;
		this->seatCount = seatCount;
		this->cargoSpace = cargoSpace;
		this->fuelConsumption = fuelConsumption;
	}

	///Setteri
	const void setSeatCount(int seats)
	{
		seatCount = seats;
	}

	const void setCargoSpace(int cargo)
	{
		cargoSpace = cargo;
	}

	const void setFuelConsumption(double consumption)
	{
		fuelConsumption = consumption;
	}

	///Getteri
	const int getSeatCount()
	{
		return seatCount;
	}

	const int getCargoSpace()
	{
		return cargoSpace;
	}

	const double getFuelConsumption()
	{
		return fuelConsumption;
	}

	friend std::ostream& operator << (std::ostream& out, const Plane& p);
};

std::ostream& operator << (std::ostream& out, const Plane& p)
{
	out << "Make: " << p.make << "\n";
	out << "Model: " << p.model << "\n";
	out << "Registration: " << p.registration << "\n";
	out << "Type: " << p.type << "\n";
	out << "Seats: " << p.seatCount << "\n";
	out << "Cargo: " << p.cargoSpace << " kg" << "\n";
	out << "Consumption: " << p.fuelConsumption << "l / 1h" << "\n";
	return out;
}

class FlightNotFound : public std::exception
{
private:
	std::string flightID;
public:
	FlightNotFound(const std::string ID) : flightID(ID) {}

	std::string what() 
	{
		std::string message = "Flight " + flightID + " not found!";
		return message;
	}
};

class Flight
{
private:
	std::string id;
	std::string destination;
	std::string takeoff;	///takeoff time
	double duration;		///flight duration in hours
	double price;			///ticket price
	bool state;				///1-active and 0-canceled		
public:
	Flight() : id(""), destination(""), takeoff(""), duration(0), price(0), state(0) {}

	Flight(const std::string id, const std::string destination, const std::string takeoff, double duration, double price, bool state)
	{
		this->id = id;
		this->destination = destination;
		this->takeoff = takeoff;
		this->duration = duration;
		this->price = price;
		this->state = state;
	}

	std::string getID() const
	{
		return id;
	}

	void setState(bool state) 
	{
		this->state = state;
	}

	void showProfit(Plane& plane)
	{
		double passengerCost = price * plane.getSeatCount();
		double flightCost = duration * plane.getFuelConsumption();

		if (passengerCost >= flightCost)
		{
			std::cout << "Profit: " << passengerCost - flightCost << "$" << "\n";
			return;
		}
		std::cout << "Loss: " << flightCost - passengerCost << "$" << "\n";
	}
};

class Employee
{
protected:
	std::string firstName;
	std::string lastName;
	int age;
	int experience;		///in years
	double salary;		///in dollars per year
public:
	Employee() : firstName(""), lastName(""), age(0), experience(0), salary(0) {}

	Employee(const std::string fname, const std::string lname, int a, int e, double s)
	{
		firstName = fname;
		lastName = lname;
		age = a; experience = e; salary = s;
	}

	virtual ~Employee() {}

	const double getSalary()
	{
		return salary;
	}

	std::string getName()
	{
		return firstName + " " + lastName;
	}

	void monthlySalary()
	{
		std::cout << std::fixed << std::setprecision(2) << double(salary) / 12 << "$ / month" << "\n";
	}

	//virtual void addExperience() = 0;

	friend std::ostream& operator << (std::ostream& out, const Employee& e);
};

std::ostream& operator << (std::ostream& out, const Employee& e)
{
	out << "Name: " << e.firstName << " " << e.lastName << "\n";
	out << "Age: " << e.age << " years" << "\n";
	out << "Experience: " << e.experience << " years" << "\n";
	out << "Salary: " << e.salary << "$ / year" << "\n";
	return out;
}

class PilotNotFound : std::exception 
{
private:
	std::string name;
public:
	PilotNotFound(std::string n) : name(n) {}

	std::string what()
	{
		std::string message = name + " has not been found in the pilot list!";
		return message;
	}
};

class Pilot : public Employee
{
private:
	std::string license;
	std::string company;
public:
	Pilot() : Employee() {}

	Pilot(const std::string fname, const std::string lname, int a, int e, double s, const std::string l, const std::string c) : Employee(fname, lname, a, e, s)
	{
		license = l;
		company = c;
	}

	void addExperience()
	{
		experience++;
		salary += (5 * salary / 100);	///5% raise
	}
};

class FlightAttendant : public Employee
{
private:
	std::vector <std::string> languages;
	std::string company;
public:
	FlightAttendant() : languages(), company(""), Employee() {}

	FlightAttendant(const std::string fname, const std::string lname, int a, int e, double s, const std::string c, const std::vector <std::string> lang) :
		Employee(fname, lname, a, e, s)
	{
		company = c;
		languages = lang;
	}

	const void getLanguages()
	{
		std::cout << "Name: " << firstName << " " << lastName << "\n";
		std::cout << "Spoken languages: ";
		for (int i = 0; i < languages.size(); ++i)
			std::cout << languages[i] << " ";
		std::cout << "\n";
	}

	void addExperience()
	{
		experience++;
		salary += (2 * salary / 100);	///2% raise
	}
};

class IAirportManager		///interface class for AirportManager
{
public:
	virtual ~IAirportManager() {}

	virtual void cancelFlight(const std::string flightID) = 0;

	virtual void addFlight(const std::string id, const std::string destination, const std::string takeoff, double duration, double price) = 0;
};

class AirportManager : public IAirportManager
{
private:
	std::vector <Plane> planes;
	std::vector <Employee*> employees;
	std::vector <Flight> flights;
public:
	AirportManager() {}

	AirportManager(const std::vector <Plane> planes, const std::vector <Employee*> employees, const std::vector <Flight> flights)
	{
		this->planes = planes;
		this->flights = flights;
		this->employees = employees;
	}

	void findPilot(const std::string name)
	{
		try
		{
			bool found = 0;
			for (int i = 0; i < employees.size(); ++i)
				if (Pilot* p = dynamic_cast<Pilot*>(employees[i]))
				//if(typeid(*employees[i]) == typeid(Pilot))
					if (employees[i]->getName() == name)
					{
						found = 1;
						break;
					}
			if (!found)
				throw(PilotNotFound(name));
			else
				std::cout << "Pilot " << name << " has been found!" << "\n";
		}
		catch (PilotNotFound err)
		{
			std::cout << "Error: " << err.what() << "\n";
		}
	}

	void cancelFlight(std::string flightID)
	{
		for (int i = 0; i < flights.size(); ++i)
			if (flights[i].getID() == flightID)
			{
				flights[i].setState(0);
				return;
			}
	}

	void addFlight(const std::string id, const std::string destination, const std::string takeoff, double duration, double price)
	{
		flights.push_back(Flight(id, destination, takeoff, duration, price, 1));		///state set to 1, flight is active
	}

	void findFlight(const std::string ID)
	{
		try
		{
			bool found = 0;
			for (int i = 0; i < flights.size() && !found; ++i)
				if (flights[i].getID() == ID)
					found = 1;

			if (!found)
				throw FlightNotFound(ID);
			else
				std::cout << "Flight ID was found!" << "\n";
		}
		catch (FlightNotFound err)
		{
			std::cout << "Error: " << err.what() << "\n";
		}
	}

	void showFlights()
	{
		for (int i = 0; i < flights.size(); ++i)
			std::cout << flights[i].getID() << "\n";
	}

	void showEmployees()
	{
		for (int i = 0; i < employees.size(); ++i)
			std::cout << employees[i]->getName() << "\n";
		if (employees.empty())
			std::cout << "There are no employees at the moment" << "\n";
	}
};

template <typename T, typename Attribute>
void sortByAttrib(std::vector<T>& elements, Attribute(T::* getter)() const)
{
	std::sort(elements.begin(), elements.end(), [=](const T& a, const T& b) { return (a.*getter)() < (b.*getter)(); });
}

int main()
{
	Flight f1("F100", "Madagascar", "13:20", 4, 250, 1);
	Flight f2("F101", "London", "08:20", 3.5, 220, 1);
	Flight f3("F102", "Ireland", "14:20", 6, 350, 1);
	Flight f4("F103", "Tenerife", "04:40", 10, 1250, 1);

	std::vector<Flight> F;
	F.push_back(f1); F.push_back(f2); F.push_back(f3); F.push_back(f4);

	Plane p1, p2;
	std::vector<Plane> P;
	P.push_back(p1); P.push_back(p2);

	std::vector<Employee*> E;
	Pilot* ptr1 = new Pilot("Andrew", "Smith", 23, 3, 80000, "Comercial", "Ryanair");
	Pilot* ptr2 = new Pilot("Dave", "Andertons", 40, 18, 100000, "Comercial", "Vueling");
	Pilot* ptr3 = new Pilot("Boris", "Rosberg", 23, 2, 32000, "Private", "None");
	E.push_back(ptr1); E.push_back(ptr2); E.push_back(ptr3);
	//std::cout << ptr1->getName() << "\n";

	AirportManager Air(P, E, F);

	//Air.findFlight("F120");

	//Air.showEmployees();

	std::string name = "Rada Rada";

	Air.findPilot(name);
	

	return 0;
}