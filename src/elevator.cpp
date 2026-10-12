#include <iostream>
#include <cstdio>

using namespace std;

const int MAX_N = 500000;

struct MinHeap {
    int arr[MAX_N];
    int cnt;
    bool deleted[MAX_N];
    
    MinHeap() {
        cnt = 0;
    }
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = i >> 1; j; i = j, j >>= 1) {
            if (arr[j] > arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else {
                break;
            }
        }
    }
    
    int top() {
        while (cnt > 0 && deleted[cnt]) {
            cnt--;
        }
        if (cnt == 0) return -1;
        return arr[1];
    }
    
    void pop() {
        if (cnt == 0) return;
        deleted[1] = true;
        // We don't actually remove, just mark as deleted
    }
    
    bool empty() {
        return top() == -1;
    }
};

struct MaxHeap {
    int arr[MAX_N];
    int cnt;
    bool deleted[MAX_N];
    
    MaxHeap() {
        cnt = 0;
    }
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = i >> 1; j; i = j, j >>= 1) {
            if (arr[j] < arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else {
                break;
            }
        }
    }
    
    int top() {
        while (cnt > 0 && deleted[cnt]) {
            cnt--;
        }
        if (cnt == 0) return -1;
        return arr[1];
    }
    
    void pop() {
        if (cnt == 0) return;
        deleted[1] = true;
    }
    
    bool empty() {
        return top() == -1;
    }
};

// Simple set implementation using sorted array
struct RequestSet {
    int requests[MAX_N];
    int size;
    
    RequestSet() {
        size = 0;
    }
    
    // Binary search for position
    int find(int x) {
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (requests[mid] == x) return mid;
            if (requests[mid] < x) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }
    
    // Find insertion position
    int lower_bound(int x) {
        int left = 0, right = size;
        while (left < right) {
            int mid = (left + right) / 2;
            if (requests[mid] < x) left = mid + 1;
            else right = mid;
        }
        return left;
    }
    
    void add(int x) {
        int pos = lower_bound(x);
        // Shift elements to the right
        for (int i = size; i > pos; i--) {
            requests[i] = requests[i-1];
        }
        requests[pos] = x;
        size++;
    }
    
    void remove(int x) {
        int pos = find(x);
        if (pos == -1) return;
        // Shift elements to the left
        for (int i = pos; i < size - 1; i++) {
            requests[i] = requests[i+1];
        }
        size--;
    }
    
    bool empty() {
        return size == 0;
    }
    
    int nearest_above(int current) {
        int pos = lower_bound(current + 1);  // First floor > current
        if (pos >= size) return -1;
        return requests[pos];
    }
    
    int nearest_below(int current) {
        int pos = lower_bound(current);  // First floor >= current
        if (pos == 0) return -1;
        return requests[pos - 1];  // Last floor < current
    }
};

int main() {
    int n;
    cin >> n;
    
    // Elevator state
    int current_floor = 0;
    bool going_up = true;
    
    // Set to store pending requests
    RequestSet requests;
    
    for (int i = 0; i < n; i++) {
        char op[10];
        cin >> op;
        
        if (op[0] == 'a' && op[1] == 'c') {  // action
            if (requests.empty()) {
                // No requests, stay in place
                continue;
            }
            
            int next_floor;
            
            // Find next floor based on current direction
            if (going_up) {
                // Look for requests above current floor
                next_floor = requests.nearest_above(current_floor);
                if (next_floor != -1) {
                    // Found request above
                } else {
                    // No requests above, reverse direction and find nearest below
                    going_up = false;
                    next_floor = requests.nearest_below(current_floor);
                }
            } else {
                // Going down, look for requests below current floor
                next_floor = requests.nearest_below(current_floor);
                if (next_floor != -1) {
                    // Found request below
                } else {
                    // No requests below, reverse direction and find nearest above
                    going_up = true;
                    next_floor = requests.nearest_above(current_floor);
                }
            }
            
            // Move elevator to next floor
            current_floor = next_floor;
            requests.remove(next_floor);
            
        } else if (op[0] == 'a') {  // add
            int floor;
            cin >> floor;
            requests.add(floor);
            
        } else if (op[0] == 'c') {  // cancel
            int floor;
            cin >> floor;
            requests.remove(floor);
            
        } else if (op[0] == 'l') {  // locate
            cout << current_floor << endl;
        }
    }
    
    return 0;
}