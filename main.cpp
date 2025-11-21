#include <string>

#include "HashTable.hpp"

namespace fs = std::filesystem;

#include <thread>
#include <chrono>


int main() {
    std::string folderPath = "pre-processedData/documents/";

    // Create your ht1 table
    HashTable ht1(1000,100,1,100000,67,0.3);

    size_t c=0;

    // Iterate over all files in folder
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) 
            continue;

        std::string filePath = entry.path().string();
        std::ifstream inFile(filePath);

        std::string fileName = entry.path().filename().string();

        if (!inFile.is_open()) {
            std::cerr << "Failed to open file: " << filePath << "\n";
            continue;
        }

        //std::cout << "Processing file: " << fileName << "\n";

        //std::cout<<fileName<<std::endl;

        ht1.insertDocument(fileName);

        c++;

        inFile.close();

        // ht1.showData();

        // ht1.showIndexWithData();

        // ht1.showHead();

        // ht1.showTail();

        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    }

    std::cout << c << " files processed and inserted into hash table.\n";

    std::cout << ht1.data->size <<" " << ht1.getOcuppiedSlots() << " " << ((double)ht1.data->size-(double)ht1.getOcuppiedSlots())/(double)ht1.data->size<<std::endl;

    // for (size_t i = 0 ; i<100;i++){
    //     ht1.insertDocument("Reuters971_freq.csv");
    // }
    
    // ht1.showData();

    // ht1.showIndexWithData();

    // ht1.showHead();

    // ht1.showTail();
    

    return 0;
}

// #include "HashTable.hpp"

// using namespace std;

// int main(){
//     HashTable ht1(10,1024);

//     ht1.insertWord("hola",15);

//     cout<< ht1.timer.getTime()<<endl;

//     ht1.insertWord("chau",16);

//     ht1.insertWord("hola",15);

//     ht1.insertWord("holaa",5);

//     ht1.eraseWord("chau",16);

//     ht1.eraseWord("hola",15);

//     ht1.eraseWord("holaa",5);

//     ht1.insertWord("owo",6);

//     ht1.insertWord("owo",6);

//     ht1.insertWord("owo",6);

//     ht1.insertWord("owo",6);

//     ht1.insertWord("uwu",2);

//     ht1.insertWord("uwu",200);

//     // ht1.eraseWord("owo",24);

//     // ht1.eraseWord("hola",15);

//     // ht1.eraseWord("uwu",202);

//     ht1.showData();

//     ht1.showIndexWithData();

//     ht1.showHead();

//     ht1.showTail();

//     return 0;
// }