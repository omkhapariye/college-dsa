#include <iostream>
#include <string>
#include <cstdlib> // For malloc and free
#include <limits>  // For numeric_limits

using namespace std;

// Constant for total number of seats
const int TOTAL_SEATS = 30;

// Node structure for Singly Linked List to store passenger details
struct Passenger {
    int seatNo;
    string name;
    int age;
    char gender;
    Passenger* next;
};

// Function prototypes (Modular design)
int* initializeSeats(int totalSeats);
void bookTicket(int* seats, int totalSeats, Passenger*& head);
void cancelTicket(int* seats, int totalSeats, Passenger*& head);
void viewTrainStatus(const int* seats, int totalSeats);
void viewPassengerList(const Passenger* head);
void freeAllMemory(int*& seats, Passenger*& head);

int main() {
    // 1. Dynamically allocate array for seat availability using malloc
    // 0 = Available, 1 = Booked
    int* seats = initializeSeats(TOTAL_SEATS);
    if (seats == nullptr) {
        cerr << "Error: Memory allocation failed for seat tracking array!\n";
        return 1;
    }

    // Head pointer for the passenger linked list
    Passenger* head = nullptr;
    int choice;

    do {
        cout << "\n----------------------------------------\n";
        cout << "   Railway Reservation System Menu\n";
        cout << "----------------------------------------\n";
        cout << "1. Book Ticket\n";
        cout << "2. Cancel Ticket\n";
        cout << "3. View Train Status\n";
        cout << "4. View Passenger List\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        // Safe input reading
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number (1-5).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                bookTicket(seats, TOTAL_SEATS, head);
                break;
            case 2:
                cancelTicket(seats, TOTAL_SEATS, head);
                break;
            case 3:
                viewTrainStatus(seats, TOTAL_SEATS);
                break;
            case 4:
                viewPassengerList(head);
                break;
            case 5:
                cout << "\nExiting system and freeing all allocated memory...\n";
                freeAllMemory(seats, head);
                cout << "All memory successfully deallocated. Thank you!\n";
                break;
            default:
                cout << "Invalid choice! Please choose between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}

/**
 * Initializes the seat status array dynamically using malloc.
 * Sets all seats to 0 (Available).
 */
int* initializeSeats(int totalSeats) {
    int* seats = (int*)malloc(totalSeats * sizeof(int));
    if (seats != nullptr) {
        for (int i = 0; i < totalSeats; i++) {
            seats[i] = 0; // 0 = Available
        }
    }
    return seats;
}

/**
 * a. Book Ticket:
 * Finds the first available seat, takes passenger details, marks the seat as booked (1),
 * and appends a new passenger node to the linked list.
 */
void bookTicket(int* seats, int totalSeats, Passenger*& head) {
    // Find the first available seat
    int allocatedSeatIndex = -1;
    for (int i = 0; i < totalSeats; i++) {
        if (seats[i] == 0) {
            allocatedSeatIndex = i;
            break;
        }
    }

    if (allocatedSeatIndex == -1) {
        cout << "\nSorry, the train is full! No seats available.\n";
        return;
    }

    int seatNumber = allocatedSeatIndex + 1;

    // Clear newline character left in input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name;
    int age;
    char gender;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Age: ";
    while (!(cin >> age) || age <= 0) {
        cout << "Invalid age! Please enter a valid positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter Gender (M/F/O): ";
    cin >> gender;

    // Mark seat as Booked (1)
    seats[allocatedSeatIndex] = 1;

    // Dynamically create a new passenger node
    Passenger* newPassenger = new Passenger();
    newPassenger->seatNo = seatNumber;
    newPassenger->name = name;
    newPassenger->age = age;
    newPassenger->gender = gender;
    newPassenger->next = nullptr;

    // Insert into linked list (in ascending order of seat numbers)
    if (head == nullptr || head->seatNo > seatNumber) {
        newPassenger->next = head;
        head = newPassenger;
    } else {
        Passenger* curr = head;
        while (curr->next != nullptr && curr->next->seatNo < seatNumber) {
            curr = curr->next;
        }
        newPassenger->next = curr->next;
        curr->next = newPassenger;
    }

    cout << "Ticket booked! Seat No: " << seatNumber << "\n";
}

/**
 * b. Cancel Ticket:
 * Takes a seat number, validates it, marks the seat as 0 (Available),
 * and removes the passenger from the linked list, freeing its node.
 */
void cancelTicket(int* seats, int totalSeats, Passenger*& head) {
    if (head == nullptr) {
        cout << "\nNo tickets have been booked yet.\n";
        return;
    }

    int seatNo;
    cout << "Enter Seat Number to cancel (1-" << totalSeats << "): ";
    if (!(cin >> seatNo)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Boundary check
    if (seatNo < 1 || seatNo > totalSeats) {
        cout << "Invalid seat number! Must be between 1 and " << totalSeats << ".\n";
        return;
    }

    // Check if the seat is actually booked
    if (seats[seatNo - 1] == 0) {
        cout << "Seat " << seatNo << " is not booked!\n";
        return;
    }

    // Free the seat in the array
    seats[seatNo - 1] = 0;

    // Remove passenger from the linked list
    Passenger* curr = head;
    Passenger* prev = nullptr;

    while (curr != nullptr && curr->seatNo != seatNo) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr) {
        if (prev == nullptr) {
            head = curr->next; // Removing head node
        } else {
            prev->next = curr->next; // Removing middle/last node
        }
        delete curr; // Free the allocated node memory
        cout << "Ticket for Seat No: " << seatNo << " cancelled successfully!\n";
    }
}

/**
 * c. View Train Status:
 * Displays all seats from 1 to 30 with their status (Booked / Available).
 */
void viewTrainStatus(const int* seats, int totalSeats) {
    cout << "\n--- Train Seat Status ---\n";
    for (int i = 0; i < totalSeats; i++) {
        cout << "Seat " << (i + 1) << ": " 
             << (seats[i] == 1 ? "Booked" : "Available") << "\n";
    }
}

/**
 * d. View Passenger List:
 * Traverses the linked list and displays details of all booked passengers.
 */
void viewPassengerList(const Passenger* head) {
    if (head == nullptr) {
        cout << "\nNo passengers booked yet.\n";
        return;
    }

    cout << "\n--- Passenger List ---\n";
    const Passenger* curr = head;
    while (curr != nullptr) {
        cout << "Seat No: " << curr->seatNo 
             << " | Name: " << curr->name 
             << " | Age: " << curr->age 
             << " | Gender: " << curr->gender << "\n";
        curr = curr->next;
    }
}

/**
 * e. Exit & Free All Memory:
 * Frees all allocated linked list nodes and the dynamically allocated seats array.
 */
void freeAllMemory(int*& seats, Passenger*& head) {
    // 1. Free all linked list nodes
    Passenger* curr = head;
    while (curr != nullptr) {
        Passenger* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;

    // 2. Free dynamically allocated array using free()
    if (seats != nullptr) {
        free(seats);
        seats = nullptr;
    }
}