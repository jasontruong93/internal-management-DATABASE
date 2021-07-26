#pragma once
#include <iostream>
#include "LoginManagement.h"
#include "TableManagement.h"
#include "Utility.h"

using namespace std;

class EmployeeManagement {
private:
	string username;
	string ssn;
	//string sql;
public:
	
	EmployeeManagement(string& username, string& ssn);
	void displayMainMenuDev(pqxx::nontransaction& N);
	void displayMainMenuManager(pqxx::nontransaction& N);
	void checkInfo(pqxx::nontransaction& N, bool isDev);
	void checkGrade(pqxx::nontransaction& N);
	void checkGradeAll(pqxx:: nontransaction& N);
	void allDeveloperInfo(pqxx::nontransaction& N);
	
	void search(pqxx::nontransaction& N);

	void DevMenu();
	void ManagerMenu();
	void databaseManagement(pqxx::nontransaction& N, bool isDev);
	void databaseLogs(pqxx::nontransaction& N);
};