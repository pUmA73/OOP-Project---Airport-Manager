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
	std::string id;
public:
	Ticket() : price(0) {}

	Ticket(double pr, const std::string i) : price(pr), id(i) {}

	virtual ~Ticket() {}

	virtual double getPrice()
	{
		return price;
	}

	void setPrice(int price)
	{
		this->price = price;
	}

	void setID(const std::string id)
	{
		this->id = id;
	}

	std::string getID()
	{
		return id;
	}
};

class TicketOnSale : virtual public Ticket
{
protected:
	double percentage;
public:
	TicketOnSale() : percentage(5), Ticket() {}

	TicketOnSale(double pr, const std::string i) : percentage(5), Ticket(pr, i) {}

	TicketOnSale(double pr, double per, const std::string i) : percentage(per), Ticket(pr, i) {}

	void setPercentage(double pr)
	{
		percentage = pr;
	}

	double getPercentage()
	{
		return percentage;
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

	CloseDateTicket(double pr, const std::string i) : closeDate(1), Ticket(pr, i) {}

	CloseDateTicket(double pr, bool close, const std::string i) : closeDate(close), Ticket(pr, i) {}

	void setClose(bool close)
	{
		closeDate = close;
	}

	bool getClose()
	{
		return closeDate;
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

	CloseDateTicketOnSale(double pr, const std::string i) : TicketOnSale(pr, i), CloseDateTicket(pr, i) {}

	CloseDateTicketOnSale(double pr, double per, bool close, const std::string i) : TicketOnSale(pr, per, i), CloseDateTicket(pr, close, i) {}

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

template<>
class ObjectPool <Ticket>
{
private:
	std::vector <std::shared_ptr<Ticket>> pool;
public:
	ObjectPool(int size1, int size2, int size3, int size4)
	{
		for (int i = 0; i < size1; ++i)
		{
			pool.push_back(std::make_shared<Ticket>());
		}
		for (int i = 0; i < size2; ++i)
		{
			pool.push_back(std::make_shared<CloseDateTicket>());
		}
		for (int i = 0; i < size3; ++i)
		{
			pool.push_back(std::make_shared<TicketOnSale>());
		}
		for (int i = 0; i < size4; ++i)
		{
			pool.push_back(std::make_shared<CloseDateTicketOnSale>());
		}
	}

	//std::shared_ptr<Ticket> acquireObject()
	//{
	//	if (pool.empty())
	//	{
	//		std::shared_ptr<Ticket> newObject = std::make_shared<Ticket>();
	//		return newObject;
	//	}

	//	std::shared_ptr<Ticket> obj = pool.back();
	//	pool.pop_back();
	//	return obj;
	//}

	std::shared_ptr<Ticket> acquireTicket()
	{
		for (int i = 0; i < pool.size(); ++i)
			if (dynamic_cast<Ticket*>(pool[i].get()))
			{
				std::shared_ptr<Ticket> obj = pool[i];
				pool.erase(pool.begin() + i);
				return obj;
			}
		std::shared_ptr<Ticket> newObj = std::make_shared<Ticket>();
		return newObj;
	}

	std::shared_ptr<Ticket> acquireTicketOnSale()
	{
		for (int i = 0; i < pool.size(); ++i)
			if (dynamic_cast<TicketOnSale*>(pool[i].get()))
			{
				std::shared_ptr<Ticket> obj = pool[i];
				pool.erase(pool.begin() + i);
				return obj;
			}
		std::shared_ptr<Ticket> newObj = std::make_shared<TicketOnSale>();
		return newObj;
	}

	std::shared_ptr<Ticket> acquireCloseDateTicket()
	{
		for (int i = 0; i < pool.size(); ++i)
			if (dynamic_cast<CloseDateTicket*>(pool[i].get()))
			{
				std::shared_ptr<Ticket> obj = pool[i];
				pool.erase(pool.begin() + i);
				return obj;
			}
		std::shared_ptr<Ticket> newObj = std::make_shared<CloseDateTicket>();
		return newObj;
	}

	std::shared_ptr<Ticket> acquireCloseDateTicketOnSale()
	{
		for (int i = 0; i < pool.size(); ++i)
			if (dynamic_cast<CloseDateTicketOnSale*>(pool[i].get()))
			{
				std::shared_ptr<Ticket> obj = pool[i];
				pool.erase(pool.begin() + i);
				return obj;
			}
		std::shared_ptr<Ticket> newObj = std::make_shared<CloseDateTicketOnSale>();
		return newObj;
	}

	void releaseObject(std::shared_ptr<Ticket> object)
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
	double fuelConsumption; ///per 100 km
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
	void setSeatCount(int seats)
	{
		seatCount = seats;
	}

	void setCargoSpace(int cargo)
	{
		cargoSpace = cargo;
	}

	void setFuelConsumption(double consumption)
	{
		fuelConsumption = consumption;
	}

	///Getteri
	std::string getRegistration() const
	{
		return registration;
	}

	int getSeatCount() const
	{
		return seatCount;
	}

	int getCargoSpace() const
	{
		return cargoSpace;
	}

	double getFuelConsumption() const
	{
		return fuelConsumption;
	}

	friend std::ostream& operator << (std::ostream& out, const Plane& p);
	friend std::istream& operator >> (std::istream& input, Plane& plane);
};

std::istream& operator>> (std::istream& input, Plane& plane) {
	std::cout << "Make: "; input >> plane.make;
	std::cout << "Model: "; input >> plane.model;
	std::cout << "Registration: "; input >> plane.registration;
	std::cout << "Type: "; input >> plane.type;
	std::cout << "Seats: "; input >> plane.seatCount;
	std::cout << "Cargo: "; input >> plane.cargoSpace;
	std::cout << "Consumption: "; input >> plane.fuelConsumption;
	return input;
}

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

	double getPrice()
	{
		return price;
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
	friend std::ostream& operator << (std::ostream& out, const Flight& f);
	friend std::istream& operator >> (std::istream& input, Flight& f);
};

std::istream& operator>> (std::istream& input, Flight& f) {
	std::cout << "ID: "; input >> f.id;
	std::cout << "Destination: "; input >> f.destination;
	std::cout << "Takeoff: "; input >> f.takeoff;
	std::cout << "Duration: "; input >> f.duration;
	std::cout << "Price: "; input >> f.price;
	std::cout << "State: "; input >> f.state;	///1-active and 0-canceled
	return input;
}

std::ostream& operator << (std::ostream& out, const Flight& f)
{
	out << "ID: " << f.id << "\n";
	out << "Destination: " << f.destination << "\n";
	out << "Takeoff: " << f.takeoff << "\n";
	out << "Duration: " << f.duration << "\n";
	out << "Price: " << f.price << "\n";
	out << "State: ";
	if(f.state)
		out << "Active" << "\n";
	else
		out << "Canceled" << "\n";
	return out;
}

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

	~AirportManager()
	{
		for (int i = 0; i < employees.size(); ++i)
			delete employees[i];
		planes.clear();
		employees.clear();
		flights.clear();
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

	void addPlane(Plane p)
	{
		planes.push_back(p);
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

	double getFlightPrice(const std::string id)
	{
		for (int i = 0; i < flights.size(); ++i)
			if (flights[i].getID() == id)
				return flights[i].getPrice();
	}

	void showFlights()
	{
		for (int i = 0; i < flights.size(); ++i)
			std::cout << flights[i] << "\n\n";
	}

	void showEmployees()
	{
		for (int i = 0; i < employees.size(); ++i)
			std::cout << *employees[i] << "\n";
		if (employees.empty())
			std::cout << "There are no employees at the moment" << "\n";
	}

	void showPlanes()
	{
		for (int i = 0; i < planes.size(); ++i)
			std::cout << planes[i] << "\n\n";
	}

	bool removePlane(std::string reg)
	{
		for (int i = 0; i < planes.size(); ++i)
			if (reg == planes[i].getRegistration())
			{
				planes.erase(planes.begin() + i);
				return 1;
			}
		return 0;
	}
};

template <typename T, typename Attribute>
void sortByAttrib(std::vector<T>& elements, Attribute(T::* getter)() const)
{
	std::sort(elements.begin(), elements.end(), [=](const T& a, const T& b) { return (a.*getter)() < (b.*getter)(); });
}

bool match(std::string flightID, std::string ticketID)
{
	return flightID.substr(1, 3) == ticketID.substr(1, 3);
}

void ticketModify(std::shared_ptr<Ticket> ticket, std::string id, double price)
{
	ticket->setID(id);
	ticket->setPrice(price);
}

int main()
{
	Flight f1("F100", "Madagascar", "13:20", 4, 250, 1);
	Flight f2("F101", "London", "08:20", 3.5, 220, 1);
	Flight f3("F102", "Ireland", "14:20", 6, 350, 1);
	Flight f4("F103", "Tenerife", "04:40", 10, 1250, 1);

	std::vector<Flight> F;
	F.push_back(f1); F.push_back(f2); F.push_back(f3); F.push_back(f4);

	/*for (int i = 0; i < F.size(); ++i)
		std::cout << F[i] << "\n";

	sortByAttrib(F, &Flight::getID);
	std::cout << "\n\n";

	for (int i = 0; i < F.size(); ++i)
		std::cout << F[i] << "\n";*/

	Plane p1("Boeing", "747", "NF100", "Airliner", 230, 1000, 130);
	Plane p2("Cessna", "Xp140", "NR450", "Private", 8, 200, 30);
	std::vector<Plane> P;
	P.push_back(p1); P.push_back(p2);



	std::vector<Employee*> E;
	Pilot* ptr1 = new Pilot("Andrew", "Smith", 23, 3, 80000, "Comercial", "Ryanair");
	Pilot* ptr2 = new Pilot("Dave", "Andertons", 40, 18, 100000, "Comercial", "Vueling");
	Pilot* ptr3 = new Pilot("Boris", "Rosberg", 23, 2, 32000, "Private", "None");
	E.push_back(ptr1); E.push_back(ptr2); E.push_back(ptr3);
	////std::cout << ptr1->getName() << "\n";

	AirportManager Air(P, E, F);

	////Air.findFlight("F120");

	////Air.showEmployees();

	//std::string name = "Rada Rada";

	//Air.findPilot(name);
	
	ObjectPool<Ticket> tickets(1, 1, 1, 1);
	std::vector<std::shared_ptr<Ticket>> client;
	
	/*std::shared_ptr<Ticket> t1 = std::make_shared<Ticket>(250, "T100.001");
	std::shared_ptr<Ticket> t2 = std::make_shared<Ticket>(220, "T101.001");
	std::shared_ptr<Ticket> t3 = std::make_shared<Ticket>(350, "T102.001");
	std::shared_ptr<Ticket> t4 = std::make_shared<CloseDateTicket>(1250, "T103.001");

	tickets.releaseObject(t1); tickets.releaseObject(t2);
	tickets.releaseObject(t3); tickets.releaseObject(t4);*/

	std::vector<std::string> menuMain = { "Planes", "Flights", "Pilots", "Flight Attendants", "Bookings", "Date and Time", "Exit" };
	std::string main = "MAIN MENU";

	std::vector<std::string> menuPlanes = { "Show Planes", "Add Plane", "Remove Plane", "Back" };
	std::string planes = "PLANES MENU";

	std::vector<std::string> menuFlights = { "Show Flights", "Book Flight", "Cancel Flight", "Back" };
	std::string flights = "FLIGHTS MENU";

	std::vector<std::string> menuPilots = { "Show Pilots", "Add Pilot", "Remove Pilot", "Back" };
	std::string pilots = "PILOTS MENU";

	std::vector<std::string> menuFA = { "Show Flight Attendants", "Add Flight Attendant", "Remove Flight Attendant", "Back" };
	std::string FA = "FLIGHT ATTENDANTS MENU";

	std::vector<std::string> menuBookings = { "Show Bookings", "Cancel Booking", "Back" };
	std::string bookings = "BOOKINGS MENU";

	int choice0, choice1, choice2;

	int key = 0;

	choice0 = choice1 = choice2 = 0;

	while (choice0 != 6)
	{
		choice1 = 0;
		Menu::getInstance().showMenu(menuMain, main, choice0);
		if (choice0 == 0)
			while (choice1 != 3)
			{
				Menu::getInstance().showMenu(menuPlanes, planes, choice1);
				if (choice1 == 0)
					do
					{
						system("cls");
						std::cout << "Printing all planes!" << "\n\n";
						Air.showPlanes();
						std::cout << "\n" << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				else if (choice1 == 1)
				{
					system("cls");
					Plane temp;
					std::cin >> temp;
					Air.addPlane(temp);
					do
					{
						system("cls");
						std::cout << "\n" << "Plane has been added to the database!" << "\n";
						std::cout << "\n" << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 2)
				{
					system("cls");
					std::string reg;
					std::cout << "Enter Plane Registration: ";
					std::cin >> reg;
					bool complete = Air.removePlane(reg);
					do
					{
						system("cls");
						if (complete)
							std::cout << "Plane has been removed from the database!" << "\n\n";
						else
							std::cout << "Plane not found!" << "\n\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 3)
					break;
			}
		else if (choice0 == 1)
			while (choice1 != 3)
			{
				Menu::getInstance().showMenu(menuFlights, flights, choice1);
				if (choice1 == 0)
					do
					{
						system("cls");
						std::cout << "Printing all flights!" << "\n\n";
						Air.showFlights();
						std::cout << "\n" << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				else if (choice1 == 1)
				{
					system("cls");
					std::string id;
					std::cout << "Enter Flight ID: ";
					std::cin >> id;
					do
					{
						client.push_back(tickets.acquireTicket());
						ticketModify(client[client.size() - 1], "T" + id.substr(1, 3) + ".001", Air.getFlightPrice(id));
						system("cls");
						std::cout << "Flight ticket booked!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 2)
				{
					system("cls");
					std::string id; 
					std::cout << "Enter Flight ID: ";
					std::cin >> id;
					Air.findFlight(id);
					Air.cancelFlight(id);
					do
					{
						system("cls");
						std::cout << "Flight has been canceled!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 3)
					break;
			}
		else if (choice0 == 2)
			while (choice1 != 3)
			{
				Menu::getInstance().showMenu(menuPilots, pilots, choice1);
				if (choice1 == 0)
					do
					{
						system("cls");
						std::cout << "Printing all pilots!" << "\n\n";
						Air.showEmployees();
						std::cout << "\n" << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				else if (choice1 == 1)
				{
					system("cls");
					std::string name;
					std::cout << "Enter Pilot Name: ";
					std::cin >> name;
					do
					{
						system("cls");
						std::cout << "Pilot has been added to the database!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 2)
				{
					system("cls");
					std::string name;
					std::cout << "Enter Pilot tName: ";
					std::cin >> name;
					do
					{
						system("cls");
						std::cout << "Pilot has been removed from the database!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 3)
					break;
			}
		else if (choice0 == 3)
			while (choice1 != 3)
			{
				Menu::getInstance().showMenu(menuFA, FA, choice1);
				if (choice1 == 0)
					do
					{
						system("cls");
						std::cout << "Printing all Flight Attendants!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				else if (choice1 == 1)
				{
					system("cls");
					std::string name;
					std::cout << "Enter Flight Attendant Name: ";
					std::cin >> name;
					do
					{
						system("cls");
						std::cout << "Flight Attendant has been added to the database!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 2)
				{
					system("cls");
					std::string name;
					std::cout << "Enter Flight Attendant Name: ";
					std::cin >> name;
					do
					{
						system("cls");
						std::cout << "Flight Attendant has been removed from the database!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 3)
					break;
			}
		else if (choice0 == 4)
			while (choice1 != 3)
			{
				Menu::getInstance().showMenu(menuBookings, bookings, choice1);
				if (choice1 == 0)
					do
					{
						system("cls");
						std::cout << "Printing all Tickets!" << "\n\n";
						if(!client.empty())
							for (int i = 0; i < client.size(); ++i)
								std::cout << "Ticket ID: " << client[i]->getID() << " | " << "Price: " << client[i]->getPrice() << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				else if (choice1 == 1)
				{
					system("cls");
					std::string id;
					std::cout << "Enter Ticket ID: ";
					std::cin >> id;
					do
					{
						for (int i = 0; i < client.size(); ++i)
							if (client[i]->getID() == id)
							{
								tickets.releaseObject(client[i]);
								client.erase(client.begin() + i);
								break;
							}
						system("cls");
						std::cout << "Booking canceled!" << "\n";
						std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
						key = _getch();
					} while (key != 13);
				}
				else if (choice1 == 2)
					break;
			}
		else if (choice0 == 5)
			do
			{
				system("cls");
				time_t now = time(0);
				char date_time[26];
				ctime_s(date_time, sizeof(date_time), &now);
				std::cout << "\n" << "Date and Time: " << date_time << "\n\n";
				std::cout << COLOR_SELECTED << " > " << "Back" << COLOR_RESET << "\n";
				key = _getch();
			} while (key != 13);
		else if(choice0 == 6)
			break;
	}

	return 0;
}
