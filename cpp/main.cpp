#include "include.h"
#include "bin_search.h"
#include "mysort.h"


int main()
{
    vector<int> a{ 1,-2,30,0,-5,6,97,9,4 };

    quicksort(a, 0, a.size() - 1);

    print_vector(a);

    return 0;
}