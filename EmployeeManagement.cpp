#include "EmployeeManagement.h"

//line 110 TableManagement
//test error in insert dev
LoginManagement l;

//constructor
EmployeeManagement::EmployeeManagement(string& username, string& ssn) {
	this->username = username;
	this->ssn = ssn;
}
//display main menu for developer
void EmployeeManagement::displayMainMenuDev(pqxx::nontransaction& N) {
	string choice;
	
	while (true) {
		int idKey = 0;
		DevMenu();
		cout << "Enter your choice: ";
		validate(choice, regex("[0-6]"));
		idKey = stoi(choice);
		switch (idKey)
		{
		//[1] .Check your information"		
		case 1: 
			system("CLS");
			checkInfo(N, true);
			break;
		//"[2]. Check your Grade for performance review"			
		case 2:
			system("CLS");
			checkGrade(N);
			break;
		//"[3]. Change Password"				
		case 3:
			system("CLS");
			LoginManagement::changePassword(N, false, this->username);
			break;
		//"[4]. Search Company Information"					
		case 4:
			system("CLS");
			search(N);
			break;
		//"[5]. Database Management"						
		case 5:
			system("CLS");
			databaseManagement(N, true);
			break;
		//"[0]. Log out"
		case 0: {
			system("CLS");
			l.logOut();
			return;
		}
		default:
			break;
		}
		
	}
}
//function to display the main menu of manager
void EmployeeManagement::displayMainMenuManager(pqxx::nontransaction& N) {
	string choice = "";
	int idKey = 0;
	while (true) {
		ManagerMenu();
		cout << "Enter your choice: ";
		validate(choice, regex("[0-7]"));
		idKey = stoi(choice);
		switch (idKey)
		{
		//[1]. Check your information		
		case 1:
			system("CLS");
			checkInfo(N, false);			
			break;
		//[2]. Check all developer Information in team
		
		case 2:
			system("CLS");
			allDeveloperInfo(N);			
			break;
		//[3]. Check Grade of developers in the team			
		case 3:
			system("CLS");
			checkGradeAll(N);
			break;
		//[4]. Change Password			
		case 4:
			system("CLS");
			LoginManagement::changePassword(N, true, this->username);			
			break;
		//[5]. Search Company Information			
		case 5:
			system("CLS");
			search(N);			
			break;
		//[6]. Database Management
		case 6:
			//add new developer
			system("CLS");
			databaseManagement(N, false);
			break;
		//[7]. Database Logs
		case 7:
			//add new developer
			system("CLS");
			databaseLogs(N);
			break;
		//[0]. Log out
		case 0:{
			system("CLS");
			l.logOut();
			return;
		}
		default:
			break;
		}
		
	}
}

void EmployeeManagement::DevMenu() {
	cout << "\n==========DEVELOPER MAIN MENU============\n[1]. Check your information\n"
		<< "[2]. Check your Grade for performance review\n"
		<< "[3]. Change Password\n"
		<< "[4]. Search Company Information\n"
		<< "[5]. Database Management\n"
		<< "[0]. Log out\n";
}

void EmployeeManagement::ManagerMenu() {
	cout << "\n==========MANAGER MAIN MENU============\n[1]. Check your information\n"
		<< "[2]. Check all developer Information in team\n"
		<< "[3]. Check Grade of developers in the team\n"
		<< "[4]. Change Password\n"
		<< "[5]. Search Company Information\n"
		<< "[6]. Database Management\n"
		<< "[7]. Database Logs\n"
		<< "[0]. Log out\n";
}

//function to check information of dev or manager
void EmployeeManagement::checkInfo(pqxx::nontransaction& N, bool isDev) {
	
	cout << "================ CHECK INFORMATION================" <<
		"\nOptions:\n 1. Personal Information\n 2. Project Information\n 3. Grade Information\nEnter your choice: ";
	
	bool flag = false;
	string sql = "", table = isDev ? "Developers" : "Manager";
	string query = !isDev ? "" : "M.SSN = D.Manager AND";
	string table2 = !isDev ? "" : ", Manager M";
	string manager = !isDev ? "" : ", M.fname || ' ' || M.lname";
	string choose= "";
	validate(choose, regex("[1-3]"));
	//personal information
	if (choose == "1") {
		sql = "SELECT D.SSN, D.Fname, D.Lname, D.DOB, D.Date_Joined, D.Date_Departed, D.Position, D.Salary" + manager + " FROM " + table + " D" + table2 + " WHERE " + query + " D.username = '" + username + "' AND D.ssn = '" + ssn + "'";
		pqxx::result R = queryS(N, flag, sql);
		for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
			infoOutput(c);
			if(isDev)
				cout << "Manager = " << c[8].as<string>() << endl;
		}
	}
	//project information
	else if (choose == "2") {
		sql = "SELECT P.Name, P.Description, P.Budget, M.fname || ' ' || M.lname, P.StartDate, P.EndDate, isProduct FROM " + table + " D, Project P, Manager M WHERE M.SSN = P.Supervisor AND D.WorkOn = P.ProjID AND D.username = '" + username + "' AND D.ssn = '" + ssn + "'";
		pqxx::result R = queryS(N, flag, sql);
		for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
			projectOutput(c);
		}
	}
	//grade information
    else if (choose == "3") {
		isDev ? checkGrade(N) : checkGradeAll(N);
	}

	cout << endl << "Do you want to check more information? (y/n): ";
	validate(choose, regex("[ny]"));
	if (choose == "y") {
		system("CLS");
		checkInfo(N, isDev);		
	}
		
}

//only for developers to check grade
void EmployeeManagement::checkGrade(pqxx::nontransaction& N){
	bool flag = false;
	string sql = "SELECT P.Grade, P.Reasoning, P.Date, D.Fname, D.Lname FROM Performance P, Developers D WHERE P.Gradee = '" + ssn + "' AND D.SSN = P.Gradee AND EXISTS (SELECT * FROM Developers WHERE username = '" + username + "')";
	pqxx::result R = queryS(N, flag, sql);
	for (pqxx::result::const_iterator c = R.begin(); c != R.end() && flag; c++){
		gradeOutput(c);
	}
	
}

//only for manager to check grade for all developers in teams
void EmployeeManagement::checkGradeAll(pqxx::nontransaction& N){
	int count = 1;
	bool flag = false;
	string sql = "SELECT P.Grade, P.Reasoning, P.Date, D.Fname, D.Lname FROM Performance P, Developers D WHERE D.SSN = P.Gradee AND P.Gradee IN (SELECT SSN FROM Developers WHERE Manager = '" + ssn + "')";
	pqxx::result R = queryS(N, flag, sql);
	
	for (pqxx::result::const_iterator c = R.begin(); c != R.end() && flag; c++) {
		if (!c[0].is_null()){
			cout << count << ". ";
			++count;
		}
		gradeOutput(c);
	}
}

//function to get all information of developers in team
void EmployeeManagement::allDeveloperInfo(pqxx::nontransaction& N) {
	bool flag = false;
	string sql = "SELECT D.SSN, D.Fname, D.Lname, D.DOB, D.Date_Joined, D.Date_Departed, D.Position, D.Salary, D.WorkOn, D.Manager, M.Fname, M.Lname FROM Developers D, Manager M WHERE M.SSN = D.Manager AND D.Manager = '" + ssn + "'";
	pqxx::result R = queryS(N, flag, sql);
	
	for (pqxx::result::const_iterator c = R.begin(); c != R.end() && flag; ++c) {
		infoOutput(c);
		cout << "WorkOn: " << c[8].as<string>() << endl;
		cout << "Manager: " << "***-**-" << c[9].as<string>().substr(5, 8) << endl;
		cout << "Manager Name: " << c[10].as<string>() + " " + c[11].as<string>() << endl;
	}
}

//function to search information of developers or Managers
void EmployeeManagement::search(pqxx::nontransaction& N) {
	string choice = "", cchoice = "", dec = ""; int i_choice = -1, ii_choice = -1; bool flag = false;

	cout << "\n===========SEARCH SECTION==========\n" 
		<< "What would you like to search for: " << endl
		<< "1. Search for Employee by Name" << endl
		<< "2. Search for Projects a Certain Employee Worked on" << endl << "Enter Your Choice: ";
	validate(choice, regex("[1-2]"));
	i_choice = stoi(choice);

	cout << "\nIs This Employee a Developer(1) or Manager(2)?: ";
	validate(cchoice, regex("[1-2]"));
	ii_choice = stoi(cchoice);
	string fname = "", lname = "", table = ii_choice == 1 ? "Developers" : "Manager";

	cout << "\nEnter the Employee's First Name (Case Sensitive): ";
	validate(fname, regex("[a-zA-Z]+"));
	cout << "Enter the Employee's Last Name (Case Sensitive): ";
	validate(lname, regex("[a-zA-Z]+"));

	switch (i_choice) {
		//search employee by Name
		case 1: {
			pqxx::result R = queryS(N, flag, "SELECT Fname, Lname, Date_Joined, Date_Departed, Position FROM " + table + " WHERE Fname = '" + fname + "' AND Lname = '" + lname + "'");
			pqxx::result::const_iterator c = R.begin(); 
			if (c[0].is_null() && !flag) {
				cout << "Person With Name " + fname + " " + lname + " is Not an Employee and Does Not Exist in Database" << endl;
				cout << "Want to try again? (y/n): ";
				validate(dec, regex("[ny]"));
				if (dec == "y")
					search(N);
				else
					return;
			}
			else {
				for (; c != R.end(); c++) {
					cout << "\nFull Name: " << c[0].as<string>() + " " + c[1].as<string>() << endl;
					cout << "Date Started: " << c[2].as<string>() + " - " + (c[3].is_null() ? " No end date specified" : c[3].as<string>()) << endl;
					cout << "Position: " << c[4].as<string>() << endl;
				}
				cout << "\nWant to search again? (y/n) ";
				validate(dec, regex("[ny]"));
				if (dec == "y")
					search(N);
				else
					return;
			}
			break;
		}
		//search the project by name
		case 2: {
			pqxx::result R = queryS(N, flag, "SELECT Name, Budget, Description FROM Project NATURAL JOIN " + table + " WHERE projID = WorkOn AND Fname = '" + fname + "' AND Lname = '" + lname + "'");
			pqxx::result::const_iterator c = R.begin();
			if (c[0].is_null() && !flag) {
				cout << "\nPerson With Name " + fname + " " + lname + " is Not an Employee and Does Not Exist in Database OR this employee does not have any current projects worked on" << endl;
				cout << "\nWant to try again? (y/n): ";
				validate(dec, regex("[ny]"));
				if (dec == "y")
					search(N);
				else
					return;
			}
			else {
				for (; c != R.end(); c++) {
					cout << "\nEmployee Name: " << fname + " " + lname << endl;
					cout << "Project Name: " << c[0].as<string>() << endl;
					cout << "Project Budget: " << c[1].as<string>() << endl;
					cout << "Project Description: " << c[2].as<string>() << endl << endl;
				}
				cout << "Want to search again? (y/n) ";
				validate(dec, regex("[ny]"));
				if (dec == "y")
					search(N);
				else
					return;
			}
			break;
		}
	}
}

//function to update or insert to the database 
void EmployeeManagement::databaseManagement(pqxx::nontransaction& N, bool isDev) {
	Developers d; Managers m; Performance pe; Project pr;
	string choice = "";
	system("CLS");
	//for developer
	if (isDev) {
		cout << "Welcome to the database management console." << endl
			<< "In here you will be able to perform advanced "
			<< "operations on your row of the Developer table. " << endl << endl
			<< "Would you like to update your information? (y/n): ";
		validate(choice, regex("[ny]"));
		//for update queries
		if(choice == "y")
			d.updateRow(N, this->ssn, this->username);
	}
	//for manager
	else {
		
		cout << "Welcome to the database management console." << endl
			<< "In here you will be able to perform advanced "
			<< "operations on the database. " << endl << endl
			<< "Which tables do you want to modify?\n 1. Developers\n 2. Manager\n 3. Performance\n 4. Project\n 0. Go Back to Main Menu\nYour choice: ";
		validate(choice, regex("[0-4]"));
		string choiice = "";
		switch (stoi(choice)) {
			//modify developers
			case 1:
				cout << "\nDevelopers Table: \n1. Update\n2. Insert\nYour choice: ";
				validate(choiice, regex("[1-2]"));
				if (stoi(choiice) == 1) {
				//check the ssn & username match in the record -------------------------
					string ssn_choice = "", username_choice ="";
					cout << "Enter the SSN of the developer you want to update: ";
					validate(ssn_choice, regex("[0-9]{9}"));	
					cout << "Enter the username of the developer you want to update: ";
					validate(username_choice, regex("[a-zA-Z0-9]+"));

					/*=======================================*/
					//check the matching of ssn and username in database
					try {
						pqxx::result R(N.exec("SELECT * FROM developers WHERE ssn = '" + ssn_choice + "' AND username = '" + username_choice+ "'"));
						if (R.empty()) {
							cout << "\nSSN and Username DO NOT match the Record! Go Back to the Menu!!!\n\n";
							break;
						}
					}
					catch(exception e){
						cout << "\nDO NOT match the Record! Go Back to the Menu!!!\n\n";
						break;
					}					
					
					d.updateRow(N, ssn_choice, username_choice);
				}
				else {
					d.insertRow(N);
				}
				break;
			//modify managers
			case 2:
				cout << "\nManager Table: \n1. Update\n2. Insert\nYour choice: ";
				validate(choiice, regex("[1-2]"));
				if (stoi(choiice) == 1) {
					string ssn_choice = "", username_choice ="";
					cout << "Enter the SSN of the manager you want to update: ";
					validate(ssn_choice, regex("[0-9]{9}"));	
					cout << "Enter the username of the manager you want to update: ";
					validate(username_choice, regex("[a-zA-Z0-9]+"));

					/*=======================================*/
					//check the matching of ssn and username in database
					try {
						pqxx::result R(N.exec("SELECT * FROM manager WHERE ssn = '" + ssn_choice + "' AND username = '" + username_choice + "'"));
						if (R.empty()) {
							cout << "\nSSN and Username DO NOT match the Record! Go Back to the Menu!!!\n\n";
							break;
						}
					}
					catch (exception e) {
						cout << "\nDO NOT match the Record! Go Back to the Menu!!!\n\n";
						break;
					}
					m.updateRow(N, ssn_choice, username_choice);
				}
				else {
					m.insertRow(N);
				}
				break;
			//modify performance
			case 3:
				cout << "You can only insert into the Performance table\n";
				cout << "Do you want to proceed?(y/n): ";
				validate(choiice, regex("[ny]"));
				if(choiice == "y")
					pe.insertRow(N);
				break;
			//modify Project
			case 4:
				cout << "\nProject Table: \n1. Update\n2. Insert\nYour choice: ";
				validate(choiice, regex("[1-2]"));
				if (stoi(choiice) == 1) {
					string id ="";
					cout << "What is the project ID that you want to update?\n";
					validate(id, regex("[0-9]{5}"));
					pr.updateRow(N, id);
				}
				else {
					pr.insertRow(N);
				}
				break;
			case 0:
				return;
		}

	}

	cout << "Do you want to modify more tables? (y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		databaseManagement(N, isDev);
		
}

//function to access database history
void EmployeeManagement::databaseLogs(pqxx::nontransaction& N) {
	string choice = "", table = ""; int i = 1; bool flag = false;
	string sql = "SET ROLE Admin";
	pqxx::result L(N.exec(sql));
	system("CLS");
	cout << "==========DATABASE LOGS SECTION==========\n"
		<<"\nWhat tables would you like to see the logs for? " << endl
		<< "1. Manager" << endl
		<< "2. Developers" << endl
		<< "3. Project" << endl
		<< "4. Performance" << endl << "Your choice: ";

	validate(choice, regex("[1-4]"));

	if (choice == "1") table = "Manager";
	else if (choice == "2") table = "Developers";
	else if (choice == "3") table = "Project";
	else table = "Performance";

	pqxx::result R = queryS(N, flag, "SELECT * FROM "+ table +"_log");

	if (R.empty() && !flag) {
		cout << "\nNo results in the table\n" << endl;
	}

	for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
		
		cout << i << ". Original Data: " << (c[0].is_null() ? "null" : c[0].as<string>()) << endl
			<< "New Data: " << c[1].as<string>() << endl
			<< "Timestamp: " << c[2].as<string>() << endl << endl;
		
		i++;
	}

	cout << "Do you want to check the logs of another table? (y/n): ";
	validate(choice, regex("[ny]"));
	if (choice == "y")
		databaseLogs(N);
	else
		system("CLS");
	sql = "SET ROLE Manager_User";
	pqxx::result Q(N.exec(sql));
}