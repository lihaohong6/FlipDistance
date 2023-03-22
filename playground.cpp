//
// Created by Peter Li on 6/8/22.
//

#include "triangulation/Helper.h"
#include <cstdio>

int main() {
    printf("%s", BinaryString(treeStringToParentheses("a((bc)d)")).toDyckPath().c_str());
}
