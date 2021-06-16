#include "MariaDB.h"
#include <json/json.h>
#include <memory>
#include <mariadb/conncpp.hpp>

struct Credentials
{
	std::string user;
	std::string password;
};

static std::unique_ptr<Credentials> readConfig();
static std::filesystem::path parentPath("");
static void load_ParentPath();
static std::unique_ptr<sql::Connection> connect_database(std::shared_ptr<PlaneData> ptr_planedata);

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
	auto airplane = planedata->airPlane;
	auto datetime = planedata->ptr_datetime.get()->c_str();

	std::unique_ptr<sql::Connection> conn = connect_database(ptr_planedata);

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
	auto airplane = planedata->airPlane;

	std::unique_ptr<sql::Connection> conn = connect_database(ptr_planedata);

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

static std::unique_ptr<sql::Connection> connect_database(std::shared_ptr<PlaneData> ptr_planedata)
{
	const auto config = readConfig();

	auto planedata = ptr_planedata.get();
	auto datetime = planedata->ptr_datetime.get()->c_str();
	auto airplane = planedata->airPlane;
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString user(config->user.c_str());
	sql::SQLString pwd(config->password.c_str());
	std::unique_ptr<sql::Connection> conn(driver->connect("tcp://localhost:3306/simconnector", user, pwd));

	return conn;
}

std::filesystem::path DLLTEMPLATE_API getParentPath()
{
	if (parentPath.string().length() == 0)
	{
		load_ParentPath();
	}

	return parentPath;
}

std::filesystem::path DLLTEMPLATE_API getConfigurationPath()
{
	auto path = getParentPath();
	path.concat("\\config.json");
	return path;
}

static void load_ParentPath()
{
	std::filesystem::path path = std::filesystem::current_path();
	std::regex pattern("^([a-zA-Z:])(.*)(SimConnector)(.*)");
	std::cmatch results;
	std::string rootPath = path.string();
	std::filesystem::path _parentPath("");
	std::string rootFolder = "SimConnector";

	bool isMatched = std::regex_match(rootPath.c_str(), results, pattern);

	if (isMatched)
	{
		for (unsigned i = 1; i < results.size(); ++i)
		{
			_parentPath.concat(results[i].str());

			if (results[i].compare(rootFolder) == 0)
			{
				break;
			}
		}
	}

	parentPath = _parentPath;
}

static std::unique_ptr<Credentials> readConfig()
{
	Json::Value root;
	std::ifstream ifs;
	std::string line;

	std::unique_ptr<Credentials> ptr_credentials = std::make_unique<Credentials>();

	const auto path = getConfigurationPath();

	ifs.open(path);
	Json::CharReaderBuilder builder;

	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		throw errs;
	}

	const std::string user(root["database"]["user"].asString());
	const std::string password(root["database"]["password"].asString());

	ptr_credentials->user = user;
	ptr_credentials->password = password;

	ifs.close();

	return ptr_credentials;
}
