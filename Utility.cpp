#include "Utility.h"

//function to validate input
void validate(string& s, const regex& reg)
{
    do {
        cin.clear();
        getline(cin, s);
        trim(s);
        if (!regex_match(s, reg)) {
            cout << "Invalid! Enter again: ";
        }
        else {
            break;
        }
    } while (true);
}

void trim(string& s)
{
    regex l("^\\s+");
    regex r("\\s+$");
    regex m("\\s+");
    s = std::regex_replace(s, r, "");
    s = std::regex_replace(s, l, "");
    s = std::regex_replace(s, m, " ");
}

//output the grade section
void gradeOutput(pqxx::result::const_iterator& c) {
    if (!c[0].is_null()) {
        cout << "\nName: " << c[3].as<string>() + " " + c[4].as<string>() << endl
            << "Grade: " << c[0].as<string>() << endl
            << "Reasoning: " << c[1].as<string>() << endl
            << "Date: " << c[2].as<string>() << endl;
    }
}

//output the information section
void infoOutput(pqxx::result::const_iterator& c) {
    cout << "\nName: " << c[1].as<string>() << " " << c[2].as<string>() << endl
         << "Last 4 SSN: ***-**-" << c[0].as<string>().substr(5, 8) << endl //last 4 of SSN
         << "DOB: " << c[3].as<string>() << endl
         << "Date Joined: " << c[4].as<string>() << endl;
    if (!c[5].is_null())
         cout << "Date Departed: " << c[5].as<string>() << endl;
    cout << "Position: " << c[6].as<string>() << endl
        << "Salary: " << c[7].as<double>() << endl;
        // << "WorkOn =" << c[8].as<string>() << endl;
}

//output the project section
void projectOutput(pqxx::result::const_iterator& c) {
    cout << "\nProject Name: " << c[0].as<string>() << endl;
    cout << "Project Description: " << c[1].as<string>() << endl;
    cout << "Project Budget: " << c[2].as<string>() << endl;
    cout << "Project Supervisor: " << c[3].as<string>() << endl;
    cout << "Project Date Range: " << c[4].as<string>() + " - " + (c[5].is_null() ? "No end date specified" : c[5].as<string>()) << endl;
    cout << "Commercial Product: " << boolalpha << c[6].as<bool>() << endl;
}

//validate and ask for user input 
void DMInput(vector<string>& input, bool isDev) {
    vector<string> questions = { "What is their SSN? (Cannot be null): ", //0
                                "What is their First Name? (Cannot be null): ", //1
                                "What is their Last Name? (Cannot be null): ", //2
                                "What is their Date of Birth? (yyyy - mm - dd): ", //3
                                "When did they join the company?: ", //4
                                "When did they depart?: ", //question not asked //5
                                "What is their Position?: ", //6
                                "What is their Salary?: ", //7
                                "What project are they working on (projID must exist within project table)?: ",//8
                                "Who is their manager? (Cannot be null): ",//9
                                "What is their username? (Cannot be null): ", //10
                                "What is their password? (Cannot be null): " };//11
    //INSERT INTO Developers (ssn, fname, lname, DOB, Date_Joined, Date_Departed, Position, Salary, WorkOn, Manager, Username, Password)  
    //   VALUES (123456789, 'Paul', 'Jason', 1999-5-10 , 2021-7-20, , Developer, 50000, , 111111111, abc, xyq);
    input.clear();
    for (unsigned int i = 0; i < questions.size(); i++) {
        string choice = "";
        
        if (i == 0) {//ssn
            cout << questions[i];
            validate(choice, regex("[0-9]{9}"));
            input.push_back(choice);
        }
        else if (i == 1 || i == 2) {
            cout << questions[i];
            validate(choice, regex("[a-zA-Z]+"));
            input.push_back(choice);
        }
        else if (i == 3 || i == 4) {//dates
            cout << questions[i];
            validate(choice, regex("[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}"));
            input.push_back(choice);
        }
        else if(i == 5) {//date_departed
            input.push_back("null");
        }
        else if (i == 6) { //position
            cout << questions[i];
            validate(choice, regex("[a-zA-Z0-9 ]+"));
            input.push_back(choice);
        }
        else if (i == 7) {
            cout << questions[i];
            validate(choice, regex("[0-9]+"));
            input.push_back(choice);
        }
        else if (isDev && i == 9) {//manager question
            cout << questions[i];
            validate(choice, regex("[a-zA-Z0-9]+"));
            input.push_back(choice);
        }
        else if (!isDev && i == 9) {
            continue;
        }
        else {
            cout << questions[i];
            validate(choice, regex("[a-zA-Z0-9]+"));
            input.push_back(choice);
        }
            
    }
    
}

void tableInfo(string table) {
    cout << "Type in the information you would like to insert in the " + table + " table. " << endl
		 << "Type null if you do not want to insert a value for a certain column\n";
}

string queryi(int i, vector<string>& input) { 
    return input[i] != "null" ? "'" + input[i] + "', " : "null, "; 
}
string queryq(int i, vector<string>& input) { 
    return input[i] != "null" ? input[i] + ", " : "null, "; 
}
string toLowerString(string& input) {
    string temp = "";
    for (unsigned int i = 0; i < input.size() ; i++)
        temp += static_cast<char>(tolower(input[i]));
    return temp;
}

//function use for update query
void DMUpdate(pqxx::nontransaction& N, string& ssn, string& username, bool isDev) {
    cout << "\nWhat value would you like to update?" << endl
        << "1. ID of Project Being Worked On\n"
        << "2. Name\n"
        << "3. DOB\n"
        << "4. SSN\n"//see if you can implement ssn adn username
        << "5. Username\n"
        << "0. Go Back\n"
        << "Your choice: ";

    string choice = "", choice2 = "", sql = "", table = isDev ? "Developers" : "Manager";
    validate(choice, regex("[0-5]"));
    
    //have to validate the ssn and username before go to that switch
    switch (int i_choice = stoi(choice)) {
        case 1: {
            cout << "What would you like to change the Project ID to? (ID must exist in the Project table): ";
            validate(choice, regex("[0-9]{5}"));
            
            sql = "UPDATE " + table + " SET WorkON = '" + choice + "' WHERE SSN = '" + ssn + "' AND username = '" + username + "'";
            queryIUD(N, sql);
            break;
        }
        case 2: {
            cout << "What would you like to change the first name to? (Case Sensitive): ";
            validate(choice, regex("[a-zA-Z]+"));
            cout << "What would you like to change the last name to? (Case Sensitive): ";
            validate(choice2, regex("[a-zA-Z]+"));
            
            sql = "UPDATE " + table + " SET fname = '" + toLowerString(choice) + "', lname = '" + toLowerString(choice2) + "' WHERE SSN = '" + ssn + "' AND username = '" + username + "'";
            queryIUD(N, sql);
            break;
        }
        case 3: {
            cout << "What would you like to change the DOB to?: ";
            validate(choice, regex("[0-9]{0,4}-[0-9]{0,2}-[0-9]{0,2}"));
            
            sql = "UPDATE " + table + " SET DOB = '" + choice + "' WHERE SSN = '" + ssn + "' AND username = '" + username + "'";
            queryIUD(N, sql);
            
            break;
        }
        case 4: {//might need database trigger to update tables that reference ssn, also,need to keep log
            cout << "What would you like to change the SSN to?: ";
            validate(choice, regex("[0-9]{9}"));
            
            sql = "UPDATE " + table + " SET SSN = '" + choice + "' WHERE SSN = '" + ssn + "' AND username = '" + username + "'";
            if (queryIUD(N, sql))
                ssn = choice;
           
            break;
        }
        case 5: {
            cout << "What would you like to change the username to?: ";
            validate(choice, regex("[a-zA-Z0-9]+"));
            
            sql = "UPDATE " + table + " SET Username = '" + choice + "' WHERE SSN = '" + ssn + "' AND username = '" + username + "'";
            if (queryIUD(N, sql))
                username = choice;
            break;
        }
        case 0: {
            return;
        }
        default: {
            cout << "Invalid selection!";
            break;
        }

    }
}

//when insert, update, or delete queries
bool queryIUD(pqxx::nontransaction& N, string query) { 
    bool flag;
    try {
        pqxx::result R(N.exec(query));

        if (R.affected_rows() > 0) {
            cout << "\nQuery Successful!\n";
            flag = true;
        }
        else {
            cout << "\nQuery UNsuccessful or No Row Changed!\n";
            flag = false;
        }
	}
	catch(exception e){
		cout << "\nAn error has occured: " << e.what() << "Please go back to the main menu and try again or contact your manager\n";
		return false;
	}
    return flag;
}

//when select queries
pqxx::result queryS(pqxx::nontransaction& N, bool& flag, string query) { //select queries
    pqxx::result temp;
    try {
		pqxx::result R(N.exec(query));
        if (!R.empty()) {
		    cout << "\nQuery Successful!\n";
            flag = true;
	    }else{
            cout << "\nQuery UNsuccessful or Table Empty!\n";
            flag = false;
        }
        temp = R;
	}
	catch(exception e){
		cout << "\nAn error has occured: " << e.what() << "Please go back to the main menu and try again or contact your manager\n";
        flag = false;
	}
    
    return temp;
}