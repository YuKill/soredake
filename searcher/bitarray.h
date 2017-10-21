#ifndef BITARRAY_H
#define BITARRAY_H

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>

using namespace std;

#define MAX_BITS 32
#define MAX_BITS 32

unsigned get_uarray_buckets(int n) {
    const unsigned base = n / MAX_BITS;
    return (n % MAX_BITS) ? base + 1 : base;
}

void init_uarray(unsigned *arr, int n, bool init) {
    memset(arr, 0, get_uarray_buckets(n) * sizeof(unsigned));

    --n;

    const unsigned div = n / MAX_BITS;
    const unsigned mod = n % MAX_BITS;

    int init_val = 0;
    if (init) init_val = (1 << 8) - 1;

    if (div) memset(arr, init_val, div * sizeof(unsigned));
    if (init_val) arr[div] = (((1 << mod) - 1) << 1) + 1;
    else arr[div] = 0;
}

bool get_uarray_bit(unsigned *arr, int i) {
    const unsigned div = i / MAX_BITS;
    const unsigned mod = i % MAX_BITS;
    return arr[div] & (1 << mod);
}

void set_uarray_bit(unsigned *arr, int i, bool lval) {
    const unsigned div = i / MAX_BITS;
    const unsigned mod = i % MAX_BITS;

    unsigned val = ((unsigned) -1) - (1 << mod);
    if (lval) arr[div] |= (1 << mod);
    else arr[div] &= (((unsigned) -1) - (1 << mod));
}

void land_uarray(unsigned *arr1, unsigned *arr2, unsigned *arr3, int n) {
    const unsigned buckets = get_uarray_buckets(n);

    for (int i = 0, e = buckets; i < e; ++i) {
        arr3[i] = arr1[i] & arr2[i];
    }
}

bool has_onebit_uarray(unsigned *arr, int n) {
    const unsigned buckets = get_uarray_buckets(n);

    for (int i = 0, e = buckets; i < e; ++i) {
        if (arr[i]) return true;
    }

    return false;
}

void print_uarray(unsigned *arr, int n) {
    for (int i = 0; i < n; ++i)
        cout << get_uarray_bit(arr, i);
    cout << endl;
}

#endif
