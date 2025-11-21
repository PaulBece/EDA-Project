#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "DoubleLinkedList.hpp"
#include "Hasher.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <chrono>

struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    void startTimer(){
        start=std::chrono::high_resolution_clock::now();
    }

    void stopTimer(){
        end = std::chrono::high_resolution_clock::now();
    }

    double getTime() {
        std::chrono::duration<double> diff = end - start;
        return diff.count();
    }
};


struct HashTable
{
    DoubleLinkedList< Pair<const std::string,size_t> > *data;
    Hasher hasher;
    DoubleLinkedList< Node<Pair<const std::string,size_t> >* > *keys;

    DoubleLinkedList<const std::string> documents;

    size_t capacity;
    size_t window;
    size_t outputCounter=0;
    size_t kTop;

    Timer timer;

    size_t nCollisions=0;

    bool collision=false;
    double maxCollisionFactor;
    size_t nDocumentsToVisualize;
    size_t nProcessedDocuments=0;

    HashTable(size_t window, size_t kTop=100, size_t nDocumentsToVisualize=1, size_t capacity=10000, uint64_t base=131, double maxCollisionFactor=0.3) : window(window), capacity(capacity), kTop(kTop), hasher(base), maxCollisionFactor(maxCollisionFactor), nDocumentsToVisualize(nDocumentsToVisualize){
        data=new DoubleLinkedList< Pair<const std::string,size_t> >;
        keys=new DoubleLinkedList< Node < Pair<const std::string,size_t> > *>[capacity];
    }

    void doubleCapacity(){
        nCollisions=0;
        capacity*=2;
        delete[] keys;
        keys=new DoubleLinkedList< Node < Pair<const std::string,size_t> > *>[capacity];
        Node<Pair<const std::string,size_t>>* currentNode=data->head;
        while (currentNode){
            size_t index = hasher.calculateKey(currentNode->data.first)%capacity;
            if (keys[index].size!=0) nCollisions++;
            keys[index].insert(currentNode);
            currentNode=currentNode->next;
        }
    }

    double getCollisionFactor(){
        return 1.0-((double)getOcuppiedSlots())/((double)data->size);
    }

    Pair<Node<Pair<const std::string,size_t>>*,size_t > find (const std::string &s, bool isInsert){
        size_t index = hasher.calculateKey(s)%capacity;
        collision=false;
        Node<Node< Pair<const std::string,size_t> >*>*  aux = keys[index].head;
        if (isInsert && aux) collision=true;
        else if (!isInsert && aux->next) collision=true;;
        while (aux){
            if (aux->data->data.first==s) return Pair<Node<Pair<const std::string,size_t>>*,size_t >(aux->data,index);
            aux=aux->next;
        }
        return Pair<Node<Pair<const std::string,size_t>>*,size_t > (0,index);
    }

    void sortData(Node<Pair<const std::string,size_t>>* node){
        Node<Pair<const std::string,size_t>>* aux = node;
        char flag = 'h';
        while (aux->next){
            if (node->data.second > aux->next->data.second){
                aux=aux->next;
                flag = 'r';
            }
            else {
                break;
            }
        }
        while (aux->prev){
            if (node->data.second < aux->prev->data.second){
                aux=aux->prev;
                flag = 'l';
            }
            else {
                break;
            }
        }
        switch (flag)
        {
        case 'h':
            break;

        case 'l':
            if (node->next){
                node->next->prev=node->prev;
                node->prev->next=node->next;
                node->prev=aux->prev;
                node->next=aux;
                if (node->prev){
                    node->prev->next=node;
                    node->next->prev=node;
                }
                else{
                    data->head=node;
                    node->next->prev=node;
                }
                
            }
            else{
                data->tail=node->prev;
                node->prev->next=node->next;
                node->prev=aux->prev;
                node->next=aux;
                if (node->prev){
                    node->prev->next=node;
                    node->next->prev=node;
                }
                else{
                    data->head=node;
                    node->next->prev=node;
                }
            }
            break;
        
        case 'r':
            if (aux==data->tail) data->tail=node;
            if (node->prev){
                node->prev->next=node->next;
                node->next->prev=node->prev;
                node->next=aux->next;
                node->prev=aux;
                if (node->next){
                    node->next->prev=node;
                    node->prev->next=node;
                }
                else{
                    
                    node->prev->next=node;
                }
            }
            else{
                data->head=node->next;
                node->next->prev=node->prev;
                node->next=aux->next;
                node->prev=aux;
                if (node->next){
                    node->next->prev=node;
                    node->prev->next=node;
                }
                else{
                    
                    node->prev->next=node;
                }
            }
            break;
        
        default:
            break;
        }
    }

    size_t getOcuppiedSlots(){
        return data->size-nCollisions;
    }

    void processDocument(const std::string& filename, bool isInsert){
        std::string path = "pre-processedData/documents/" + filename;
        std::ifstream file(path);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file: " << path << std::endl;
            return;
        }

        std::string line, word;
        std::string freqStr;
        size_t frequency;

        // Omitir la cabecera (asumiendo que tiene)
        std::getline(file, line); 

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            
            // Parsear: palabra,frecuencia
            std::getline(ss, word, ',');
            std::getline(ss, freqStr);
            
            // Convertir frecuencia a size_t
            // (std::stoull es más seguro para size_t)
            try {
                frequency = std::stoull(freqStr);
            } catch (...) {
                std::cerr << "Error de formato en " << filename << ": " << line << std::endl;
                continue;
            }

            if (isInsert) {
                insertWord(word, frequency);
            } else {
                eraseWord(word, frequency);
            }
        }
        file.close();
    }

    void insertDocument(const std::string document){
        // --- FASE 1: BORRADO (Si la ventana está llena) ---
        // Comprobamos si la lista 'documents' ha alcanzado el tamaño de la ventana
        if (documents.size >= window) {
            
            // 1a. Obtener el nombre del archivo más antiguo (el de la cola)
            std::string fileToErase = documents.tail->data;
            
            // 1b. Procesar el archivo para borrar sus palabras de 'data'
            //std::cout << "Borrando: " << fileToErase << std::endl;
            processDocument(fileToErase, /*isInsert=*/false);
            
            // 1c. Quitar el archivo de la lista 'documents' (Pop del Tail)
            documents.popTail();
        }

        // --- FASE 2: INSERCIÓN (Del nuevo documento) ---
        
        // 2a. Procesar el nuevo archivo para insertar sus palabras en 'data'
        //std::cout << "Insertando: " << document << std::endl;
        processDocument(document, /*isInsert=*/true);

        nProcessedDocuments++;
        
        // 2b. Añadir el nuevo archivo a la lista 'documents' (Insert en Head)
        documents.insert(document);
        if (nProcessedDocuments%nDocumentsToVisualize==0) getTopWords();
    }

    void insertWord(const std::string &s, size_t quantity){
        timer.startTimer();
        Pair< Node<Pair<const std::string,size_t>>*,size_t > aux=find(s,1);
        if (aux.first){
            aux.first->data.second+=quantity;
            timer.stopTimer();
            sortData(aux.first);
        }
        else{
            Node< Pair<const std::string,size_t> > *tmp = data->insert(Pair<const std::string, size_t>(s,quantity));
            keys[aux.second].insert(tmp);
            timer.stopTimer();
            if (collision) {
                nCollisions++;
            }
            sortData(tmp);
        }
        if (getCollisionFactor()>maxCollisionFactor) doubleCapacity();
    }

    void eraseWord(const std::string &s, size_t quantity){
        timer.startTimer();
        Pair< Node<Pair<const std::string,size_t>>*,size_t > aux=find(s,0);
        aux.first->data.second-=quantity;
        if (aux.first->data.second==0){
            auto tmp = keys[aux.second].head;
            while (tmp){
                if (tmp->data->data.first==s) break;
                tmp=tmp->next;
            }
            data->erase(aux.first);
            keys[aux.second].erase(tmp);
            timer.stopTimer();
            if (collision) {
                nCollisions--;
            }
        }
        else{
            timer.stopTimer();
            sortData(aux.first);
        }
    }

    bool getTopWords(){
        if (documents.size<window) return false;
        std::string baseFilename = std::to_string(outputCounter) + ".csv";
        std::string outputPath = "output/documents/";
        std::string fullFilename = outputPath + baseFilename;
        
        std::ofstream outFile(fullFilename);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open file: " << fullFilename << std::endl;
            return false;
        }

        outFile << "word,frequency\n";

        Node< Pair<const std::string,size_t> >* currentNode = data->tail;
        size_t count = 0;

        while (currentNode && count < kTop) {
            outFile << currentNode->data.first << "," 
                    << currentNode->data.second << "\n";
            
            currentNode = currentNode->prev;
            count++;
        }
        

        outFile.close();
        outputCounter++;
        return true;
    }

    void showData(){
        auto tmp = data->head;
        while (tmp){
            std::cout<<tmp->data.first<<" "<<tmp->data.second<<std::endl;
            tmp=tmp->next;
        }
    }

    void showIndexWithData(){
        for (int i=0;i<capacity;i++){
            if (keys[i].head) std::cout<<i<<" ";
        }
        std::cout<<std::endl;
    }

    void showHead(){
        if (data->head){
            std::cout<<data->head->data.first<<" "<<data->head->data.second;
        }
        std::cout<<std::endl;
    }

    void showTail(){
        if (data->tail){
            std::cout<<data->tail->data.first<<" "<<data->tail->data.second;
        }
        std::cout<<std::endl;
    }

    ~HashTable(){
        delete data;
        delete[] keys;
    }
};



#endif