#include <iostream>
#include <string>
using namespace std;

class Room {
protected:
    int roomNo;
    string type;
    float price;
    bool booked;
    static int count;

public:
    Room() {
        roomNo = 0;
        type = "Standard";
        price = 0;
        booked = false;
        count++;
    }

    void input() {
        cout << "Enter Room No: ";
        cin >> roomNo;
        cout << "Enter Price per night: ";
        cin >> price;
        if (price <= 0) {
            cout << "Invalid price! Set to 1000.\n";
            price = 1000;
        }
    }

    void setPrice(float p) {
        if (p > 0)
            price = p;
        else
            cout << "Invalid price!\n";
    }

    int getRoomNo() { return roomNo; }
    bool isBooked() { return booked; }
    void setBooked(bool b) { booked = b; }

    static void showCount() {
        cout << "Total room objects: " << count << endl;
    }

    virtual float calculateBill(int days) {
        return price * days;
    }

    virtual void display() {
        cout << "Room " << roomNo << " | " << type << " | Rs " << price << "/night | ";
        cout << (booked ? "Booked" : "Available") << endl;
    }

    virtual ~Room() { count--; }
};

int Room::count = 0;

class DeluxeRoom : public Room {
public:
    DeluxeRoom() { type = "Deluxe"; }

    // Deluxe rooms add a fixed Rs 500 service charge
    float calculateBill(int days) override {
        return price * days + 500;
    }
};

class Guest {
private:
    int id, roomNo, days;
    int status; // 0 = Booked, 1 = Checked-in, 2 = Checked-out
    string name;

public:
    Guest() {
        id = roomNo = days = status = 0;
    }

    void input(int guestId, int room) {
        id = guestId;
        roomNo = room;
        cout << "Enter Guest Name: ";
        cin >> name;
        cout << "Enter Number of Days: ";
        cin >> days;
        if (days < 1) {
            cout << "Invalid days! Set to 1.\n";
            days = 1;
        }
    }

    int getId() { return id; }
    int getRoomNo() { return roomNo; }
    int getDays() { return days; }
    int getStatus() { return status; }
    void setStatus(int s) { status = s; }

    void display() {
        string st[] = {"Booked", "Checked-in", "Checked-out"};
        cout << "ID " << id << " | " << name << " | Room " << roomNo << " | " << days << " days | " << st[status] << endl;
    }

    friend void printBill(Guest g, float charge, float gst, float total);
};

void printBill(Guest g, float charge, float gst, float total) {
    cout << "\n------------- BILL -------------\n";
    cout << "Guest: " << g.name << "   Room: " << g.roomNo << "   Days: " << g.days << endl;
    cout << "Room Charges : Rs " << charge << endl;
    cout << "GST (12%)    : Rs " << gst << endl;
    cout << "TOTAL        : Rs " << total << endl;
    cout << "--------------------------------\n";
}

int findRoom(Room* rooms[], int count, int no) {
    for (int i = 0; i < count; i++)
        if (rooms[i]->getRoomNo() == no)
            return i;
    return -1;
}

int findGuest(Guest guests[], int count, int id) {
    for (int i = 0; i < count; i++)
        if (guests[i].getId() == id)
            return i;
    return -1;
}

int main() {
    const int MAX = 100;
    Room* rooms[MAX];
    Guest guests[MAX];
    int roomCount = 0, guestCount = 0, nextId = 1001;
    int choice;

    do {
        cout << "\n===== HOTEL MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Room\n";
        cout << "2. Display Rooms\n";
        cout << "3. Update Room Price\n";
        cout << "4. Delete Room\n";
        cout << "5. Show Available Rooms\n";
        cout << "6. Book Room\n";
        cout << "7. Guest Check-In\n";
        cout << "8. Guest Check-Out (Bill & Payment)\n";
        cout << "9. Show Guests\n";
        cout << "10. Show Total Rooms\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (roomCount == MAX) {
                    cout << "Room list is full!\n";
                    break;
                }
                int t;
                cout << "Room type (1. Standard  2. Deluxe): ";
                cin >> t;

                Room* r;
                if (t == 2)
                    r = new DeluxeRoom();
                else
                    r = new Room();
                r->input();

                if (findRoom(rooms, roomCount, r->getRoomNo()) != -1) {
                    cout << "Room number already exists!\n";
                    delete r;
                } else {
                    rooms[roomCount++] = r;
                    cout << "Room added successfully!\n";
                }
                break;
            }

            case 2:
                if (roomCount == 0)
                    cout << "No rooms found!\n";
                for (int i = 0; i < roomCount; i++)
                    rooms[i]->display();
                break;

            case 3: {
                int no, f;
                float p;
                cout << "Enter Room No: ";
                cin >> no;
                f = findRoom(rooms, roomCount, no);
                if (f == -1) {
                    cout << "Room not found!\n";
                } else {
                    cout << "Enter new price: ";
                    cin >> p;
                    rooms[f]->setPrice(p);
                }
                break;
            }

            case 4: {
                int no, f;
                cout << "Enter Room No to delete: ";
                cin >> no;
                f = findRoom(rooms, roomCount, no);
                if (f == -1) {
                    cout << "Room not found!\n";
                } else if (rooms[f]->isBooked()) {
                    cout << "Room is booked, cannot delete!\n";
                } else {
                    delete rooms[f];
                    for (int i = f; i < roomCount - 1; i++)
                        rooms[i] = rooms[i + 1];
                    roomCount--;
                    cout << "Room deleted successfully!\n";
                }
                break;
            }

            case 5: {
                bool any = false;
                for (int i = 0; i < roomCount; i++) {
                    if (!rooms[i]->isBooked()) {
                        rooms[i]->display();
                        any = true;
                    }
                }
                if (!any)
                    cout << "No rooms available!\n";
                break;
            }

            case 6: {
                int no, f;
                cout << "Enter Room No to book: ";
                cin >> no;
                f = findRoom(rooms, roomCount, no);
                if (f == -1) {
                    cout << "Room not found!\n";
                } else if (rooms[f]->isBooked()) {
                    cout << "Room is already booked!\n";
                } else if (guestCount == MAX) {
                    cout << "Guest list is full!\n";
                } else {
                    guests[guestCount++].input(nextId, no);
                    rooms[f]->setBooked(true);
                    cout << "Room booked! Guest ID: " << nextId++ << endl;
                }
                break;
            }

            case 7: {
                int id, g;
                cout << "Enter Guest ID: ";
                cin >> id;
                g = findGuest(guests, guestCount, id);
                if (g == -1) {
                    cout << "Guest not found!\n";
                } else if (guests[g].getStatus() != 0) {
                    cout << "Guest is not in 'Booked' state!\n";
                } else {
                    guests[g].setStatus(1);
                    cout << "Check-in successful!\n";
                }
                break;
            }

            case 8: {
                int id, g;
                cout << "Enter Guest ID: ";
                cin >> id;
                g = findGuest(guests, guestCount, id);
                if (g == -1) {
                    cout << "Guest not found!\n";
                } else if (guests[g].getStatus() != 1) {
                    cout << "Guest is not checked in!\n";
                } else {
                    int r = findRoom(rooms, roomCount, guests[g].getRoomNo());
                    float charge = rooms[r]->calculateBill(guests[g].getDays());
                    float gst = charge * 0.12;
                    float total = charge + gst;
                    printBill(guests[g], charge, gst, total);

                    float paid = 0, amount;
                    while (paid < total) {
                        cout << "Enter payment (Balance Rs " << total - paid << "): ";
                        if (!(cin >> amount)) {   // user typed letters
                            cin.clear();
                            cin.ignore(1000, '\n');
                            amount = 0;
                        }
                        if (amount > 0)
                            paid += amount;
                        else
                            cout << "Invalid amount!\n";
                    }
                    if (paid > total)
                        cout << "Return change: Rs " << paid - total << endl;

                    cout << "Payment complete. Thank you!\n";
                    guests[g].setStatus(2);
                    rooms[r]->setBooked(false);
                }
                break;
            }

            case 9:
                if (guestCount == 0)
                    cout << "No guest records found!\n";
                for (int i = 0; i < guestCount; i++)
                    guests[i].display();
                break;

            case 10:
                Room::showCount();
                break;

            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    for (int i = 0; i < roomCount; i++)
        delete rooms[i];

    return 0;
}
