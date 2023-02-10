#include "Process.h"
#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 512
#pragma comment(lib, "user32.lib")

Process::Process(string id) {
    this->id = id;
    hMapFile = nullptr;
    pBuf = nullptr;
}

bool Process::openMapping() {
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            id.c_str());               // name of mapping object

    if (hMapFile == nullptr) {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
                 GetLastError());
        return false;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0,
                                  0,
                                  BUF_SIZE);

    if (pBuf == nullptr) {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
                 GetLastError());

        CloseHandle(hMapFile);

        return false;
    }
    this->pBuf = pBuf;
    this->hMapFile = hMapFile;
    return true;

}

void Process::writeMessage(string msg) {
    CopyMemory((PVOID) pBuf, msg.c_str(), (_tcslen(msg.c_str()) * sizeof(TCHAR)));
}

string Process::readMessage() {
    string s = string(pBuf);
    return s;
}

bool Process::isAlive(){
    return hMapFile != nullptr;
}
