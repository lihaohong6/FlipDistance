//
// Created by Peter Li on 6/8/22.
//

#include "utils/rand.h"

int main() {
    int r = 100;
    while (r--) {
        auto p = randomTriangulation(24);
        auto sources = p.first.getSources();
        printf("%lu\n", sources.size());
    }
}
