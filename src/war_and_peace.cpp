#include "../include/war_and_peace.h"
#include <algorithm>


int main() {
    const auto book = readFile("data/war_and_peace.txt");
    const auto warTerms = readFile("data/war_terms.txt");
    const auto peaceTerms = readFile("data/peace_terms.txt");

    const auto tokenizedChapters = tokenizeChapters(book);

    const auto densities = calculateDensities(tokenizedChapters, warTerms, peaceTerms);

    if (densities.first.size() != tokenizedChapters.size() || densities.second.size() != tokenizedChapters.size()) {
        printErrToConsole("Error: Size mismatch in densities and tokenizedChapters.\n");
        return EXIT_FAILURE;
    }

    const auto chapterCategories = categorizeChapters(densities);

    printChapters(chapterCategories);

    return 0;
}