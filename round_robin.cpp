#include <bits/stdc++.h>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
};

int main() {
    int n, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    
    cout << "Enter time quantum: ";
    cin >> quantum;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter Arrival Time and Burst Time for P" << i+1 << ": ";
        cin >> p[i].arrival >> p[i].burst;
        p[i].pid = i + 1;
        p[i].remaining = p[i].burst;
    }

    queue<int> q;
    vector<int> gantt_process;
    vector<int> gantt_time;

    int time = 0;
    int completed = 0;
    vector<bool> inQueue(n, false);

    // Sort by arrival time
    sort(p.begin(), p.end(), [](Process &a, Process &b){
        return a.arrival < b.arrival;
    });

    int idx = 0;

    while (completed < n) {
        // Add all arriving processes
        while (idx < n && p[idx].arrival <= time) {
            q.push(idx);
            inQueue[idx] = true;
            idx++;
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        // Add Gantt chart entry
        gantt_process.push_back(p[i].pid);
        gantt_time.push_back(time);

        int execute = min(quantum, p[i].remaining);
        p[i].remaining -= execute;
        time += execute;

        // Check for new arrivals during execution
        while (idx < n && p[idx].arrival <= time) {
            q.push(idx);
            inQueue[idx] = true;
            idx++;
        }

        if (p[i].remaining == 0) {
            p[i].completion = time;
            completed++;
        } else {
            q.push(i);
        }
    }

    gantt_time.push_back(time);

    cout << "\n\n===== ROUND ROBIN SCHEDULING RESULT =====\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";

    for (auto &x : p) {
        x.turnaround = x.completion - x.arrival;
        x.waiting = x.turnaround - x.burst;

        cout << "P" << x.pid << "\t" << x.arrival << "\t" << x.burst << "\t"
             << x.completion << "\t" << x.turnaround << "\t" << x.waiting << "\n";
    }

    cout << "\n===== GANTT CHART =====\n";
    for (int gp : gantt_process) {
        cout << "| P" << gp << " ";
    }
    cout << "|\n";

    for (int t : gantt_time) {
        cout << t << "   ";
    }
    cout << endl;

    return 0;
}
