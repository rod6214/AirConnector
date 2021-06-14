#include "MariaDB.h"
#include <stdio.h>

#include <string.h>
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

void DLLTEMPLATE_API insertDataPlane(std::shared_ptr<PlaneData> ptr_planedata)
{
	auto planedata = ptr_planedata.get();
	auto datetime = planedata->ptr_datetime.get()->c_str();
	auto airplane = planedata->airPlane;
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString user("root");
	sql::SQLString pwd("pass");
	std::unique_ptr<sql::Connection> conn(driver->connect("tcp://localhost:3306/simconnector", user, pwd));

	// Create a new PreparedStatement
	std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("insert into plane (title, kohlsmann, altitude, latitude, longitude, pitch, bank, heading, headingMagnetic, isOnGround, _datetime) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

	stmnt->setString(1, airplane->title);
	stmnt->setDouble(2, airplane->kohlsmann);
	stmnt->setDouble(3, airplane->altitude);
	stmnt->setDouble(4, airplane->latitude);
	stmnt->setDouble(5, airplane->longitude);
	stmnt->setDouble(6, airplane->pitch);
	stmnt->setDouble(7, airplane->bank);
	stmnt->setDouble(8, airplane->heading);
	stmnt->setDouble(9, airplane->headingMagnetic);
	stmnt->setBoolean(10, airplane->isOnGround);
	stmnt->setDateTime(11, datetime);

	stmnt->executeQuery();

	conn->close();
}

bool DLLTEMPLATE_API positionHasChanged(std::shared_ptr<PlaneData> ptr_planedata)
{
	auto planedata = ptr_planedata.get();
	auto datetime = planedata->ptr_datetime.get()->c_str();
	auto airplane = planedata->airPlane;
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString user("root");
	sql::SQLString pwd("pass");
	std::unique_ptr<sql::Connection> conn(driver->connect("tcp://localhost:3306/simconnector", user, pwd));

	std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("select latitude, longitude from plane where latitude=? and longitude=?"));

	const auto lenLat = static_cast<size_t>(_scprintf("%.4f", airplane->latitude));
	const auto lenLon = static_cast<size_t>(_scprintf("%.4f", airplane->longitude));

	std::string strLat(lenLat, '\0');
	std::string strLon(lenLon, '\0');
	
	_CSTD sprintf_s(&strLat[0], lenLat + 1, "%.4f", airplane->latitude);
	_CSTD sprintf_s(&strLon[0], lenLon + 1, "%.4f", airplane->longitude);

	stmnt->setString(1, &strLat[0]);
	stmnt->setString(2, &strLon[0]);
	
	sql::ResultSet* results = stmnt->executeQuery();
	
	size_t size = results->rowsCount();

	conn->close();

	return size == 0;
}
