#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <limits>

using namespace std;

const int MAX_CALLS = 1000;
const string DATA_FILE = "calls.txt";
const string BACKUP_FILE = "backup_calls.txt";

struct DateTime {
    int day, month, year;
    int hour, minute;
};

struct Location {
    string street;
    string society;
    string city;
};

struct EmergencyCall {
    string name;
    string phone;
    Location location;
    string type;
    string priority;
    DateTime timestamp;
};

// ==================== CONSOLE COLORS ====================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_WHITE 15
#define COLOR_CYAN 11

// ==================== FUNCTION PROTOTYPES ====================
bool adminLogin();
int mainMenu();
void addCall();
void viewCalls();
void searchCall();
void editCall();
void deleteCall();
void showStatistics();
void sortByName();
void backupData();
void restoreData();
int loadCalls(EmergencyCall calls[]);
void saveCalls(EmergencyCall calls[], int n);
bool validatePhone(const string &phone);
bool validateName(const string &name);
DateTime getCurrentDateTime();
void displayHeader();
void displayCallWithColor(EmergencyCall c);
void clearScreen();
void printTitle(const string &title);

// ==================== MAIN ====================
int main() {
    clearScreen();
    printTitle("EMERGENCY CALL MANAGEMENT SYSTEM");

    if(!adminLogin()) {
        setColor(COLOR_RED);
        cout << "\nAccess denied. Program exiting.\n";
        setColor(COLOR_WHITE);
        return 0;
    }

    int choice;
    do {
        choice = mainMenu();
        switch(choice) {
            case 1: addCall(); break;
            case 2: viewCalls(); break;
            case 3: searchCall(); break;
            case 4: editCall(); break;
            case 5: deleteCall(); break;
            case 6: showStatistics(); break;
            case 7: sortByName(); break;
            case 8: backupData(); break;
            case 9: restoreData(); break;
            case 10: setColor(COLOR_CYAN);
                     cout << "\nExiting system. Stay safe!\n";
                     setColor(COLOR_WHITE);
                     break;
            default: setColor(COLOR_RED);
                     cout << "\nInvalid option!\n";
                     setColor(COLOR_WHITE);
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
        printTitle("EMERGENCY CALL MANAGEMENT SYSTEM");
    } while(choice != 10);

    return 0;
}

// ==================== HELPERS ====================
void clearScreen() { system("cls"); }

void printTitle(const string &title) {
    setColor(COLOR_CYAN);
    cout << "============================================================\n";
    cout << "                   " << title << "\n";
    cout << "============================================================\n\n";
    setColor(COLOR_WHITE);
}

// ==================== ADMIN LOGIN ====================
bool adminLogin() {
    string user, pass;
    int attempts = 3;

    while(attempts--) {
        setColor(COLOR_YELLOW);
        cout << "Admin Login\n";
        setColor(COLOR_WHITE);
        cout << "Username: ";
        getline(cin, user);
        cout << "Password: ";
        getline(cin, pass);

        if(user == "admin" && pass == "1234") {
            setColor(COLOR_GREEN);
            cout << "\nLogin successful!\n";
            setColor(COLOR_WHITE);
            return true;
        } else {
            setColor(COLOR_RED);
            cout << "Wrong credentials. Attempts left: " << attempts << "\n\n";
            setColor(COLOR_WHITE);
        }
    }
    return false;
}

// ==================== MENU ====================
int mainMenu() {
    int choice;
    setColor(COLOR_CYAN);
    cout << "\n================ MAIN MENU =================\n";
    setColor(COLOR_WHITE);
    cout << "1. Add Emergency Call\n";
    cout << "2. View All Calls\n";
    cout << "3. Search Call by Phone\n";
    cout << "4. Edit Call\n";
    cout << "5. Delete Call\n";
    cout << "6. View Statistics\n";
    cout << "7. Sort Calls by Name\n";
    cout << "8. Backup Data\n";
    cout << "9. Restore Data\n";
    cout << "10. Exit\n";
    setColor(COLOR_CYAN);
    cout << "============================================\n";
    setColor(COLOR_WHITE);
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    return choice;
}

// ==================== VALIDATION ====================
bool validatePhone(const string &phone) {
    if(phone.length() != 10) return false;
    for(int i=0; i<10; i++)
        if(phone[i] < '0' || phone[i] > '9') return false;
    return true;
}

bool validateName(const string &name) {
    return name.length() <= 18 && name.length() > 0;
}

// ==================== DATE & TIME ====================
DateTime getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    DateTime dt;
    dt.day = ltm->tm_mday;
    dt.month = ltm->tm_mon + 1;
    dt.year = ltm->tm_year + 1900;
    dt.hour = ltm->tm_hour;
    dt.minute = ltm->tm_min;

    return dt;
}

// ==================== FILE HANDLING ====================
int loadCalls(EmergencyCall calls[]) {
    ifstream file(DATA_FILE.c_str());
    if(!file) return 0;

    int count = 0;
    while(file >> calls[count].name
               >> calls[count].phone
               >> calls[count].location.street
               >> calls[count].location.society
               >> calls[count].location.city
               >> calls[count].type
               >> calls[count].priority
               >> calls[count].timestamp.day
               >> calls[count].timestamp.month
               >> calls[count].timestamp.year
               >> calls[count].timestamp.hour
               >> calls[count].timestamp.minute) {
        count++;
        if(count >= MAX_CALLS) break;
    }
    file.close();
    return count;
}

void saveCalls(EmergencyCall calls[], int n) {
    ofstream file(DATA_FILE.c_str());
    for(int i=0; i<n; i++) {
        file << calls[i].name << " "
             << calls[i].phone << " "
             << calls[i].location.street << " "
             << calls[i].location.society << " "
             << calls[i].location.city << " "
             << calls[i].type << " "
             << calls[i].priority << " "
             << calls[i].timestamp.day << " "
             << calls[i].timestamp.month << " "
             << calls[i].timestamp.year << " "
             << calls[i].timestamp.hour << " "
             << calls[i].timestamp.minute << endl;
    }
    file.close();
}

// ==================== ADD CALL ====================
void addCall() {
    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    EmergencyCall c;

    setColor(COLOR_CYAN);
    cout << "\n----- Add Emergency Call -----\n";
    setColor(COLOR_WHITE);

    do { cout << "Name: "; getline(cin, c.name); } while(!validateName(c.name));
    do { cout << "Phone (10 digits): "; getline(cin, c.phone); } while(!validatePhone(c.phone));

    cout << "Street: "; getline(cin, c.location.street);
    cout << "Society: "; getline(cin, c.location.society);
    cout << "City: "; getline(cin, c.location.city);

    cout << "Type (Fire/Medical/Police): "; getline(cin, c.type);
    cout << "Priority (Low/Medium/High/Critical): "; getline(cin, c.priority);

    c.timestamp = getCurrentDateTime();
    calls[n++] = c;
    saveCalls(calls, n);

    setColor(COLOR_GREEN);
    cout << "\nCall added successfully!\n";
    setColor(COLOR_WHITE);
}

// ==================== DISPLAY ====================
void displayCallWithColor(EmergencyCall c) {
    if(c.priority == "Critical") setColor(COLOR_RED);
    else if(c.priority == "High") setColor(COLOR_YELLOW);
    else if(c.priority == "Medium") setColor(COLOR_WHITE);
    else if(c.priority == "Low") setColor(COLOR_GREEN);
    else setColor(COLOR_WHITE);

    cout << left << setw(15) << c.name
         << setw(12) << c.phone
         << setw(12) << c.location.street
         << setw(12) << c.location.society
         << setw(10) << c.location.city
         << setw(10) << c.type
         << setw(10) << c.priority
         << c.timestamp.day << "/" << c.timestamp.month << "/" << c.timestamp.year
         << "   "
         << c.timestamp.hour << ":" << c.timestamp.minute << "\n";

    setColor(COLOR_WHITE);
}

void displayHeader() {
    setColor(COLOR_CYAN);
    cout << left << setw(15) << "Name"
         << setw(12) << "Phone"
         << setw(12) << "Street"
         << setw(12) << "Society"
         << setw(10) << "City"
         << setw(10) << "Type"
         << setw(10) << "Priority"
         << setw(15) << "Date"
         << setw(10) << "Time" << "\n";
    cout << "-------------------------------------------------------------------\n";
    setColor(COLOR_WHITE);
}

// ==================== VIEW / SEARCH / EDIT / DELETE ====================
void viewCalls() {
    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    if(n == 0) { setColor(COLOR_RED); cout << "\nNo records found.\n"; setColor(COLOR_WHITE); return; }

    setColor(COLOR_CYAN); cout << "\n----- All Emergency Calls -----\n"; setColor(COLOR_WHITE);
    displayHeader();
    for(int i=0; i<n; i++) displayCallWithColor(calls[i]);
}

void searchCall() {
    string phone; cout << "Enter phone to search: "; getline(cin, phone);

    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    for(int i=0;i<n;i++){
        if(calls[i].phone==phone){
            setColor(COLOR_CYAN); cout<<"\n----- Call Found -----\n"; setColor(COLOR_WHITE);
            displayHeader();
            displayCallWithColor(calls[i]);
            return;
        }
    }
    setColor(COLOR_RED); cout<<"\nCall not found.\n"; setColor(COLOR_WHITE);
}

// ==================== FIXED EDIT ====================
void editCall() {
    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    string phone;
    cout << "Enter phone of the call to edit: ";
    getline(cin, phone);

    for(int i = 0; i < n; i++) {
        if(calls[i].phone == phone) {

            setColor(COLOR_CYAN); cout << "\n----- Edit Call Details -----\n"; setColor(COLOR_WHITE);
            string input;

            cout << "New Name (" << calls[i].name << "): "; getline(cin,input); if(!input.empty()) calls[i].name = input;

            do {
                cout << "New Phone (" << calls[i].phone << "): "; getline(cin,input);
                if(input.empty()) break;
                if(validatePhone(input)) { calls[i].phone = input; break; }
                setColor(COLOR_RED); cout << "Invalid phone! Must be 10 digits.\n"; setColor(COLOR_WHITE);
            } while(true);

            cout << "New Street (" << calls[i].location.street << "): "; getline(cin,input); if(!input.empty()) calls[i].location.street=input;
            cout << "New Society (" << calls[i].location.society << "): "; getline(cin,input); if(!input.empty()) calls[i].location.society=input;
            cout << "New City (" << calls[i].location.city << "): "; getline(cin,input); if(!input.empty()) calls[i].location.city=input;
            cout << "New Type (" << calls[i].type << "): "; getline(cin,input); if(!input.empty()) calls[i].type=input;

            cout << "New Priority (" << calls[i].priority << ") [Low/Medium/High/Critical]: ";
            getline(cin,input);
            if(!input.empty() && (input=="Low"||input=="Medium"||input=="High"||input=="Critical")) calls[i].priority=input;

            calls[i].timestamp = getCurrentDateTime();

            saveCalls(calls,n);
            setColor(COLOR_GREEN); cout<<"\nCall updated successfully! Timestamp updated.\n"; setColor(COLOR_WHITE);
            return;
        }
    }

    setColor(COLOR_RED); cout<<"\nCall not found.\n"; setColor(COLOR_WHITE);
}

void deleteCall() {
    string phone; cout << "Enter phone to delete: "; getline(cin, phone);

    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    for(int i=0;i<n;i++){
        if(calls[i].phone==phone){
            for(int j=i;j<n-1;j++) calls[j]=calls[j+1];
            saveCalls(calls,--n);
            setColor(COLOR_GREEN); cout<<"\nCall deleted.\n"; setColor(COLOR_WHITE);
            return;
        }
    }
    setColor(COLOR_RED); cout<<"\nCall not found.\n"; setColor(COLOR_WHITE);
}

// ==================== STATISTICS & SORT ====================
void showStatistics() {
    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);
    setColor(COLOR_CYAN); cout<<"\n----- Statistics -----\n"; setColor(COLOR_WHITE);
    cout << "Total Calls: " << n << "\n";
}

void sortByName() {
    EmergencyCall calls[MAX_CALLS];
    int n = loadCalls(calls);

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(calls[i].name>calls[j].name) swap(calls[i],calls[j]);

    saveCalls(calls,n);
    setColor(COLOR_GREEN); cout<<"\nCalls sorted by name.\n"; setColor(COLOR_WHITE);
}

// ==================== BACKUP & RESTORE ====================
void backupData() { ifstream in(DATA_FILE.c_str()); ofstream out(BACKUP_FILE.c_str()); out << in.rdbuf(); setColor(COLOR_GREEN); cout<<"\nBackup created successfully.\n"; setColor(COLOR_WHITE);}
void restoreData() { ifstream in(BACKUP_FILE.c_str()); ofstream out(DATA_FILE.c_str()); out << in.rdbuf(); setColor(COLOR_GREEN); cout<<"\nData restored successfully.\n"; setColor(COLOR_WHITE);}

