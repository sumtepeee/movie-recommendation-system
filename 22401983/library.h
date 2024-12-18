#ifndef LIBRARY_H
#define LIBRARY_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility> // std::pair kullanımı için
#include <algorithm> // std::find kullanımı için
#include <cstdlib>   // rand() ve srand() için
#include <ctime> 
#include <tuple>

using namespace std;

// Hash-based
class Graph
{
private:
    unordered_map<int, vector<pair<int, int>>> userToItems; // kullanıcının izledigi filmler
    unordered_map<int, vector<pair<int, int>>> itemToUsers; // filmler kimler tarafınfan izlendi

    unordered_map<int, tuple<int, float,  long>> user_items; // kullanıcının izledigi filmler
    unordered_map<int, tuple<int, float,  long>> item_users; // filmler kimler tarafınfan izlendi

public:
    Graph();
    ~Graph();
    // Methodes
    void addEdge(int user_id, int item_id, int rating);
    void add_edge(int user_id, int item_id, int rating, long timestamp);
    void printUserToItems();
    void printItemToUsers();
    void recommendation_rondom(int user_id);  // 1.algoritma
    void popular_recommendation(int user_id); // 2.algoritma
    void similar_user(int user_id);           // 3.algoritma
    void shortest_path(int user_id);          // 4.algoritma
    void random_walk(int user_id);            // 5.algoritma
};







#endif