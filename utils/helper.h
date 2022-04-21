//
// Created by Peter Li on 4/13/22.
//

#ifndef FLIPDISTANCE_HELPER_H
#define FLIPDISTANCE_HELPER_H

template <typename T>
size_t findNext(const std::vector<T> &l, T a, T b, size_t start) {
    int counter = 1;
    while (counter) {
        if (l[start] == a) {
            counter++;
        } else if (l[start] == b) {
            counter--;
        }
        start++;
    }
    return start - 1;
}

#endif //FLIPDISTANCE_HELPER_H
