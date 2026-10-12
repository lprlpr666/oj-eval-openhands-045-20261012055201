#include <iostream>
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    
    // Elevator state
    int current_floor = 0;
    bool going_up = true;
    
    // Set to store pending requests
    set<int> requests;
    
    for (int i = 0; i < n; i++) {
        string operation;
        cin >> operation;
        
        if (operation == "action") {
            if (requests.empty()) {
                // No requests, stay in place
                continue;
            }
            
            int next_floor;
            
            // Find next floor based on current direction
            if (going_up) {
                // Look for requests at or above current floor
                auto it = requests.lower_bound(current_floor);
                if (it != requests.end()) {
                    next_floor = *it;
                } else {
                    // No requests above, reverse direction
                    going_up = false;
                    next_floor = *requests.rbegin();
                }
            } else {
                // Going down, look for requests at or below current floor
                auto it = requests.upper_bound(current_floor);
                if (it != requests.begin()) {
                    --it;
                    next_floor = *it;
                } else {
                    // No requests below, reverse direction
                    going_up = true;
                    next_floor = *requests.begin();
                }
            }
            
            // Move elevator to next floor
            current_floor = next_floor;
            requests.erase(next_floor);
            
        } else if (operation == "add") {
            int floor;
            cin >> floor;
            requests.insert(floor);
            
        } else if (operation == "cancel") {
            int floor;
            cin >> floor;
            requests.erase(floor);
            
        } else if (operation == "locate") {
            cout << current_floor << endl;
        }
    }
    
    return 0;
}