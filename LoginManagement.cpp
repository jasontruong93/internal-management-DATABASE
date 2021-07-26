#include "LoginManagement.h"

LoginManagement::LoginManagement() {
	setUserName("user");
	setPassword("123");
	setSSN("123456789");
	this->user_name_temp = "";
	this->password_temp = "";
	this->sql = "";
}
void LoginManagement::setUserName(string user_name_input) {
	this->user_name = user_name_input;
}
void LoginManagement::setPassword(string password_input) {
	this->password = password_input;
}
string LoginManagement::getUserName() const {
	return user_name;
}
string LoginManagement::getPassword() const {
	return password;
}
void LoginManagement::setSSN(string ssn_input) {
	this->ssn = ssn_input;
}
string LoginManagement::getSSN() const {
	return ssn;
}

void LoginManagement::login(pqxx::nontransaction& N, bool isManager) {
	switch (isManager){
		case false: {

			cout << "\n===========DEVELOPER SECTION==========\n";
			cout << "Enter Username: ";

			//validate input
			regex reg_input("[a-zA-Z0-9]+");
			validate(user_name_temp, reg_input);

			//input validation , protect against sql injection 
			sql = "SELECT username, password, ssn FROM Developers WHERE username = '" + user_name_temp + "'";
			pqxx::result R(N.exec(sql));

			for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {

				setUserName(c[0].as<string>());
				setPassword(c[1].as<string>());
				setSSN(c[2].as<string>());
			}

			if (user_name_temp == getUserName()) {
				cout << "Enter Password: ";
				validate(password_temp, reg_input);

				if (password_temp == getPassword()) {
					cout << "\nSuccessfully Logged In!";
					sql = "SET ROLE developer_user";
					pqxx::result L(N.exec(sql));
					system("CLS");
					
					EmployeeManagement e(user_name, ssn);
					e.displayMainMenuDev(N);
				}
				else {
					cout << "Incorrect password! Try to login again!\n\n";

					login(N, false);
				}
			}
			else {
				cout << "Incorrect username! Try to login again!\n\n";
				login(N, false);
			}
			break;
		}
		case true: {
			cout << "\n===========MANAGER SECTION==========\n";
			cout << "Enter Username: ";
			//validate input
			regex reg_input("[a-zA-Z0-9]+");
			validate(user_name_temp, reg_input);

			//input validation , protect against sql injection 
			sql = "SELECT username, password, ssn FROM Manager WHERE username = '" + user_name_temp + "'";
			pqxx::result R(N.exec(sql));

			//string username_sql, password_sql;
			for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
				setUserName(c[0].as<string>());
				setPassword(c[1].as<string>());
				setSSN(c[2].as<string>());
			}

			if (user_name_temp == getUserName()) {
				cout << "Enter Password: ";
				validate(password_temp, reg_input);
				if (password_temp == getPassword()) {
					cout << "\nSuccessfully Logged In!";
					sql = "SET ROLE Manager_user";
					pqxx::result L(N.exec(sql));
					system("CLS");
					
					EmployeeManagement e(user_name, ssn);
					e.displayMainMenuManager(N);
				}
				else {
					cout << "Incorrect password! Try to login again!\n";
					login(N, true);
				}
			}
			else {
				cout << "Incorrect Username! Try Again!!!\n";
				login(N, true);
			}
			break;
		}
	}
	//logOut();
}

void LoginManagement::changePassword(pqxx::nontransaction& N, bool isManager, string& username) {
	cout << "\n=========CHANGE PASSWORD==========\n";
	string password_temp1, password_temp2;
	cout << "Enter new password: ";
	regex reg_input("[a-zA-Z0-9]+");
	validate(password_temp1, reg_input);
	
	cout << "Enter new password again: ";
	validate(password_temp2, reg_input);

	string query = "";
	if (password_temp1 == password_temp2) {
		//input validation , protect against sql injection 
		query = "UPDATE Developers SET password = '" + password_temp2 + "' WHERE username = '" + username + "'";
		pqxx::result R(N.exec(query));

		cout << "Password Changed Successfully!\n";
		return;
	}
	else {
		cout << "New Password Does NOT Match!!! Try again\n";
		changePassword(N, isManager, username);
	}
}

string LoginManagement::forgotPassword(pqxx::nontransaction& N) {
	cout << "Do you know your SSN and Username?(y/n): ";
	string choice;
	validate(choice, regex("[ny]"));
	if (choice == "y") {
		cout << "Are you a Developer (1) or Manager (2)?: ";
		validate(choice, regex("[1-2]"));
		int ichoice = stoi(choice);
		if (ichoice == 1) {
			forgotPasswordVerbage(true, N);
		}
		else if (ichoice == 2) {
			forgotPasswordVerbage(false, N);
		}
	}
	else{
		return "Please contact the administrator for further help!\n";
	}	
	return "";
}

void LoginManagement::forgotPasswordVerbage(bool isdev, pqxx::nontransaction& N) {
	user_name_temp = "";
	string ssn_temp = "";
	cout << "Enter Username: ";
	//validate input
	validate(user_name_temp, regex("[a-zA-Z0-9]+"));

	cout << "Enter SSN: ";
	//validate input
	validate(ssn_temp, regex("[0-9]{9}"));

	//input validation , protect against sql injection 
	if(isdev)
		sql = "SELECT username, password, ssn FROM Developers WHERE username = '" + user_name_temp + "' AND  SSN = '" + ssn_temp + "'";
	else
		sql = "SELECT username, password, ssn FROM Managers WHERE username = '" + user_name_temp + "' AND  SSN = '" + ssn_temp + "'";
	pqxx::result R(N.exec(sql));
	pqxx::result::const_iterator c = R.begin();
	if (!c[0].is_null()) {
		LoginManagement::changePassword(N, false, user_name_temp);
		pqxx::result Q(N.exec(sql));
		for (pqxx::result::const_iterator d = Q.begin(); d != Q.end(); ++d) {
			setUserName(d[0].as<string>());
			setPassword(d[1].as<string>());
			setSSN(d[2].as<string>());
		}
		
	}
	else {
		string dec = "";
		cout << "No such UserName and SSN in database." << endl;
		cout << "Do you want to try again? (y/n): ";
		validate(dec, regex("[ny]"));
		forgotPassword(N);
	}
}

void LoginManagement::logOut() {
	setUserName("");
	setPassword("");
	setSSN("");
	this->sql = "";
	this->user_name_temp = "";
	this->password_temp = "";
	cout << "\nLog OUT Successfully!\n";
}