#include "TableManagement.h"

//function to insert row into database for developer
void Developers::insertRow(pqxx::nontransaction& N) {
	tableInfo("developers");
	DMInput(input, true);
	string sql_inner = queryi(0, input) + queryi(1, input) + queryi(2, input)
		+ queryi(3, input) + queryi(4, input) + queryq(5, input) + queryi(6, input)
		+ queryq(7, input) + queryi(8, input) + queryi(9, input) + queryi(10, input)
		+ (input[11] != "null" ? "'" + input[11] + "'" : "null ");
	
	string sql = "INSERT INTO Developers VALUES ( " + sql_inner + ")";
	queryIUD(N, sql);
	
	string choice = "";
	cout << "\nWould you like to do another insertion?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		insertRow(N);
}

//function to update row into database for developer
void Developers::updateRow(pqxx::nontransaction& N, string& ssn, string& username) {
	DMUpdate(N, ssn, username, true);
	
	string choice = "";
	cout << "\nWould you like to do another update?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		updateRow(N, ssn, username);
}

//function to insert row into database for manager
void Managers::insertRow(pqxx::nontransaction& N) {
	tableInfo("Manager");

	DMInput(input, false);
	string sql_inner = queryi(0, input) + queryi(1, input) + queryi(2, input)
		+ queryi(3, input) + queryi(4, input) + queryq(5, input) + queryi(6, input)
		+ queryq(7, input) + queryi(8, input) + queryi(9, input) 
		+ (input[10] != "null" ? "'" + input[10] + "'" : "null ");

	string sql = "INSERT INTO Manager VALUES ( " + sql_inner + ")";

	queryIUD(N, sql);

	string choice = "";
	cout << "\nWould you like to do another insertion?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		insertRow(N);
}
//INSERT INTO developers (ssn, fname, lname, dob, date_joined, position, salary, username, password) VALUES ('123456789', 'jason', 'truong', '1999-9-9', '2021-5-5', 'dev', 20000, 'jason', 'truong');
void Managers::updateRow(pqxx::nontransaction& N, string& ssn, string& username) {
	DMUpdate(N, ssn, username, false);

	string choice = "";
	cout << "\nWould you like to do another update?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		updateRow(N, ssn, username);
}

//function to insert rows into database for Performance
void Performance::insertRow(pqxx::nontransaction& N) {
	tableInfo("Performance");
	input.clear();
	string choice = "";

	cout << "What is the SSN of the Grader?: ";
	validate(choice, regex("[0-9]{9}"));		//check the format
	input.push_back(choice);

	cout << "What is the SSN of the Gradee?: ";
	validate(choice, regex("[0-9]{9}"));
	input.push_back(choice);

	cout << "What is the Grade?: ";
	validate(choice, regex("[0-9]{1,3}"));
	input.push_back(choice);

	cout << "What is the reasoning of the Grader?: ";
	validate(choice, regex("[a-zA-Z0-9 ]+"));
	input.push_back(choice);

	cout << "What is the Date of the Review? (yyyy-mm-dd): ";
	validate(choice, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}")); //check the format
	input.push_back(choice);

	string sql_inner = queryi(0, input) + queryi(1, input) + queryq(2, input)
		+ queryi(3, input) + "'" + input[4] + "' ";
	string sql = "INSERT INTO Performance VALUES ( " + sql_inner + ")";
	queryIUD(N, sql);

	cout << "\nWould you like to do another insertion?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		insertRow(N);
}

//function to insert rows in database for Project
void Project::insertRow(pqxx::nontransaction& N) {
	tableInfo("Project");

	input.clear();
	string choice = "";

	cout << "What is the projID?: ";
	validate(choice, regex("[0-9]{5}"));
	input.push_back(choice);

	cout << "What is the Name of the Project?: ";
	validate(choice, regex("[a-zA-Z0-9 ]+"));
	input.push_back(choice);

	cout << "What is the Description?: ";
	validate(choice, regex("[a-zA-Z0-9 ]+"));
	input.push_back(choice);

	cout << "What is the budget?: ";
	validate(choice, regex("[0-9]{1,8}"));
	input.push_back(choice);

	cout << "What is the Supervisor ID?: ";
	validate(choice, regex("[0-9]{9}"));
	input.push_back(choice);

	cout << "What is the Start Date? (yyyy-mm-dd): ";
	validate(choice, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"));
	input.push_back(choice);

	cout << "What is the End Date? (yyyy-mm-dd): ";
	validate(choice, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"));
	input.push_back(choice);

	cout << "Will this be a commercial product? (y/n): ";
	validate(choice, regex("[ny]"));///check if this actually works
	choice == "y" ? input.push_back("true") : input.push_back("false");

	string sql_inner = queryi(0, input) + queryi(1, input) + queryi(2, input)
		+ queryq(3, input) + queryi(4, input) + queryi(5, input) + queryi(6, input)
		+ input[7] + " ";

	string sql = "INSERT INTO Project VALUES ( " + sql_inner + ")";
	queryIUD(N, sql);

	cout << "\nWould you like to do another insertion?(y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		insertRow(N);
}

//function to update Rows in database for Project
void Project::updateRow(pqxx::nontransaction& N, string& projID) {
	cout << "\nWhat value would you like to update?" << endl
		<< "1. Name\n"
		<< "2. Dates\n"
		<< "3. Description\n"//see if you can implement ssn adn username
		<< "4. Budget\n"
		<< "5. Commercial Viability\n"
		<< "Your choice: ";

	string choice = "", choice2 = "", table = "Project", sql = "";
	validate(choice, regex("[1-5]"));


	switch (int i_choice = stoi(choice)) {
		case 6: {
			cout << "What would you like to change the Project ID to? (ID must exist in the Project table): ";
			validate(choice, regex("[0-9]{5}"));
			sql = "UPDATE " + table + " SET projID = '" + choice + "' WHERE projID = '" + projID + "'";
			queryIUD(N, sql);

			break;
		}
		case 1: {
			cout << "What would you like to change the name to? (Case Sensitive): ";
			validate(choice, regex("[a-zA-Z ]+"));//might need to include extra characters

			
			sql = "UPDATE " + table + " SET Name = '" + toLowerString(choice) + "' WHERE projID = '" + projID + "'";
			queryIUD(N, sql);
			break;
		}
		case 2: {
			cout << "What would you like to change the Start Date to?: ";
			validate(choice, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"));
			cout << "What would you like to change the End Date to?: ";
			validate(choice2, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"));

			
			sql = "UPDATE " + table + " SET StartDate = '" + choice + "', EndDate = '" + choice2 + "' WHERE projID = '" + projID + "'";
			queryIUD(N, sql);
			break;
		}
		case 3: {//might need database trigger to update tables that reference ssn, also,need to keep log
			cout << "What would you like to change the description to?: ";
			validate(choice, regex("[a-zA-Z0-9]+"));

			sql = "UPDATE " + table + " SET description = '" + choice + "' WHERE projID = '" + projID + "'";
			queryIUD(N, sql);
			break;
		}
		case 4: {
			cout << "What would you like to change the budget to?: ";//budget
			validate(choice, regex("[0-9]+"));

			queryIUD(N, "UPDATE " + table + " SET budget = " + choice + " WHERE projID = '" + projID + "'");
			break;
		}
		case 7: {
			cout << "What would you like to change the supervisor to?: ";//supervisor
			validate(choice, regex("[a-zA-Z]+"));

			sql = "UPDATE " + table + " SET supervisor = '" + choice + "' WHERE projID = '" + projID + "'";
			queryIUD(N, sql);
			break;
		}
		case 5: {
			cout << "What would you like to change the commercial viability to? (y/n): ";//comercial viability
			validate(choice, regex("[ny]"));

			sql = "UPDATE " + table + " SET isProduct = " + (choice == "y" ? "true" : "false") + " WHERE projID = '" + projID + "'";
			queryIUD(N, sql);
			break;
		}
		default: {
			cout << "Invalid selection!";
			break;
		}

	}
}