#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include "HashTable.hpp"


int main(int argc, char* argv[]) {

    size_t window = 100;
    size_t kTop = 100;
    size_t nDocumentsToVisualize = 10;
    size_t capacity = 100000;
    uint64_t base = 67;
    double maxCollisionFactor = 0.3;

    if (argc > 1) window = std::stoull(argv[1]);
    if (argc > 2) kTop = std::stoull(argv[2]);
    if (argc > 3) nDocumentsToVisualize = std::stoull(argv[3]);
    if (argc > 4) capacity = std::stoull(argv[4]);
    if (argc > 5) base = std::stoull(argv[5]);
    if (argc > 6) maxCollisionFactor = std::stod(argv[6]);

    std::cout << "Initializing HashTable\n";

    HashTable ht1(window, kTop, nDocumentsToVisualize, capacity, base, maxCollisionFactor);

    std::string folderPath = "pre-processedData/documents/";
    size_t fileCounter = 0;

    while (true) {
        std::string fileName = std::to_string(fileCounter) + ".csv";
        std::string fullPath = folderPath + fileName;

        if (std::filesystem::exists(fullPath)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            if (fileCounter%50==0) std::cout << "Found: " << fileName << ". Processing Document" << std::endl;
            
            ht1.insertDocument(fileName); 
            
            if (fileCounter%50==0) std::cout<< "Insert Document and Delete Document from the hash table time:\n  Only insert and delete: " <<ht1.timer.getTime()
                                            << "\n  Sorting after inserting and deleting: "<<ht1.timerWithSort.getTime() << std::endl;

            fileCounter++;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    return 0;
}