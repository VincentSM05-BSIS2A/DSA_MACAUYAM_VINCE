#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Function to display the heap as a vector
void displayHeap(const vector<int>& heap) {
    cout << "[ ";
    for (int val : heap) cout << val << " ";
    cout << "]\n";
}

// Function to heapify up
void heapifyUp(vector<int>& heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index] > heap[parent]) {
            swap(heap[index], heap[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

// Function to heapify down
void heapifyDown(vector<int>& heap, int index) {
    int size = heap.size();
    while (true) {
        int left = 2 * index + 1, right = 2 * index + 2, largest = index;
        if (left < size && heap[left] > heap[largest]) largest = left;
        if (right < size && heap[right] > heap[largest]) largest = right;

        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        }
        else {
            break;
        }
    }
}

// Function to insert a shipment into the heap
void insertShipment(vector<int>& heap, int value) {
    heap.push_back(value);
    heapifyUp(heap, heap.size() - 1);
}

// Function to remove the highest-priority shipment (root)
int removeShipment(vector<int>& heap) {
    if (heap.empty()) return -1;

    int root = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) heapifyDown(heap, 0);

    return root;
}

// Function to display the backstory and mission
void displayIntroduction() {
    cout << "🌌 Welcome to Galactic Cargo Management! 🌌\n";
    cout << "In the far future, humans have built colonies on planets across the galaxy. ";
    cout << "These colonies depend on shipments of important supplies like food, medicine, and tools to survive and grow. ";
    cout << "To keep everything running smoothly, a system is needed to organize and prioritize deliveries based on how urgent they are.\n\n";

    cout << "You are the cargo manager on the spaceship SS Nexus, one of the fastest and most advanced freighters in the galaxy. ";
    cout << "Your job is to make sure the most critical shipments are delivered first, without delaying less urgent ones for too long.\n\n";

    cout << "🛠️ Your Mission:\n";
    cout << "1. Insert Shipments: Add new shipments into the priority system without disrupting the order.\n";
    cout << "2. Dispatch Shipments: Remove and deliver the most urgent shipments efficiently.\n";
    cout << "3. Maintain Order: Use the Heapify process to keep the system working correctly.\n";
    cout << "4. Monitor Changes: Watch how your actions impact the priority structure and make adjustments as needed.\n";
    cout << "-------------------------------------------------------------------------------\n\n";
}

int main() {
    vector<int> heap;
    vector<int> shipments = { 12, 35, 9, 18, 23 }; // Predefined shipment priorities

    // Display backstory and mission
    displayIntroduction();

    while (true) {
        cout << "Available shipments: ";
        displayHeap(shipments);

        cout << "\nCommands:\n";
        cout << "1. Insert a shipment into the cargo heap.\n";
        cout << "2. Dispatch the highest-priority shipment.\n";
        cout << "3. View the current heap.\n";
        cout << "4. Exit.\n";

        int choice;
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            if (shipments.empty()) {
                cout << "No shipments left to insert.\n";
                continue;
            }

            cout << "Choose a shipment to insert (priority value): ";
            int value;
            cin >> value;

            auto it = find(shipments.begin(), shipments.end(), value);
            if (it != shipments.end()) {
                insertShipment(heap, value);
                shipments.erase(it);
                cout << "Inserted shipment with priority " << value << " into the heap.\n";
            }
            else {
                cout << "Invalid shipment. Please select a valid priority.\n";
            }
        }
        else if (choice == 2) {
            int dispatched = removeShipment(heap);
            if (dispatched != -1) {
                cout << "Dispatched shipment with priority " << dispatched << ".\n";
            }
            else {
                cout << "Heap is empty. No shipments to dispatch.\n";
            }
        }
        else if (choice == 3) {
            if (heap.empty()) {
                cout << "The heap is empty.\n";
            }
            else {
                cout << "Current heap (priority order): ";
                displayHeap(heap);
            }
        }
        else if (choice == 4) {
            cout << "Exiting Galactic Cargo Management. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

        if (heap.empty() && shipments.empty()) {
            cout << "\nAll shipments have been managed. You completed the task!\n";
            break;
        }
    }

    return 0;
}
