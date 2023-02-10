#ifndef STARTPROCESS_PROCESS_H
#define STARTPROCESS_PROCESS_H

#include <bits/stdc++.h>
#include <winnt.h>

using namespace std;

class Process {
private:
    string id;
    HANDLE hMapFile;
    LPCTSTR pBuf;
public:

    Process(string id);

    bool openMapping();

    string readMessage();

    void writeMessage(string msg);

    bool isAlive();

};


#endif
