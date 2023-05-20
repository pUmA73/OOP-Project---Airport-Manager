#include <iostream>
#include <string>
#include <memory>

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

int main()
{
	Ticket t1(250);

	TicketOnSale(300, 10);

	ObjectPool<Ticket> TicketPool(2);

	std::shared_ptr<Ticket> ticket1 = TicketPool.acquireObject();

	if (ticket1)
		std::cout << "Acquired ticket!" << "\n";


	return 0;
}