//
// Created by Peter Li on 6/8/22.
//

#include "utils/rand.h"

int main() {
    auto start = clock();
    auto p = randomTriangulation(14);
    auto end = clock();
    printf("%zu %f", p.first.getSize(), (double)(end-start) / CLOCKS_PER_SEC);
}
