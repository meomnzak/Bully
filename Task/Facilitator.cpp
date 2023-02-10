

#include "Facilitator.h"
#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <tchar.h>
#include <bits/stdc++.h>
#include "Message.h"

#define BUF_SIZE 512

const CHAR *PATH = "Task.exe";

Facilitator::Facilitator() {
    aliveProcesses.clear();
    id = "0";
    globalClock = 0;
}

PROCESS_INFORMATION Facilitator::createProcess(string id) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    string s = PATH;
    s += " ";
    s += id;
    char *cmdArgs = const_cast<char *>(s.c_str());

    // Start the child process.
    if (!CreateProcess(PATH,   // No module name (use command line)
                       cmdArgs,        // Command line
                       nullptr,           // Process handle not inheritable
                       nullptr,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       nullptr,           // Use parent's environment block
                       nullptr,           // Use parent's starting directory
                       &si,            // Pointer to STARTUPINFO structure
                       &pi)           // Pointer to PROCESS_INFORMATION structure
            ) {
        printf("CreateProcess failed (%d).\n", GetLastError());
    }

    return pi;

}

bool Facilitator::addProcess() {
    HANDLE hMapFile;
    LPCTSTR pBuf;
    int intId = stoi(id);
    intId++;
    id = to_string(intId);

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            nullptr,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            BUF_SIZE,                // maximum object size (low-order DWORD)
            id.c_str());                 // name of mapping object

    if (hMapFile == nullptr) {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
                 GetLastError());
        return false;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                                  FILE_MAP_ALL_ACCESS, // read/write permission
                                  0,
                                  0,
                                  BUF_SIZE);

    if (pBuf == nullptr) {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
                 GetLastError());

        CloseHandle(hMapFile);

        return false;
    }

    auto pi = createProcess(id);

    aliveProcesses[id] = {hMapFile, pBuf, pi};

    cout << "Process with id: " + id + " has started" << endl;

    updateClock();
    Message msg(false, getClock()+1, false, 0, {});  // the reason of the plus one here because there could be a situation when after say millisecond this will be updated so the new process will have older time than others

    writeMessage(id, msg.encodeMessage());

    startElection();

    return true;

}

bool Facilitator::shutProcess(string id) {
    if (!aliveProcesses.count(id)) return false;
    UnmapViewOfFile(aliveProcesses[id].pBuf);
    CloseHandle(aliveProcesses[id].hMapFile);
    TerminateProcess(aliveProcesses[id].pi.hProcess, 1);
    CloseHandle(aliveProcesses[id].pi.hProcess);
    CloseHandle(aliveProcesses[id].pi.hThread);
    aliveProcesses.erase(id);
    cout << "Process with id: " + id + " has been terminated" << endl;
    if (id == curCoordinator) {
        curCoordinator = "";
    }
    return true;
}

void Facilitator::writeMessage(string id, string msg) {
    CopyMemory((PVOID) aliveProcesses[id].pBuf, msg.c_str(), (_tcslen(msg.c_str()) * sizeof(TCHAR)));
}

string Facilitator::readMessage(string id) {
    string s = string(aliveProcesses[id].pBuf);
    return s;
}

void Facilitator::distributedComputing() {
    string s = readMessage(curCoordinator);
    Message msg(s);
    srand(time(0));
    msg.setArrSz((rand() % 10) + 5);
    msg.setMakeComputation(1);
    writeMessage(curCoordinator, msg.encodeMessage());            // make a command to the coordinator to create a random array then wait
    Sleep(2000);
    Message msgAfter(readMessage(curCoordinator));
    cout << msgAfter.encodeMessage() << endl;
    vector<vector<int>> segments(aliveProcesses.size() - 1);
    vector<int> arr = msgAfter.getArr();
    int n = msg.getArrSz();
    int sz = n / (aliveProcesses.size() - 1);
    for (int i = 0; i < n; i++) {                                   // dividing the array from coordinator between the other processes
        int idx = i / sz;
        if (idx == segments.size()) {                               // when the size of the array is not divisible by the number of processes the last one will get the remaning
            idx--;
        }
        segments[idx].push_back(arr[i]);
    }

    for (int i = 0; i < segments.size(); i++) {
        for (int j = 0; j < segments[i].size(); j++) {
            cout << segments[i][j] << " ";
        }
        cout << endl;
    }

    int i = 0;

    for (auto elem: aliveProcesses) {                        // put every segment in the corresponding shared memory then wait
        if (elem.first == curCoordinator) continue;
        string s = readMessage(elem.first);
        Message msg(s);
        msg.setArrSz(segments[i].size());
        msg.setArr(segments[i]);
        msg.setMakeComputation(1);
        cout << elem.first << "-> ";
        for (int k = 0; k < segments[i].size(); k++) {
            cout << segments[i][k] << " ";
        }
        cout << endl;
        i++;
        writeMessage(elem.first, msg.encodeMessage());
    }

    Sleep(2000);
    int ans = INT_MAX;
    for (auto elem: aliveProcesses) {                          // get the min from every process then decide the final answer
        string s = readMessage(elem.first);
        Message msg(s);
        if (msg.getArrSz() == 0) continue;
        if (elem.first != curCoordinator) {
            ans = min(ans, msg.getArr()[0]);
            cout << "The minimum element in process " << elem.first << " is " << msg.getArr()[0] << endl;
        }
        msg.setArr({});
        msg.setArrSz(0);
        writeMessage(elem.first, msg.encodeMessage());

    }
    cout << "The minimum element is " << ans << endl;
}

string Facilitator::startElection() {
    cout << "Election has started" << endl << "The available processes are : ";
    for (const auto &elem: aliveProcesses) {
        cout << elem.first << " ";
    }
    cout << endl;
    string oldCoordinator = curCoordinator;
    if (!oldCoordinator.empty()) {                                   // make sure to drop the old coordinator
        string s = readMessage(oldCoordinator);
        Message msg(s);
        msg.setIsCoordinator(false);
        writeMessage(oldCoordinator, msg.encodeMessage());
    }
    curCoordinator = aliveProcesses.rbegin()->first;                           // the map is sorted so the last element is the coordinator
    cout << "The winner of the election is " + curCoordinator << endl;
    string s = readMessage(curCoordinator);

    Message msg(s);
    msg.setIsCoordinator(true);
    writeMessage(curCoordinator, msg.encodeMessage());
//    if (aliveProcesses.size() > 1)                          // the logic of the bonus feature which is commented
//        distributedComputing();
    return curCoordinator;
}

int Facilitator::getClock() const {
    return globalClock;
}

void Facilitator::updateClock() {
    if (!aliveProcesses.count(curCoordinator)) return;
    Message msg(readMessage(curCoordinator));
    globalClock = msg.getCurTime();
}

void Facilitator::checkProcesses() {
    int maxTime = 0;
    for (const auto &elem: Facilitator::aliveProcesses) {             // go through every process and check its time
        Message msg(readMessage(elem.first));
        maxTime = max(maxTime, msg.getCurTime());
        cout << "The time of process with id " << elem.first << " is " << msg.getCurTime() << endl;
    }
    updateClock();                                                   // update the global time in the facilitator with the one in the coordinator
    if (maxTime > globalClock) {
        cout << "The coordinator is down" << endl;
        startElection();
    }
}

void Facilitator::shutAllProcesses() {
    vector<string> processes;
    for (auto elem : aliveProcesses)
        processes.push_back(elem.first);

    for (auto process: processes)
        shutProcess(process);
}