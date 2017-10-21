#include "bitarray.h"

int main() {
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 27;
        unsigned buckets = get_uarray_buckets(n);
        unsigned arr[buckets];
        init_uarray(arr, n, false);
        print_uarray(arr, n);
    }

    {
        cout << "=-----------------------------------------=" << endl;
        int n = 27;
        unsigned buckets = get_uarray_buckets(n);
        unsigned arr[buckets];
        init_uarray(arr, n, true);
        print_uarray(arr, n);
    }
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 69;
        unsigned buckets = get_uarray_buckets(n);
        unsigned arr[buckets];
        init_uarray(arr, n, false);
        print_uarray(arr, n);
    }

    {
        cout << "=-----------------------------------------=" << endl;
        int n = 69;
        unsigned buckets = get_uarray_buckets(n);
        unsigned arr[buckets];
        init_uarray(arr, n, true);
        print_uarray(arr, n);
    }
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 69;
        unsigned buckets = get_uarray_buckets(n);

        unsigned arr1[buckets];
        init_uarray(arr1, n, false);

        unsigned arr2[buckets];
        init_uarray(arr2, n, true);

        print_uarray(arr1, n);
        print_uarray(arr2, n);
        land_uarray(arr1, arr2, arr2, n);
        print_uarray(arr2, n);
    }
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 99;
        unsigned buckets = get_uarray_buckets(n);

        unsigned arr1[buckets];
        init_uarray(arr1, n, false);

        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr1, i, true);

        unsigned arr2[buckets];
        init_uarray(arr2, n, true);

        print_uarray(arr1, n);
        print_uarray(arr2, n);
        land_uarray(arr1, arr2, arr2, n);
        print_uarray(arr2, n);
    }
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 120;
        unsigned buckets = get_uarray_buckets(n);

        unsigned arr1[buckets];
        init_uarray(arr1, n, false);

        unsigned arr2[buckets];
        init_uarray(arr2, n, true);

        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr1, i, true);

        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr2, i, false);

        print_uarray(arr1, n);
        print_uarray(arr2, n);
        land_uarray(arr1, arr2, arr2, n);
        print_uarray(arr2, n);
    }
    {
        cout << "=-----------------------------------------=" << endl;
        int n = 120;
        unsigned buckets = get_uarray_buckets(n);

        unsigned arr1[buckets];
        init_uarray(arr1, n, false);

        unsigned arr2[buckets];
        init_uarray(arr2, n, true);

        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr1, i, true);
        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr1, i, true);

        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr2, i, false);
        for (int i = 0; i < n; i += 2)
            set_uarray_bit(arr2, i, false);

        print_uarray(arr1, n);
        print_uarray(arr2, n);
        land_uarray(arr1, arr2, arr2, n);
        print_uarray(arr2, n);
    }
}
