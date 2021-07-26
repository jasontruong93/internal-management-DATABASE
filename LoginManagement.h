#pragma once

#include <iostream>
#include <string>
#include <pqxx/pqxx> 
#include "Utility.h"
#include "EmployeeManagement.h"

using namespace std;

class LoginManagement {
private:
	string user_name;
	string password;
	string ssn;
	string user_name_temp;
	string password_temp;
	string sql;
public:
	//constructor
	LoginManagement();
	
	void setUserName(string user_name);
	void setPassword(string password);
	void setSSN(string user_name);

	string getUserName() const;
	string getPassword() const;
	string getSSN() const;
	void static changePassword(pqxx::nontransaction& N, bool isManager, string& username);
	string forgotPassword(pqxx::nontransaction& N);
	void forgotPasswordVerbage(bool isdev, pqxx::nontransaction& N);
	void login(pqxx::nontransaction& N, bool isManager);

	void logOut();

};