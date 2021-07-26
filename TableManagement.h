#pragma once

#include "Utility.h"

using namespace std;

class Generic
{
protected:
	string select;
	string from;
	string where;
	string update;
	string insert;
	vector<string> input;

public:
	Generic() { select = "SELECT "; from = "FROM "; where = "WHERE "; insert = "INSERT INTO "; update = "UPDATE "; }
};

class Developers : public Generic
{
private:
	

public:
	void insertRow(pqxx::nontransaction& N);
	void updateRow(pqxx::nontransaction& N,  string& ssn, string& username);
};

class Managers : public Generic
{
private:
	

public:
	void insertRow(pqxx::nontransaction& N);
	void updateRow(pqxx::nontransaction& N, string& ssn, string& username);

};

class Performance : public Generic
{
private:
	

public:
	void insertRow(pqxx::nontransaction& N);
};

class Project : public Generic
{
private:
	

public:
	void insertRow(pqxx::nontransaction& N);
	void updateRow(pqxx::nontransaction& N, string& projID);

};