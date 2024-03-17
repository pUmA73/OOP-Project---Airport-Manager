# OOP Project

## Project Subject
Airport / Airline management app

## Implemented Classes
The following classes have been implemented:
- Menu - singleton class for the menu of the console application
- Ticket
- TicketOnSale - inherits from virtually Ticket
- CloseDateTicket - inherits from virtually Ticket
- CloseDateTicketOnSale - inherits  from CloseDateTicket and TicketOnSale (diamond inheritance problem, the two classes CloseDateTicket and TicketOnSale inherit virtually from the base class to avoid duplicating fields)
- ObjectPool<T> - implements the object pool design pattern (template class)
- Plane
- FlightNotFound - exception class for handling flights that are not found
- Flight
- Employee
- Pilot - inherits from employee
- PilotNotFound - exception class for hanling pilots that are not found
- FlightAttendant - inherits from employee
- IAirportManager - interface defining operations that an airport manager should implement
- AirportManager - implements the IAirportManager interface, managing the overall operations of the airport, including planes, flights, and employees

## Functionalities
The app can manage most of the things that are present in an airport (i.e. planes, flights, employees, tickets). A simple menu is implemented for the user to interact with the app. Systems for management of the planes, flights, tickets, and employees are available.
