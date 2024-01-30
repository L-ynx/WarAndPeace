#define DOCTEST_CONFIG_IMPLEMENT

#include "../include/doctest.h"

int main(int argc, char *argv[]) {
    doctest::Context context(argc, argv);
    int result = context.run();

    if (context.shouldExit())
        return result;

    return result;
}
