
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct EventoTTL {
    string clave;
    time_t expira;
    unsigned long long version;
};

class MinHeap {
private:
    vector<EventoTTL> heap;

    int parent(int i);
    int left(int i);
    int right(int i);

    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    MinHeap();

    void insert(EventoTTL evento);
    EventoTTL extractMin();

    EventoTTL getMin();
    bool empty();
    int size();
};
