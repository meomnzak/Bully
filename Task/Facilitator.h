
#ifndef TASK_FACILITATOR_H
#define TASK_FACILITATOR_H

#include <bits/stdc++.h>
#include <winnt.h>
#include <afxres.h>

using namespace std;

class Facilitator {

private:
    struct processInfo {
        HANDLE hMapFile;
        LPCTSTR pBuf;
        PROCESS_INFORMATION pi;
    };

    int globalClock;
    map<string, processInfo> aliveProcesses;
    string id;
    string curCoordinator;
public:

    Facilitator();

    int getClock() const;

    bool addProcess();

    bool shutProcess(string id);

    void writeMessage(string id, string msg);

    string readMessage(string id);

    string startElection();

    void checkProcesses();

    void updateClock();

    void distributedComputing();

    void shutAllProcesses();

    PROCESS_INFORMATION createProcess(string id);
};


#endif
