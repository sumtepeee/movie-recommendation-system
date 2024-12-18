/*
 * main.cpp
 *
 *  Created on : 13 december  2024
 *  Author : Sumeyye
 *  number : 22401983
 */

#include <iostream>
#include <typeinfo>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <stdbool.h>
#include "library.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Graph graph;

    // Dosya okuma islemleri
    ifstream file("u.data"); // Dosyanın adını buraya yazın
    if (!file.is_open()) {
        cerr << "Dosya açılamadı!" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line); // line i kelime kelime okuyoruz
        int user_id, item_id;
        int rating;
        long timestamp;

        // Sutunlari sirayla okuyoruz
        if (ss >> user_id >> item_id >> rating >> timestamp) {
          graph.addEdge(user_id, item_id, rating); 
          graph.add_edge(user_id, item_id, rating, timestamp);
        }
    }
    int choice = -1;
    int user_id;
 while (true) {
        // Kullanıcı ID'sini al
        cout << "\nEnter your user ID (or 0 to exit): ";
        cin >> user_id;

        // Programdan çıkış kontrolü
        if (user_id == 0) {
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        }
  

    while (choice != 0) {
        cout << "\nSelect an option:\n";
        cout << "1. Recommendation Random\n";
        cout << "2. Popular Recommendation\n";
        cout << "3. Similar User\n";
        cout << "4. Shortest Path\n";
        cout << "5. Random Walk Algorithm\n";
        cout << "6. Change User ID\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 6) {
                cout << "Returning to user selection...\n";
                break; // İç döngüden çık ve kullanıcı ID'sini tekrar sor
            }

            if (choice == 0) {
                cout << "Exiting the program. Goodbye!" << endl;
                return 0; // Programı tamamen sonlandır
            }

        switch (choice) {
            case 1:
                graph.recommendation_rondom(user_id);
                break;
            case 2:
                graph.popular_recommendation(user_id);
                break;
            case 3:
                graph.similar_user(user_id);
                break;
            case 4:
                graph.shortest_path(user_id);
                break;
            case 5:
                graph.random_walk(user_id);
                break;
            case 0:
                cout << "Exiting the program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please select a valid option." << endl;
        }
    }
}
    return 0;
}
