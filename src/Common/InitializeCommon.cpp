/*
Copyright (C) 2008 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "InitializeCommon.h"
#include "DatabaseMigration.h"
#include "MySQLM.h"
#include "Config.h"
#include <iostream>
#include <string>

using std::string;

void Initializing::initializeMySQL() {
	std::cout << std::setw(outputWidth) << std::left << "Initializing MySQL... ";
	Config config("conf/mysql.lua");
	bool failed = false;
	// Character Database
	if (!(chardb.set_option(new mysqlpp::ReconnectOption(true)) && chardb.connect(config.getString("chardb_database").c_str(), config.getString("chardb_host").c_str(), config.getString("chardb_username").c_str(), config.getString("chardb_password").c_str(), config.getInt("chardb_port")))) {
		std::cout << "FAILED: " << chardb.error() << std::endl;
		exit(1);
	}
	// Data Database
	if (!(datadb.set_option(new mysqlpp::ReconnectOption(true)) && datadb.connect(config.getString("datadb_database").c_str(), config.getString("datadb_host").c_str(), config.getString("datadb_username").c_str(), config.getString("datadb_password").c_str(), config.getInt("datadb_port")))) {
		std::cout << "FAILED: " << datadb.error() << std::endl;
		exit(1);
	}
	std::cout << "DONE" << std::endl;
}

void Initializing::checkSchemaVersion(bool update) {
	DatabaseMigration dbMigration(update);
	
	bool succeed = dbMigration.checkVersion();

	if (!succeed && !update) {
		// Wrong version and we're not allowed to update, so lets quit.
		std::cout << "ERROR: Wrong version of database, please run Login Server to update." << std::endl;
		exit(4);
	}
	else if (!succeed) {
		// Failed, but we can update it.
		std::cout << std::setw(outputWidth) <<  "Updating database...";
		
		dbMigration.update();

		std::cout << "DONE" << std::endl;
	}
}

void Initializing::setUsersOffline(int onlineid) {
	mysqlpp::Query query = chardb.query();
	query << "UPDATE users INNER JOIN characters ON users.id = characters.userid SET users.online = 0, characters.online = 0 WHERE users.online = " << mysqlpp::quote << onlineid;
	query.exec();
}
