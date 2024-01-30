#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <execution>


const auto readFile = [](const std::string &fileName) -> std::vector<std::string> {
    std::ifstream file(fileName);
    std::vector<std::string> content;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(file, line)) {
        content.push_back(line);
    }

    return content;
};

const auto tokenize = [](const std::string &text) -> std::vector<std::string> {
    std::vector<std::string> words;
    std::istringstream iss(text);

    // tokenize the text, remove punctuation and make all characters lowercase
    std::transform(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                   std::back_inserter(words),
                   [](const std::string &word) {
                       std::string result;
                       std::copy_if(word.begin(), word.end(), std::back_inserter(result), [](char c) {
                           return std::isalpha(c);
                       });
                       std::transform(result.begin(), result.end(), result.begin(), [](char c) {
                           return std::tolower(c);
                       });
                       return result;
                   });

    return words;
};

const auto filterWords = [](const std::vector<std::string> &words,
                            const std::vector<std::string> &filterList) -> std::vector<std::string> {
    std::vector<std::string> filteredWords;

    // maybe use a different filter instead of substrings
    std::copy_if(words.begin(), words.end(), std::back_inserter(filteredWords), [&](const std::string &word) {
        return std::any_of(filterList.begin(), filterList.end(), [&](const std::string &term) {
            return word.find(term) != std::string::npos;
        });
    });

    return filteredWords;
};


const auto countOccurrences = [](const std::vector<std::string> &words) -> std::unordered_map<std::string, int> {
    std::unordered_map<std::string, int> wordCount;

    std::for_each(std::execution::par, words.begin(), words.end(), [&](const std::string &word) {
        wordCount[word]++;
    });

    return wordCount;
};

const auto calculateDistances = [](const std::vector<std::string> &text,
                                   const std::vector<std::string> &filteredWords) -> std::vector<int> {
    std::vector<int> distances;
    int currentDistance = 0;

    std::transform(text.begin(), text.end(), std::back_inserter(distances), [&](const std::string &word) {
        if (std::find(filteredWords.begin(), filteredWords.end(), word) != filteredWords.end()) {
            int distance = currentDistance;
            currentDistance = 0;
            return distance;
        } else {
            currentDistance++;
            return 0;
        }
    });

    if (!filteredWords.empty() && distances.size() < filteredWords.size()) {
        distances.push_back(currentDistance);
    }

    return distances;
};

const auto calculateTotalOccurrences = [](const std::vector<std::string> &terms,
                                          const std::unordered_map<std::string, int> &termCounts) -> double {
    return std::accumulate(terms.begin(), terms.end(), 0.0, [&termCounts](double sum, const std::string &term) {
        auto it = termCounts.find(term);
        return sum + (it != termCounts.end() ? it->second : 0);
    });
};

const auto calculateTermDensity = [](const std::vector<std::string> &text,
                                     const std::vector<std::string> &terms) -> double {
    const auto filteredWords = filterWords(text, terms);
    const auto distances = calculateDistances(text, filteredWords);
    const auto termCounts = countOccurrences(filteredWords);

    const int totalDistance = std::accumulate(distances.begin(), distances.end(), 0);
    const double relativeDistance = static_cast<double>(totalDistance) / text.size();

    const double totalOccurrences = calculateTotalOccurrences(terms, termCounts);

    return (totalOccurrences + relativeDistance) / text.size();
};

const auto tokenizeChapters = [](const std::vector<std::string> &book) -> std::vector<std::vector<std::string>> {
    std::vector<std::vector<std::string>> chapters;
    std::vector<std::string> currentChapter;
    bool insideChapter = false;

    std::for_each(book.begin(), book.end(), [&](const std::string &line) {
        if (line.find("CHAPTER") != std::string::npos) {
            insideChapter = true;
            if (!currentChapter.empty()) {
                chapters.push_back(currentChapter);
                currentChapter.clear();
            }
        } else if (insideChapter && !line.empty()) {
            if (line.find("*** END OF THE PROJECT GUTENBERG EBOOK, WAR AND PEACE ***") != std::string::npos) {
                return;
            }

            auto words = tokenize(line);
            std::copy(words.begin(), words.end(), std::back_inserter(currentChapter));
        }
    });

    if (!currentChapter.empty()) {
        chapters.push_back(currentChapter);
    }

    return chapters;
};

const auto calculateDensities = [](const std::vector<std::vector<std::string>> &tokenizedChapters,
                                   const std::vector<std::string> &warTerms,
                                   const std::vector<std::string> &peaceTerms) -> std::pair<std::vector<double>, std::vector<double>> {

    std::vector<double> warDensities;
    std::vector<double> peaceDensities;

    std::for_each(std::execution::par, tokenizedChapters.begin(), tokenizedChapters.end(),
                  [&](const std::vector<std::string> &chapter) {
                      double warDensity = calculateTermDensity(chapter, warTerms);
                      double peaceDensity = calculateTermDensity(chapter, peaceTerms);

                      warDensities.push_back(warDensity);
                      peaceDensities.push_back(peaceDensity);
                  });

    return std::make_pair(warDensities, peaceDensities);
};

const auto categorizeChapters = [](const std::pair<std::vector<double>, std::vector<double>> &densities) -> std::vector<std::string> {
    std::vector<std::string> categories;

    std::transform(densities.first.begin(), densities.first.end(), densities.second.begin(),
                   std::back_inserter(categories),
                   [](double warDensity, double peaceDensity) {
                       return warDensity > peaceDensity ? "War-related" : "Peace-related";
                   });

    return categories;
};

auto printChapter = [](const std::string &category, size_t index) {
    std::cout << "Chapter " << index + 1 << ": " << category << std::endl;
};

auto printChapters = [](const std::vector<std::string> &chapterCategories) {
    size_t index = 0;
    std::for_each(chapterCategories.begin(), chapterCategories.end(),
                  [&index](const std::string &category) {
                      printChapter(category, index++);
                  });
};

auto printErrToConsole = [](const std::string &error) {
    std::cerr << error << std::endl;
};