<!-- markdownlint-disable -->

# Hotel Management System — Explained Simply

**Subject:** OOPS Lab | **Language:** C++ | **File:** `HotelManagement.cpp`

This document explains what the project does and how the code works, in plain language. You do not need to read the code first.

---

## 1. What is this project?

A small program that runs a hotel's front desk. Using a text menu, you can:

- add and manage **rooms**
- **book** a room for a guest
- **check the guest in** when they arrive
- **check them out**, print their **bill** and take **payment**

Everything is stored in memory while the program runs. When you close it, the data is gone (there is no file or database).

### The four modules

| # | Module | What it does | Menu option |
|---|---|---|---|
| 1 | Manage room details | Add, show, update price of, and delete rooms | 1, 2, 3, 4 |
| 2 | Check availability and book rooms | Show free rooms, book one for a guest | 5, 6 |
| 3 | Guest check-in and check-out | Check in a booked guest, check them out, list all guests | 7, 8, 9 |
| 4 | Bills and payments | Calculate the bill, add GST, take payment, give back change | inside 8 |

---

## 2. How to run it

```bash
g++ HotelManagement.cpp -o hotel
./hotel
```

---

## 3. The story of one guest (the whole flow)

1. **Add a room.** Menu 1 → room 201, Deluxe, Rs 2500 per night.
2. **Book it.** Menu 6 → enter room 201, guest name, number of days. The program gives a **Guest ID** (e.g. 1001). The room now shows as *Booked*.
3. **Check in.** Menu 7 → enter Guest ID 1001. Status changes to *Checked-in*.
4. **Check out.** Menu 8 → enter Guest ID 1001. The program prints the bill and asks for payment until it is fully paid. The room becomes *Available* again.

```
Booked  ──check-in──►  Checked-in  ──check-out + payment──►  Checked-out
```

---

## 4. The code: three building blocks

The program has three main pieces. Think of them as **blueprints** (classes) that create real things (objects).

```
Room  ───────────►  DeluxeRoom        (a special kind of Room)

Guest                                 (a person who booked a room)

printBill()                           (a helper that prints the bill)
```

### 4.1 `Room` class — one hotel room

Each `Room` object remembers:

| Variable | Meaning |
|---|---|
| `roomNo` | Room number, e.g. 101 |
| `type` | "Standard" or "Deluxe" |
| `price` | Price per night |
| `booked` | `true` if someone has booked it, `false` if free |
| `count` (static) | How many Room objects exist in total (shared by all rooms) |

And it can do things (functions):

| Function | What it does |
|---|---|
| `Room()` — constructor | Runs automatically when a room is created. Sets default values and increases `count` |
| `input()` | Asks the user for room number and price. Rejects a price of 0 or less |
| `setPrice()` | Changes the price (only if it is positive) |
| `getRoomNo()`, `isBooked()` | "Getters": let other code read the private values |
| `setBooked()` | Marks the room booked or free |
| `calculateBill(days)` | Returns `price × days`. It is **virtual** (see DeluxeRoom below) |
| `display()` | Prints one line, e.g. `Room 101 | Standard | Rs 1500/night | Available` |
| `showCount()` (static) | Prints how many rooms exist |
| `~Room()` — destructor | Runs when a room is deleted. Decreases `count` |

### 4.2 `DeluxeRoom` class — a fancier room

`DeluxeRoom` is built **on top of** `Room` (`class DeluxeRoom : public Room`). It gets everything Room has, and changes two things:

- sets its type to `"Deluxe"`
- **overrides** `calculateBill()`: a Deluxe room charges `price × days + Rs 500` (a flat service charge)

This is the heart of the OOPS project: same function name, different behaviour depending on the type of room.

### 4.3 `Guest` class — one booking

| Variable | Meaning |
|---|---|
| `id` | Guest ID given at booking (1001, 1002, ...) |
| `roomNo` | Which room this guest booked |
| `days` | How many days they will stay |
| `status` | 0 = Booked, 1 = Checked-in, 2 = Checked-out |
| `name` | Guest's name (one word) |

Important functions: `input()` (asks name and days), `setStatus()`, `display()`.

### 4.4 `printBill()` — a friend function

Guest's data is `private`, so normally nobody outside the class can read it. `printBill()` is declared as a **friend** of `Guest`, which gives it special permission to read the name, room and days directly and print the bill:

```
------------- BILL -------------
Guest: Rahul   Room: 201   Days: 3
Room Charges : Rs 8000
GST (12%)    : Rs 960
TOTAL        : Rs 8960
--------------------------------
```

### 4.5 Two search helpers

`findRoom()` and `findGuest()` loop through the arrays and return the position of the match, or `-1` if not found. Almost every menu option uses them first ("does this room / guest exist?").

---

## 5. The `main()` function

### Storage

```cpp
Room* rooms[100];     // an array of POINTERS to rooms
Guest guests[100];    // an array of Guest objects
```

Why `Room*` (pointers) for rooms? Because one array must hold **both** normal `Room`s and `DeluxeRoom`s. A pointer to the base class (`Room*`) can point to either. This is what makes polymorphism work.

### The menu loop

A `do { ... } while (choice != 0);` loop shows the menu again and again until the user enters 0. Inside, a `switch` runs the code for the chosen option.

### What each menu option does

| Option | What happens in the code |
|---|---|
| 1 Add Room | Asks type. Creates `new DeluxeRoom()` or `new Room()`. Calls `input()`. If the room number already exists, deletes it and says so. Otherwise stores the pointer in `rooms[]` |
| 2 Display Rooms | Loops over all rooms and calls `display()` |
| 3 Update Price | Finds the room, asks the new price, calls `setPrice()` |
| 4 Delete Room | Refuses if the room is booked. Otherwise `delete`s it and shifts the later rooms one step left to close the gap |
| 5 Show Available | Loops over rooms and displays only those where `isBooked()` is false |
| 6 Book Room | Checks the room exists and is free. Takes guest details, gives a new Guest ID, marks the room booked |
| 7 Check-In | Finds the guest. Only allowed if status is *Booked*. Sets status to *Checked-in* |
| 8 Check-Out | Only allowed if *Checked-in*. Calculates bill (see below), asks payment until paid, frees the room |
| 9 Show Guests | Loops over guests and calls `display()` |
| 10 Total Rooms | Calls `Room::showCount()` |

### How the bill and payment work (option 8)

```
charge = room->calculateBill(days)     // Standard: price × days
                                       // Deluxe:   price × days + 500
gst    = charge × 0.12                 // 12% tax
total  = charge + gst
```

Then a loop keeps asking for payment:

```
paid = 0
while (paid < total):
    ask amount
    if amount is valid: paid = paid + amount
if paid > total: return the extra as change
```

So the guest can pay in parts (e.g. Rs 1000 now, the rest later in the same prompt), and overpaying gives change.

The line `rooms[r]->calculateBill(days)` is **polymorphism** in action: the program does not check "is this Deluxe?". C++ automatically picks the correct version at runtime.

---

## 6. OOPS concepts used, in simple words

| Concept | Simple meaning | Where in this project |
|---|---|---|
| **Class and object** | A class is a blueprint; an object is a real thing made from it | `Room`, `Guest` are blueprints; room 101 is an object |
| **Encapsulation** | Keep data private; allow access only through functions | Variables are `private`/`protected`; use getters and setters |
| **Constructor** | Function that runs automatically when an object is created | `Room()`, `Guest()`, `DeluxeRoom()` |
| **Destructor** | Function that runs automatically when an object is destroyed | `~Room()` reduces the room count |
| **Inheritance** | A new class reuses an existing class | `DeluxeRoom` inherits from `Room` |
| **Polymorphism** | Same function call, different behaviour depending on the object | `calculateBill()` differs for Standard and Deluxe |
| **Virtual function / overriding** | The keyword that makes polymorphism work; a child class replaces the parent's version | `virtual calculateBill()` in Room, `override` in DeluxeRoom |
| **Virtual destructor** | Makes sure the right destructor runs when deleting through a base pointer | `virtual ~Room()` |
| **Static member** | One value shared by all objects of the class | `Room::count` counts all rooms |
| **Friend function** | An outside function allowed to read private data | `printBill()` and `Guest` |
| **Dynamic memory** | Create objects while the program runs with `new`, free them with `delete` | Rooms are created with `new` and deleted at the end |

---

## 7. Rules built into the program (validation)

- Two rooms cannot have the same number.
- Room price must be above 0.
- Number of days must be at least 1.
- A booked room cannot be booked again or deleted.
- You can only check in a guest who is *Booked*, and only check out a guest who is *Checked-in*.
- Payment must be a positive number. Typing letters at the payment prompt is rejected and asked again.

---

## 8. Limitations (be honest about these in the viva)

- No saving: all data is lost when the program closes.
- Maximum 100 rooms and 100 guests.
- Guest name is one word only (it uses `cin >>`).
- Typing letters where a number is expected (for example the menu choice) can make the program exit, because input checking is basic.
- No dates: availability is just "booked or not", not "free from 5 Nov to 8 Nov".

**Easy improvements to mention:** save to a file, add a `Suite` room class (another child of `Room`), date-based booking.

---

## 9. Likely viva questions

**Q: Where did you use inheritance?**
`DeluxeRoom` inherits from `Room`.

**Q: Where is polymorphism?**
`calculateBill()` is virtual. We call it through a `Room*` pointer and C++ runs the Standard or Deluxe version based on the real object.

**Q: Why are rooms stored as `Room*` and not `Room`?**
So one array can hold both `Room` and `DeluxeRoom` objects. If we stored plain `Room` objects, a Deluxe room would be cut down to a normal Room and lose its own behaviour.

**Q: Why is the destructor virtual?**
When we `delete` a `DeluxeRoom` through a `Room*`, a virtual destructor makes sure the correct destructor runs.

**Q: What does `static` do for `count`?**
There is only one `count`, shared by all rooms. Each new room adds 1 and each deleted room subtracts 1.

**Q: What is a friend function and why did you use it?**
A function outside the class that is allowed to read its private data. `printBill()` needs the guest's private name, room and days to print the bill.

**Q: How is the bill calculated?**
Standard: price × days. Deluxe: price × days + Rs 500. Then 12% GST is added.

**Q: What happens if you book an already booked room?**
`isBooked()` returns true, so the program prints "Room is already booked!" and does not create a booking.
