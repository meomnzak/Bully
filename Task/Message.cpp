

#include "Message.h"

Message::Message(string s) {
    stringstream ss(s);
    string one, two, three, four, five;
    getline(ss, one, '|');
    getline(ss, two, '|');
    getline(ss, three, '|');
    getline(ss, four, '|');
    getline(ss, five);

    isCoordinator = stoi(one);
    curTime = stoi(two);
    makeComputation = stoi(three);
    arrSz = stoi(four);

    vector<int> vec;

    stringstream elements(five);
    string element;

    try {
        while (getline(elements, element, ',')) {
            vec.push_back(stoi(element));
        }
    } catch (exception &err) {
        cout << "Conversion failure: " << err.what() << endl;
    }

    arr = vec;
}

Message::Message(bool isCoordinator, int curTime, bool makeComputation, int arrSz, vector<int> arr) {
    this->isCoordinator = isCoordinator;
    this->curTime = curTime;
    this->makeComputation = makeComputation;
    this->arrSz = arrSz;
    this->arr = arr;
}

string Message::encodeMessage() {
    string s = to_string(isCoordinator);
    s += '|';
    s += to_string(curTime);
    s += '|';
    s += to_string(makeComputation);
    s += '|';
    s += to_string(arrSz);
    s += '|';
    for (auto elem: arr) {
        s += to_string(elem);
        s += ',';
    }

    return s;
}

bool Message::getMakeComputation() {
    return makeComputation;
}

void Message::setMakeComputation(bool makeComputation) {
    this->makeComputation = makeComputation;
}


bool Message::getIsCoordinator() const {
    return isCoordinator;
}

void Message::setIsCoordinator(bool isCoordinator) {
    this->isCoordinator = isCoordinator;
}

int Message::getCurTime() const {
    return curTime;
}

void Message::setCurTime(int curTime) {
    this->curTime = curTime;
}

int Message::getArrSz() {
    return arrSz;
}

void Message::setArrSz(int arrSz) {
    this->arrSz = arrSz;
}

vector<int> Message::getArr() {
    return arr;
}

void Message::setArr(vector<int> arr) {
    this->arr = arr;
}
