/*
* Database Final Project - Summer 2021
* Team Members: Nghia Truong, Muhammed Ademola, Nhi Nguyen
*/
#include <chrono>
#include <thread>//for pausing the terminal - Muhammed

#include "LoginManagement.h"
#include "Utility.h"

using namespace std;
using namespace pqxx;

void LoginMenu();

int main() {

    string sql;

    try {
        connection C("host=localhost port=5432 dbname=ems user=postgres password =tainghia");//change based on your database and user 
        //connection C("host=localhost port=5432 dbname=ems user=postgres password=password");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return 1;
        }

        /* Create a non-transactional object. */
        nontransaction N(C);
        cout << "WELCOME TO [Software Development] COMPANY!!!\n"; 

        LoginManagement l;
        int choice = 0;

        //validate input
        string chooseStr;

        while (true) {
            LoginMenu();
            validate(chooseStr, regex("[0-2]"));
            choice = stoi(chooseStr);
            switch (choice) {
                case 1: {
                    //login
                    system("CLS");
                    cout << "\n ==========LOGIN SECTION==========\n"
                         << "Your Position: Developer (1) | Manager (2)\n"
                         << "You choice: ";
                    string chooseStr1;
                    validate(chooseStr1, regex("[1-2]"));
                    chooseStr1 == "1" ? l.login(N, false) : l.login(N, true);
                                  
                    break;
                }
                case 2:
                    system("CLS");
                    cout << "\n ===========FORGOT PASSWORD===========\n";
                    cout << l.forgotPassword(N);
                    break;                  

                case 0:
                    cout << "\nBye!!!\n";
                    C.close();
                    return 0;
            }
        }
        
    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

void LoginMenu() {
    cout << "\nMENU \n"
        << "[1]: Sign in.\n"
        << "[2]: Forgot Password.\n"
        << "[0]: Exit.\n"
        << "Your choice: ";
}