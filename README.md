# Hotel Management System

> OOPS Lab Project · C++

|

A menu-driven console application in C++ that manages rooms, bookings, guest check-in/check-out and billing for a small hotel.

For a beginner-friendly walkthrough of the code, see [OOPS_Hotel_Explained.md](OOPS_Hotel_Explained.md).

---

## Modules

| # | Module | What it does | Menu options |
|---|---|---|---|
| 1 | Manage room details | Add a room (Standard or Deluxe), display all rooms, update a room's price, delete a room | 1, 2, 3, 4 |
| 2 | Check availability and book rooms | List rooms that are free, book a room for a guest | 5, 6 |
| 3 | Manage guest check-in and check-out | Check a booked guest in, check them out, list all guests with their status | 7, 8, 9 |
| 4 | Calculate bills and payments | Generate the bill at check-out (room charges + 12% GST), accept payment until the balance is cleared, return change | inside 8 |

Option 10 shows how many `Room` objects currently exist.

## How it works

1. An admin adds rooms (option 1). Each room has a number, a type and a price per night.
2. A guest books an available room (option 6) by giving their name and number of days. They receive a **Guest ID** (starting at 1001) and the room is marked *Booked*.
3. On arrival the guest is checked in (option 7): `Booked -> Checked-in`.
4. At departure (option 8) the bill is printed and payment is requested until the full amount is paid. Any excess is returned as change. The guest becomes `Checked-out` and the room is free again.

### Billing rules

| Room type | Room charge |
|---|---|
| Standard | `price x days` |
| Deluxe | `price x days + Rs 500` (flat service charge) |

GST of 12% is added on the room charge. Example: Deluxe room at Rs 2500/night for 3 days: 2500 x 3 + 500 = 8000, GST = 960, **total = Rs 8960**.

## Class design

```
Room  (base class)
 └── DeluxeRoom  (derived class, overrides calculateBill)

Guest  (stores booking and stay details)

printBill()  (friend function of Guest)
```

| Class | Responsibility |
|---|---|
| `Room` | Room number, type, price and booked flag. Virtual `calculateBill()` and `display()`. Static counter of live rooms |
| `DeluxeRoom` | Sets type to "Deluxe" and overrides `calculateBill()` to add the Rs 500 service charge |
| `Guest` | Guest ID, name, room number, days of stay, status (0 = Booked, 1 = Checked-in, 2 = Checked-out) |

Rooms are stored in `Room* rooms[100]`, so one array holds both `Room` and `DeluxeRoom` objects, and the correct `calculateBill()` runs at runtime. Guests are stored in `Guest guests[100]`.

## OOP concepts used

| Concept | Where |
|---|---|
| Encapsulation | Data members are `private`/`protected`, accessed through getters and setters |
| Constructors | `Room()`, `DeluxeRoom()`, `Guest()` |
| Inheritance | `DeluxeRoom : public Room` |
| Polymorphism | Virtual `calculateBill()` and `display()`, called through `Room*` pointers |
| Function overriding | `DeluxeRoom::calculateBill()` uses the `override` keyword |
| Virtual destructor | `virtual ~Room()` so deleting through a base pointer is safe |
| Static data member and function | `Room::count` and `Room::showCount()` |
| Friend function | `printBill()` reads the private fields of `Guest` |
| Dynamic memory | Rooms are created with `new` and released with `delete` |

## Input validation

- Duplicate room numbers are rejected.
- Price must be positive (an invalid price falls back to Rs 1000 when adding, and is rejected when updating).
- Number of days must be at least 1.
- A booked room cannot be booked again or deleted.
- Check-in works only for *Booked* guests; check-out only for *Checked-in* guests.
- Payment must be a positive number. Letters or zero at the payment prompt are rejected and asked again.

## Build and run

Requires any C++ compiler (g++ recommended).

```bash
g++ HotelManagement.cpp -o hotel
./hotel          # on Windows: hotel.exe
```

## Sample output

```
Enter choice: 8
Enter Guest ID: 1001

------------- BILL -------------
Guest: Rahul   Room: 201   Days: 3
Room Charges : Rs 8000
GST (12%)    : Rs 960
TOTAL        : Rs 8960
--------------------------------
Enter payment (Balance Rs 8960): 1000
Enter payment (Balance Rs 7960): 9000
Return change: Rs 40
Payment complete. Thank you!
```

<!-- Add a screenshot of your own run here:  ![Demo](screenshot.png) -->

## Limitations

- Data is kept in memory only and is lost when the program exits.
- Capacity is fixed at 100 rooms and 100 guests.
- Guest names must be a single word (input uses `cin >>`).
- Typing letters where a number is expected (for example at the main menu) makes the program exit.
- Availability is "booked or not"; there is no date-based booking.

## Possible extensions

- Save and load rooms and guests from a file.
- Add a `Suite` class derived from `Room` with its own pricing.
- Date-based availability.
- Extra services (food, laundry) added to the bill.

## Author

Kavyansh Gupta · [GitHub](https://github.com/kavyansh09)
