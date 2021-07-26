
--\cd 'C:\\Users\\madem\\OneDrive\\Documents\\cppfiles\\Database Design (Summer 2021)\\Final Project'
--Should already be in directory where CSV files are located, Data Tables:
--CREATE DATABASE ems;
--\c ems

CREATE TABLE Manager(SSN CHAR(9) Primary Key, Fname VARCHAR(100) NOT NULL, Lname VARCHAR(100) NOT NULL, DOB DATE CHECK (DOB > '1900-01-01'), Date_Joined DATE CHECK (Date_Joined > '1970-01-01'), Date_Departed DATE CHECK (Date_Departed > Date_joined), Position VARCHAR(100), Salary NUMERIC(11, 2) CHECK (Salary > 0), WorkOn CHAR(5), Username VARCHAR(20) NOT NULL, Password VARCHAR(15) NOT NULL);

CREATE TABLE Developers(SSN CHAR(9) Primary Key, Fname VARCHAR(100), Lname VARCHAR(100), DOB DATE CHECK (DOB > '1900-01-01'), Date_Joined DATE CHECK (Date_Joined > '1970-01-01'), Date_Departed DATE CHECK (Date_Departed > Date_joined), Position VARCHAR(100), Salary NUMERIC(10, 2) CHECK (Salary > 0), WorkOn CHAR(5), Manager CHAR(9) REFERENCES Manager ON UPDATE CASCADE NOT NULL, Username VARCHAR(20) NOT NULL, Password VARCHAR(15) NOT NULL);

CREATE TABLE Project(projID CHAR(5) Primary Key, Name VARCHAR(100) NOT NULL, Description TEXT, Budget REAL CHECK (Budget > 0), Supervisor CHAR(9) REFERENCES Manager ON UPDATE CASCADE NOT NULL, StartDate DATE CHECK (StartDate > '1970-01-01'), EndDate DATE CHECK (EndDate > StartDate), isProduct BOOL);

CREATE TABLE Performance(Grader CHAR(9) REFERENCES Manager ON UPDATE CASCADE, Gradee CHAR(9) REFERENCES Developers ON UPDATE CASCADE, Grade REAL NOT NULL CHECK (Grade > 0), Reasoning TEXT NOT NULL, Date DATE, CONSTRAINT Performance_pk PRIMARY KEY (Grader, Gradee, Date));

\copy Manager( SSN, fname, lname, DOB, Date_Joined, Date_Departed, Position,Salary, WorkOn, Username, Password) FROM 'C:\Users\taing\Desktop\cpp\Project3_Database\pj3\Database\manager.csv' WITH DELIMITER ',' CSV HEADER;

\copy Developers(SSN, Fname, LName, DOB, Date_Joined, Date_Departed, Position, Salary, WorkOn, Manager, Username, Password) FROM 'C:\Users\taing\Desktop\cpp\Project3_Database\pj3\Database\developers.csv' WITH DELIMITER ',' CSV HEADER;

\copy Project(projID, Name, Description, Budget, Supervisor, StartDate, EndDate, isProduct) FROM 'C:\Users\taing\Desktop\cpp\Project3_Database\pj3\Database\project.csv' WITH DELIMITER ',' CSV HEADER;

\copy Performance(Grader, Gradee, Grade, Reasoning, Date) FROM 'C:\Users\taing\Desktop\cpp\Project3_Database\pj3\Database\Performance.csv' WITH DELIMITER ',' CSV HEADER;

--Logging tables to keep track of changes made

CREATE TABLE Manager_Log(old_data TEXT, new_data TEXT, changed_on TIMESTAMP(6));

CREATE TABLE Developers_Log(old_data TEXT, new_data TEXT, changed_on TIMESTAMP(6));

CREATE TABLE Project_Log(old_data TEXT, new_data TEXT, changed_on TIMESTAMP(6));

CREATE TABLE Performance_Log(old_data TEXT, new_data TEXT, changed_on TIMESTAMP(6));

--DATABASE ROLES --\du to list to roles
CREATE ROLE Developer_User WITH NOSUPERUSER NOCREATEDB NOCREATEROLE NOINHERIT NOLOGIN;
CREATE ROLE Manager_User WITH NOSUPERUSER NOCREATEDB NOCREATEROLE NOINHERIT NOLOGIN;
CREATE ROLE Admin WITH NOSUPERUSER NOCREATEDB NOCREATEROLE NOINHERIT NOLOGIN; --WITH LOGIN PASSWORD '12344321'

GRANT ALL ON DATABASE ems TO Admin;

GRANT SELECT, UPDATE ON Developers TO Developer_User;
GRANT SELECT ON Performance TO Developer_User;
GRANT SELECT ON Project TO Developer_User;
GRANT SELECT ON Manager TO Developer_User;

GRANT INSERT ON Developers_Log TO Developer_User;
GRANT INSERT ON Performance_Log TO Developer_User;
GRANT INSERT ON Project_Log TO Developer_User;
GRANT INSERT ON Manager_Log TO Developer_User;

GRANT SELECT, UPDATE, INSERT ON Developers TO Manager_User;
GRANT SELECT, UPDATE, INSERT ON Performance TO Manager_User;
GRANT SELECT, UPDATE, INSERT ON Project TO Manager_User;
GRANT SELECT, UPDATE, INSERT ON Manager TO Manager_User;

GRANT INSERT ON Developers_Log TO Manager_User;
GRANT INSERT ON Performance_Log TO Manager_User;
GRANT INSERT ON Project_Log TO Manager_User;
GRANT INSERT ON Manager_Log TO Manager_User;

GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO Admin;

--SET ROLE developer_user;
--SET ROLE Manager_user;
--SET ROLE admin;

--Functions --\df to list functions
--manager
CREATE OR REPLACE FUNCTION log_manager_changes() RETURNS TRIGGER AS $$
DECLARE
    table_old_data TEXT;
    table_new_data TEXT;
BEGIN
    if (TG_OP = 'UPDATE') then
        table_old_data := ROW(OLD.*);
        table_new_data := ROW(NEW.*);
        INSERT INTO Manager_Log
        values (table_new_data, table_new_data, NOW());
        RETURN NEW;
    elsif (TG_OP = 'INSERT') then
        table_new_data := ROW(NEW.*);
        INSERT INTO Manager_Log(new_data, changed_on)
        values (table_new_data, NOW());
        RETURN NEW;
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

--developers
CREATE OR REPLACE FUNCTION log_developers_changes() RETURNS TRIGGER AS $$
DECLARE
    table_old_data TEXT;
    table_new_data TEXT;
BEGIN
    if (TG_OP = 'UPDATE') then
        table_old_data := ROW(OLD.*);
        table_new_data := ROW(NEW.*);
        INSERT INTO Developers_Log
        values (table_new_data, table_new_data, NOW());
        RETURN NEW;
    elsif (TG_OP = 'INSERT') then
        table_new_data := ROW(NEW.*);
        INSERT INTO Developers_Log(new_data, changed_on)
        values (table_new_data, NOW());
        RETURN NEW;
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

--project
CREATE OR REPLACE FUNCTION log_project_changes() RETURNS TRIGGER AS $$
DECLARE
    table_old_data TEXT;
    table_new_data TEXT;
BEGIN
    if (TG_OP = 'UPDATE') then
        table_old_data := ROW(OLD.*);
        table_new_data := ROW(NEW.*);
        INSERT INTO Project_Log
        values (table_new_data, table_new_data, NOW());
        RETURN NEW;
    elsif (TG_OP = 'INSERT') then
        table_new_data := ROW(NEW.*);
        INSERT INTO Project_Log(new_data, changed_on)
        values (table_new_data, NOW());
        RETURN NEW;
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

--performance
CREATE OR REPLACE FUNCTION log_performance_changes() RETURNS TRIGGER AS $$
DECLARE
    table_old_data TEXT;
    table_new_data TEXT;
BEGIN
    if (TG_OP = 'UPDATE') then
        table_old_data := ROW(OLD.*);
        table_new_data := ROW(NEW.*);
        INSERT INTO performance_Log
        values (table_new_data, table_new_data, NOW());
        RETURN NEW;
    elsif (TG_OP = 'INSERT') then
        table_new_data := ROW(NEW.*);
        INSERT INTO performance_Log(new_data, changed_on)
        values (table_new_data, NOW());
        RETURN NEW;
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

--developer and manger ssn match and projID match functions

CREATE OR REPLACE FUNCTION SSN_matches_dev() RETURNS TRIGGER AS $$
BEGIN
    if EXISTS (SELECT * FROM Developers WHERE NEW.SSN = SSN) then
        RAISE EXCEPTION 'Value already exists in Developer table. Type in a different value';
    END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION SSN_matches_man() RETURNS TRIGGER AS $$
BEGIN
    if EXISTS (SELECT * FROM Manager WHERE NEW.SSN = SSN) then
        RAISE EXCEPTION 'Value already exists in Manager table. Type in a different value';
    END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION ProjID_matches() RETURNS TRIGGER AS $$
BEGIN
    if NOT EXISTS (SELECT * FROM Project WHERE NEW.WorkON = ProjID) then
        RAISE EXCEPTION 'Value must exist in Project table. Type in a different value';
    END IF;

	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

--Triggers --SELECT * FROM PG_Trigger --shows the triggers in the database
CREATE TRIGGER manager_changes
BEFORE UPDATE OR INSERT ON Manager
FOR EACH ROW EXECUTE PROCEDURE log_manager_changes();

CREATE TRIGGER developers_changes
BEFORE UPDATE OR INSERT ON Developers
FOR EACH ROW EXECUTE PROCEDURE log_developers_changes();

CREATE TRIGGER project_changes
BEFORE UPDATE OR INSERT ON Project
FOR EACH ROW EXECUTE PROCEDURE log_project_changes();

CREATE TRIGGER performance_changes
BEFORE UPDATE OR INSERT ON Performance
FOR EACH ROW EXECUTE PROCEDURE log_performance_changes();

CREATE TRIGGER value_check_trigger
BEFORE UPDATE OR INSERT ON Manager
FOR EACH ROW EXECUTE PROCEDURE SSN_matches_dev();

CREATE TRIGGER value_check_trigger2
BEFORE UPDATE OR INSERT ON Developers
FOR EACH ROW EXECUTE PROCEDURE SSN_matches_man();

CREATE TRIGGER value_check_trigger3
BEFORE UPDATE OR INSERT ON Developers
FOR EACH ROW EXECUTE PROCEDURE ProjID_matches();

CREATE TRIGGER value_check_trigger3
BEFORE UPDATE OR INSERT ON Manager
FOR EACH ROW EXECUTE PROCEDURE ProjID_matches();