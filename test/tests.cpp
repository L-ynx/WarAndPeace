#include "../include/doctest.h"
#include "../include/war_and_peace.h"

TEST_CASE("Tokenize Chapters Function") {
    const std::vector<std::string> book = {"CHAPTER 1", "This is chapter 1.", "CHAPTER 2", "This is chapter 2."};
    const auto tokenizedChapters = tokenizeChapters(book);
    REQUIRE(tokenizedChapters.size() == 2);
    REQUIRE(tokenizedChapters[0].size() == 4);
    REQUIRE(tokenizedChapters[1].size() == 4);
}

TEST_CASE("Read File Function") {
    const std::string fileName = "data/test.txt";
    const auto content = readFile(fileName);

    REQUIRE(content.size() == 3);
    CHECK(content[0] == "Line 1");
    CHECK(content[1] == "Line 2");
    CHECK(content[2] == "Line 3");
}

TEST_CASE("Tokenize Function") {
    const std::string text = "This is a test sentence.";
    const auto tokens = tokenize(text);

    REQUIRE(tokens.size() == 5);
    CHECK(tokens[0] == "this");
    CHECK(tokens[1] == "is");
    CHECK(tokens[2] == "a");
    CHECK(tokens[3] == "test");
    CHECK(tokens[4] == "sentence");
}

TEST_CASE("Filter Words Function - War-related Terms") {
    const std::vector<std::string> words = {"hope", "love", "war", "peace", "struggle"};
    const std::vector<std::string> filterList = readFile("data/war_terms.txt");

    const auto filteredWords = filterWords(words, filterList);

    REQUIRE(filteredWords.size() == 2);
    CHECK(filteredWords[0] == "war");
    CHECK(filteredWords[1] == "struggle");
}

TEST_CASE("Filter Words Function - Peace-related Terms") {
    const std::vector<std::string> words = {"hope", "love", "war", "peace", "struggle"};
    const std::vector<std::string> filterList = readFile("data/peace_terms.txt");

    const auto filteredWords = filterWords(words, filterList);

    REQUIRE(filteredWords.size() == 2);
    CHECK(filteredWords[0] == "love");
    CHECK(filteredWords[1] == "peace");
}

TEST_CASE("Count Occurrences Function") {
    const std::vector<std::string> words = {"war", "dark", "light", "war", "hope"};
    const auto wordCount = countOccurrences(words);

    REQUIRE(wordCount.size() == 4);
    CHECK(wordCount.at("war") == 2);
    CHECK(wordCount.at("dark") == 1);
    CHECK(wordCount.at("light") == 1);
    CHECK(wordCount.at("hope") == 1);
}

TEST_CASE("Calculate Densities Function") {
    const std::vector<std::vector<std::string>> tokenizedChapters = {
            {"word", "word", "war",   "war", "peace"},
            {"word", "war",  "peace", "war", "peace", "word"}
    };
    const std::vector<std::string> warTerms = {"war"};
    const std::vector<std::string> peaceTerms = {"peace"};

    const auto densities = calculateDensities(tokenizedChapters, warTerms, peaceTerms);

    REQUIRE(densities.first.size() == 2);
    REQUIRE(densities.second.size() == 2);

    CHECK(densities.first[0] == 0.48);
    CHECK(densities.second[0] == 0.36);
}

TEST_CASE("Categorize Chapters Function") {
    const std::vector<double> warDensities = {0.4, 0.2, 0.6};
    const std::vector<double> peaceDensities = {0.3, 0.5, 0.2};
    const std::pair<std::vector<double>, std::vector<double>> densities = std::make_pair(warDensities, peaceDensities);

    const auto categories = categorizeChapters(densities);

    REQUIRE(categories.size() == 3);
    CHECK(categories[0] == "War-related");
    CHECK(categories[1] == "Peace-related");
    CHECK(categories[2] == "War-related");
}

TEST_CASE("Calculate Total Occurrences Function") {
    const std::vector<std::string> terms = {"war", "love", "hate"};
    const std::unordered_map<std::string, int> termCounts = {{"war",    3},
                                                             {"love",   1},
                                                             {"honest", 2}};

    const double totalOccurrences = calculateTotalOccurrences(terms, termCounts);

    CHECK(totalOccurrences == 4.0);
}

