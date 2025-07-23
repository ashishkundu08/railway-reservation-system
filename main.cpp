#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <limits>

using namespace std;

// User structure
struct user {
    string name, psw;
};

// Contact class
class contact {
public:
    string name, address, number, aadhaar;

    contact() {}

    bool operator == (const contact& b) {
        return aadhaar == b.aadhaar;
    }

    friend istream& operator >> (istream& in, contact& a) {
        cout << "Name: "; getline(in, a.name);
        cout << "Aadhaar Number: "; getline(in, a.aadhaar);
        cout << "Phone Number: "; getline(in, a.number);
        cout << "Address: "; getline(in, a.address);
        return in;
    }

    friend ostream& operator << (ostream& out, const contact& a) {
        out << "Name: " << a.name << endl;
        out << "Aadhaar: " << a.aadhaar << endl;
        out << "Phone: " << a.number << endl;
        out << "Address: " << a.address << endl;
        return out;
    }
};

// Date class
class date {
public:
    int month, year;

    date() {}

    friend istream& operator >> (istream& in, date& a) {
        cout << "Month: "; in >> a.month;
        cout << "Year: "; in >> a.year;
        return in;
    }

    friend ostream& operator << (ostream& out, const date& a) {
        out << a.month << "/" << a.year;
        return out;
    }
};

// Reservation class
class reservation {
public:
    static int base_fare;
    static int ticket_counter;

    contact reserver;
    string source, destination;
    date travel_date;
    string ticket_no;
    char clas;
    int fare;

    reservation() {}

    void input(list<string>& stations) {
        cin.ignore();
        cout << "Source Station: "; getline(cin, source);
        cout << "Destination Station: "; getline(cin, destination);
        cout << "Enter Your Details:\n";
        cin >> reserver;
        cout << "Enter Date of Travel:\n";
        cin >> travel_date;

        cout << "Class (E = Economy / B = Business): ";
        while (true) {
            cin >> clas;
            clas = toupper(clas);
            if (clas == 'E' || clas == 'B') break;
            cout << "Invalid class. Please enter 'E' or 'B': ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear remaining input

        fare = base_fare * (clas == 'B' ? 2 : 1);

        // Generate unique ticket number
        ticket_no = "#IN" + to_string(1000 + ticket_counter++) + clas;
    }

    void display() const {
        cout << "Ticket No: " << ticket_no << endl;
        cout << "From: " << source << " To: " << destination << endl;
        cout << "Class: " << (clas == 'B' ? "Business" : "Economy") << endl;
        cout << "Fare: INR " << fare << endl;
        cout << "Travel Date: " << travel_date << endl;
        cout << reserver;
    }

    string ticket() const {
        return ticket_no;
    }

    bool operator == (const reservation& b) const {
        return ticket_no == b.ticket_no;
    }
};

int reservation::base_fare = 100;
int reservation::ticket_counter = 1;

// Global data
list<string> stations = {"Delhi", "Mumbai", "Chennai", "Kolkata"};
list<reservation> reservations;
user admin_user = {"admin", "admin123"};
user normal_user = {"user", "user123"};

// Functions
void reserve_ticket() {
    reservation r;
    r.input(stations);
    reservations.push_back(r);
    cout << "\nReservation Successful. Your Ticket No: " << r.ticket() << endl;
}

void cancel_ticket() {
    string tno;
    cout << "Enter Ticket No to Cancel: "; cin >> tno;
    auto it = find_if(reservations.begin(), reservations.end(), [&](reservation& r) {
        return r.ticket() == tno;
    });
    if (it != reservations.end()) {
        it->display();
        char confirm;
        cout << "Cancel this ticket? (Y/N): "; cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            reservations.erase(it);
            cout << "Reservation Cancelled.\n";
        }
    } else {
        cout << "No reservation found with Ticket No: " << tno << endl;
    }
}

void show_all_reservations() {
    int i = 1;
    for (const auto& r : reservations) {
        cout << "Reservation #" << i++ << ":\n";
        r.display();
        cout << "-------------------------\n";
    }
    if (reservations.empty()) {
        cout << "No reservations found.\n";
    }
}

void show_stations() {
    int i = 1;
    cout << "Available Stations:\n";
    for (const auto& s : stations) {
        cout << i++ << ". " << s << endl;
    }
}

void login_menu() {
    string uname, pass;
    while (true) {
        cout << "Username: "; cin >> uname;
        cout << "Password: "; cin >> pass;
        if ((uname == admin_user.name && pass == admin_user.psw) ||
            (uname == normal_user.name && pass == normal_user.psw)) {
            cout << "Login successful.\n";
            break;
        } else {
            cout << "Invalid credentials. Try again.\n";
        }
    }
}

int main() {
    cout << "Indian Rail Reservation System 1.0\n";
    login_menu();

    char choice;
    do {
        cout << "\nMain Menu:\n";
        cout << "1. Reserve Seat\n";
        cout << "2. Cancel Reservation\n";
        cout << "3. View Reservations\n";
        cout << "4. Show Stations\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        switch (choice) {
            case '1': reserve_ticket(); break;
            case '2': cancel_ticket(); break;
            case '3': show_all_reservations(); break;
            case '4': show_stations(); break;
            case '0': cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != '0');

    return 0;
}
