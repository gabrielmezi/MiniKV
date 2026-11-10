
#include "../include/MinHeap.h"
using namespace std;

int MinHeap::parent(int i) {
    return (i - 1) / 2;
}

int MinHeap::left(int i) {
    return 2 * i + 1;
}

int MinHeap::right(int i) {
    return 2 * i + 2;
}

void MinHeap::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)].expira > heap[i].expira) {
        swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::heapifyDown(int i) {
    int n = heap.size();

    while (left(i) < n) {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < n && heap[l].expira < heap[smallest].expira)
            smallest = l;

        if (r < n && heap[r].expira < heap[smallest].expira)
            smallest = r;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
        else {
            break;
        }
    }
}

MinHeap::MinHeap() {
}

void MinHeap::insert(EventoTTL evento) {
    heap.push_back(evento);
    heapifyUp(heap.size() - 1);
}

EventoTTL MinHeap::extractMin() {
    if (heap.empty()) {
        throw runtime_error("Heap vacio");
    }

    EventoTTL minVal = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
        heapifyDown(0);

    return minVal;
}

EventoTTL MinHeap::getMin() {
    if (heap.empty()) {
        throw runtime_error("Heap vacio");
    }

    return heap[0];
}

bool MinHeap::empty() {
    return heap.empty();
}

int MinHeap::size() {
    return heap.size();
}
