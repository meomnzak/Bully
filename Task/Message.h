

#ifndef TASK_MESSAGE_H
#define TASK_MESSAGE_H

#include<bits/stdc++.h>

using namespace std;

class Message {
private:
    bool isCoordinator;
    int curTime;
    bool makeComputation;
    int arrSz;
    vector<int> arr;
public:
    Message(bool isCoordinator, int curTime, bool makeComputation, int arrSz, vector<int> arr);

    bool getIsCoordinator() const;

    void setIsCoordinator(bool isCoordinator);

    int getCurTime() const;

    int getArrSz();

    bool getMakeComputation();

    void setMakeComputation(bool MakeComputation);

    void setArrSz(int arrSz);

    vector<int> getArr();

    void setArr(vector<int> arr);

    void setCurTime(int curTime);

    Message(string s);

    string encodeMessage();

};


#endif //TASK_MESSAGE_H