#ifndef DOUBLELINKEDLIST_HPP
#define DOUBLELINKEDLIST_HPP

#include <string>

template <typename A, typename B>
struct Pair{
    A first;
    B second;

    Pair(A s, B count) : first(s), second(count){}
};

template <typename A>
struct Node
{
    A data;
    Node<A>* next;
    Node<A>* prev;

    Node(A data, Node<A>* next=0, Node<A>* prev=0) : data(data), next(next), prev(prev){}
};

template <typename A>
struct DoubleLinkedList
{
    Node<A>* head=0;
    Node<A>* tail=0;

    size_t size=0;
    
    Node<A>* insert(A data){
        size++;
        head=new Node<A>(data,head);
        if (head->next){
            head->next->prev=head;
        }
        else {
            tail=head;
        }
        return head;
    }

    void erase(Node<A>* p){
        size--;
        if (p->prev){
            if (p->next){
                p->prev->next=p->next;
                p->next->prev=p->prev;
            }
            else{
                p->prev->next=p->next;
                tail=p->prev;
            }
        }
        else {
            if (p->next){
                head=p->next;
                p->next->prev=p->prev;
            }
            else{
                head=0;
                tail=0;
            }
        }
        delete p;
    }

    void popTail(){
        erase(tail);
    }

    // void print(){
    //     Node<A>* tmp=head;
    //     while (tmp){
    //         std::cout<<tmp->data<<" ";
    //         tmp=tmp->next;
    //     }
    //     std::cout<<std::endl;
    // }

    ~DoubleLinkedList(){
        while (head){
            Node<A>* tmp = head;
            head=head->next;
            delete tmp;
        }
    }
};


// int main(){
//     // const std::string s="hola";
//     // int frecuency=7;
//     // Pair<const std::string,int> p1(s,frecuency);

//     // Node<Pair<const std::string,int>> n1(p1);

//     // std::cout<<n1.data.first<<" "<<n1.data.second;

//     DoubleLinkedList<int> ll1;
//     ll1.insert(15);
//     ll1.insert(15);
//     Node<int>* aux = ll1.insert(13);
//     ll1.insert(14);

//     ll1.print();

//     ll1.erase(aux);

//     ll1.print();
    
//     return 0;
// }

#endif