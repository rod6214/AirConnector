#include "TestMariaDB.h"

void testConn() {
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString user("root");
	sql::SQLString pwd("pass");
	std::unique_ptr<sql::Connection> conn(driver->connect("tcp://localhost:3306/test", user, pwd));

	// Create a new PreparedStatement
	std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("insert into persons (name, lastname) values (?, ?)"));

	sql::SQLString name("Jhon");
	sql::SQLString lastname("Doe");
	// Bind values to SQL statement
	stmnt->setString(1, name);
	stmnt->setString(2, lastname);

	// Execute query
	stmnt->executeQuery();

	conn->close();

}

void insertDataPlane(AirPlane* airplane)
{
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString user("root");
	sql::SQLString pwd("pass");
	std::unique_ptr<sql::Connection> conn(driver->connect("tcp://localhost:3306/simconnector", user, pwd));

	// Create a new PreparedStatement
	std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("insert into plane (title, kohlsmann, altitude, latitude) values (?, ?, ?, ?)"));

	stmnt->setString(1, airplane->title);
	stmnt->setDouble(2, airplane->kohlsmann);
	stmnt->setDouble(3, airplane->altitude);
	stmnt->setDouble(4, airplane->latitude);

	// Execute query
	stmnt->executeQuery();

	// Close connection
	conn->close();
}
