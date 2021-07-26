#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string>
#include <pqxx/pqxx>

using namespace std;

void validate(std::string& s, const std::regex& reg);
void trim(std::string& s);
void gradeOutput(pqxx::result::const_iterator& c);
void infoOutput(pqxx::result::const_iterator& c);
void projectOutput(pqxx::result::const_iterator& c);

void DMInput(vector<string>& input, bool isDev);
void tableInfo(string table);
string queryi(int i, vector<string>& input);
string queryq(int i, vector<string>& input);
string toLowerString(string& input);
void DMUpdate(pqxx::nontransaction& N, string& ssn, string& username, bool isDev);

bool queryIUD(pqxx::nontransaction& N, string query);
pqxx::result queryS(pqxx::nontransaction& N, bool& flag, string query);