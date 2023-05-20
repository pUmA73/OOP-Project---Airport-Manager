#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

class String
{
private:
	char* data;
public:
	String() : data(nullptr) {}

	String(const char str[])
	{
		size_t len = strlen(str);
		data = new char[len + 1];
		for (int i = 0; i < len; ++i)
			data[i] = str[i];
		data[len] = '\0';
	}
	
	String(const String& source)	///Copy Constructor
	{
		data = new char[strlen(source.data) + 1];
		for (int i = 0; i < strlen(source.data); ++i)
			data[i] = source.data[i];
		data[strlen(source.data)] = '\0';
	}

	/*String(const String&& source) noexcept	///Move Constructor
	{
		data = source.data;
		source.data = nullptr;
	}*/

	~String() 
	{ 
		if(data != nullptr)
			delete[] data; 
	}

	String& operator = (const String& right)
	{
		if (this == &right)
			return *this;
		if(data != nullptr)
			delete[] data;
		size_t len = strlen(right.data);
		data = new char[len + 1];
		for (int i = 0; i < len; ++i)
			data[i] = right.data[i];
		data[len] = '\0';
		return *this;
	}

	/*String& operator = (const String&& other) noexcept
	{
		if (this == &other)
			return *this;
		delete[] data;
		int len = strlen(other.data);
		data = new char[len + 1];
		for (int i = 0; i < len; ++i)
			data[i] = other.data[i];
		data[len] = '\0';
		delete[] other.data;
		return *this;
	}*/

	void setString(const char s[])
	{
		size_t len = strlen(s);
		delete[] data;
		data = new char[len + 1];
		for (int i = 0; i < len; ++i)
			data[i] = s[i];
		data[len] = '\0';
	}

	int size()
	{
		int s = 0;
		for (int i = 0; data[i] != '\0'; ++i)
			s++;
		return s;
	}
	friend std::ostream& operator << (std::ostream& out, const String& s);
	friend std::istream& operator >> (std::istream& in, String& s);
};

std::ostream& operator << (std::ostream& out, const String& s)
{
	out << s.data;
	return out;
}

std::istream& operator >> (std::istream& in, String& s)
{
	char temp[100];
	in >> temp;
	s = String(temp);
	return in;
}

class Plane
{
private:
	String make;
	String model;
	String registration;
	String type;
	int seatCount;
	int cargoSpace;
	int fuelConsumption; ///per 100 miles or km
	///State
public:
	Plane()
	{
		std::cin >> make >> model >> registration >> type;
		std::cin.get();
		std::cin >> seatCount >> cargoSpace >> fuelConsumption;
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

	void setFuelConsumption(int consumption)
	{
		fuelConsumption = consumption;
	}

	///Getteri
	int getSeatCount()
	{
		return seatCount;
	}

	int getCargoSpace()
	{
		return cargoSpace;
	}

	int getFuelConsumption()
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

class Flight
{
private:
	int id;
	char destination[20];
	char takeoff[6];	///takeoff time
	int duration;		///flight duration in hours
	int price;			///ticket price
public:
	Flight()
	{
		std::cin >> id;
		std::cin.get();
		std::cin >> destination >> takeoff;
		std::cin.get();
		std::cin >> duration;
		std::cin >> price;
	}
	
	void showProfit(Plane& plane)
	{
		int seats = plane.getSeatCount();
		int passengerCost = price * seats;
		int flightCost = duration * plane.getFuelConsumption() * 2;

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
	String firstName;
	String lastName;
	int age;
	int experience;	///in years
	int salary;		///in dollars per year
public:
	Employee() : age(0), experience(0), salary(0) {}

	Employee(const char fname[], const char lname[], int a, int e, int s)
	{
		firstName.setString(fname);
		lastName.setString(lname);
		age = a; experience = e; salary = s;
	}

	Employee(const Employee& other)
	{
		firstName = other.firstName;
		lastName = other.lastName;
		age = other.age;
		experience = other.experience;
		salary = other.salary;
	}

	Employee& operator = (const Employee& other)
	{
		if (this == &other)
			return *this;
		firstName = other.firstName;
		lastName = other.lastName;
		age = other.age;
		experience = other.experience;
		salary = other.salary;
		return *this;
	}

	int getSalary()
	{
		return salary;
	}

	void monthlySalary()
	{
		std::cout << std::fixed << std::setprecision(2) << double(salary) / 12 << "$ / month" << "\n";
	}

	friend std::ostream& operator << (std::ostream& out, const Employee& e);
};

std::ostream& operator << (std::ostream& out, const Employee& e)
{
	out << "Name: " << e.firstName << " " << e.lastName <<"\n";
	out << "Age: " << e.age << " years" << "\n";
	out << "Experience: " << e.experience << " years" << "\n";
	out << "Salary: " << e.salary << "$ / year" << "\n";
	return out;
}

class Pilot : public Employee
{
private:	
	String license;
	String company;
public:
	Pilot() : Employee() {}

	 Pilot(const char fname[], const char lname[], int a, int e, int s, const char l[], const char c[]) : Employee(fname, lname, a, e, s)
	 {
		license.setString(l);
		company.setString(c);
	 }

	 Pilot(const Pilot& other)
	 {
		 firstName = other.firstName;
		 lastName = other.lastName;
		 age = other.age;
		 experience = other.experience;
		 salary = other.salary;
		 license = other.license;
		 company = other.company;
	 }

	 Pilot& operator = (const Pilot& other)
	 {
		 if (this == &other)
			 return *this;
		 firstName = other.firstName;
		 lastName = other.lastName;
		 age = other.age;
		 experience = other.experience;
		 salary = other.salary;
		 license = other.license;
		 company = other.company;
		 return *this;
	 }

	void getName()
	{
		std::cout << "Name: " << firstName << " " << lastName << "\n";
		std::cout << "Occupation: Pilot at " << company << "\n";
	}
};

class FlightAttendant : public Employee
{
private:
	String languages;
	String company;
public:
	FlightAttendant() : languages(nullptr), company(nullptr), Employee() {}

	FlightAttendant(const char fname[], const char lname[], int a, int e, int s, const char c[], const char lang[]) : Employee(fname, lname, a, e, s)
	{
		company.setString(c);
		languages.setString(lang);
	}

	void getLanguages()
	{
		std::cout << "Name: " << firstName << " " << lastName << "\n";
		std::cout << "Spoken languages: " << " " << languages << "\n";
	}
};

class Airport
{
private:
	Pilot* pilots;
	int pilotCount;
	FlightAttendant* flightAttendants;
	int flightAttendantCount;
	Plane* planes;
	int planeCount;
	Flight* flights;
	int flightCount;
public:
	Airport() : pilots(nullptr), pilotCount(0), flightAttendants(nullptr), flightAttendantCount(0),
		planes(nullptr), planeCount(0), flights(nullptr), flightCount(0) {}
	
	Airport(int pilotNo, int attendantNo, int planeNo, int flightNo) :
		pilotCount(pilotNo), flightAttendantCount(attendantNo), planeCount(planeNo), flightCount(flightNo)
	{
		pilots = new Pilot[pilotNo];
		flightAttendants = new FlightAttendant[attendantNo];
		planes = new Plane[planeNo];
		flights = new Flight[flightNo];

		for (int i = 0; i < pilotNo; ++i)
			pilots[i] = Pilot();
		for (int i = 0; i < attendantNo; ++i)
			flightAttendants[i] = FlightAttendant();
		for (int i = 0; i < planeNo; ++i)
			planes[i] = Plane();
		for (int i = 0; i < flightNo; ++i)
			flights[i] = Flight();
	}

	~Airport()
	{
		delete[] pilots;
		delete[] flightAttendants;
		delete[] planes;
		delete[] flights;
	}

	double avgSalary()
	{
		int total = 0, i;
		for (i = 0; i < pilotCount; ++i)
			total += pilots[i].getSalary();
		for (i = 0; i < flightAttendantCount; ++i)
			total += flightAttendants[i].getSalary();
		return (double)total / (pilotCount + flightAttendantCount);
	}

	void addPilot(const Pilot& p)
	{
		
		int newPilotCount = pilotCount + 1;
		Pilot* newPilots = new Pilot[newPilotCount];

		for (int i = 0; i < pilotCount; ++i)
			newPilots[i] = pilots[i]; 

		newPilots[newPilotCount - 1] = p;
		delete[] pilots;

		pilots = newPilots;
		pilotCount = newPilotCount;
	}

	void showPilots()
	{
		for (int i = 0; i < pilotCount; ++i)
			std::cout << pilots[i] << "\n";
	}
};

int main()
{
	Pilot a("James", "Andrews", 23, 2, 80000, "Comercial", "Ryanair");
	Pilot b("Ray", "Montee", 44, 21, 140000, "Private", "None");
	/*Plane p1;
	std::cout << p1 << "\n";
	std::cout << a << "\n" << b << "\n";
	a.monthlySalary();
	b.getName();*/
	Airport A;
	A.addPilot(a);
	A.addPilot(b);
	A.showPilots();
	
	
	return 0;
}