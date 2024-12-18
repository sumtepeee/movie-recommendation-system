/*
 * main.cpp
 *
 *  Created on : 13 december  2024
 *  Author : Sumeyye
 *  number : 22401983
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility> // std::pair kullanımı için
#include <algorithm> // std::find, std::partial_sort kullanımı için  
#include <cstdlib>   // rand() ve srand() için
#include <ctime> 
#include <queue>

#include "library.h"

using namespace std;


// Adjaceny List implementation
Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::addEdge(int user_id, int item_id, int rating)
{
    userToItems[user_id].emplace_back(item_id, rating);
    itemToUsers[item_id].emplace_back(user_id, rating);
    
}

// 5. algoritma icin 
void Graph::add_edge(int user_id, int item_id, int rating, long timestamp){
    user_items.emplace(user_id, make_tuple(item_id, rating, timestamp));
    item_users.emplace(item_id, make_tuple(user_id, rating, timestamp));
}

void Graph::recommendation_rondom(int user_id)
{
    clock_t start = clock();
    cout << "\nExecuting Recommendation Random Algorithm for user " << user_id << endl;
    unordered_map<int, bool> watched;  // kullanıcının izledigi filmleri tutar
    if (userToItems.find(user_id) != userToItems.end()) {
        for (const auto& movie : userToItems[user_id]) {
            watched[movie.first] = true;
        }
    }

    vector<int> unwatched_movies;  // kullanıcının izlemedigi filmleri tutar
    for (const auto& movie : itemToUsers) {
        if (watched.find(movie.first) == watched.end()) {
            unwatched_movies.push_back(movie.first);
        }
    }

    if (!unwatched_movies.empty()) {
        srand(time(0)); // Rastgelelik için seed
        int randomIndex = rand() % unwatched_movies.size();
        cout << "Movie ID : " <<  unwatched_movies[randomIndex] << endl;
    } 
    else {
        cout << "\nNo movies left to recommend for user "<< user_id << "!" << endl; // Önerilecek film yok
    }
    clock_t end = clock(); // Zaman ölçüm bitişi
    cout << "Execution time : " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
}

void Graph::popular_recommendation(int user_id){
    clock_t start = clock();
    cout << "\nExecuting Most Popular Recommendation Algorithm for user " << user_id << endl;
    // Dereceleri hesaplıyoruz
    vector<pair<int, int>> degrees;
    for (const auto& item : itemToUsers) {
        degrees.emplace_back(item.first, item.second.size()); // Film ID ve izlenme sayısı
    }

    // Kullanıcının izlediği filmleri buluyoruz
    unordered_map<int, bool> watched;
    if (userToItems.find(user_id) != userToItems.end()) {
        for (const auto& movie : userToItems[user_id]) {
            watched[movie.first] = true;    // İzlenen filmleri işaretliyoruz
        }
    }

    // İzlenmiş filmleri `degrees` vektöründen çıkartıyoruz
    auto unwatchedEnd = remove_if(degrees.begin(), degrees.end(),
        [&watched](const pair<int, int>& movie) {
            return watched.find(movie.first) != watched.end(); // İzlenmiş filmleri çıkart
        });
    degrees.erase(unwatchedEnd, degrees.end());

    // Eğer izlenmemiş film yoksa mesaj verip çık
    if (degrees.empty()) {
        cout << "No unwatched movies available for user " << user_id << "." << endl;
        return;
    }

    // İlk 5 izlenmeyen filmi sıralıyoruz
    auto limit = min(degrees.size(), size_t(5)); // Eger izlenmeyen film sayisi 5 ten kucukse
    partial_sort(degrees.begin(), degrees.begin() + limit, degrees.end(),  // Baslangic konumu, siralanacak en son konum, dizinin sonu
        [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second > b.second;                                 // Azalan sıralama (izlenme sayısına göre)
        });

    // İzlenmemiş ilk 5 filmi yazdırıyoruz
    cout << "ecommended unwatched movies for user " << user_id << ":\n";
    for (size_t i = 0; i < limit; ++i) {
        cout << "  Movie ID : " << degrees[i].first << ", Watch Count: " << degrees[i].second << endl;
    }
    clock_t end = clock(); // Zaman ölçüm bitişi
    cout << "Execution time : " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
}

void Graph::similar_user(int user_id){
    clock_t start = clock();
    cout << "\nExecuting Similar User Algorithm for user " << user_id << endl;
    // Kullanicinin izledigi filmleri buluyoruz
    unordered_map<int, bool> watched;
    if (userToItems.find(user_id) != userToItems.end()) {            // Boyle bir kullanici var mı kontrolunu yapıyoruz
        for (const auto& movie : userToItems[user_id]) {
            watched[movie.first] = true;                             // İzlenen filmleri işaretliyoruz
        }
    }

    // Ayni filmi izleyen kullanicilari bulup kac tane ortak film izlediklerini buluyoruz
    unordered_map<int, int> similar_users;                                  // Kullanici -> Ortak film sayisi
    for (const auto& movie : watched) {
        if (itemToUsers.find(movie.first) != itemToUsers.end()) {           // Bu filmi izleyen kullanıcılar varsa
            for (const auto& otherUser : itemToUsers[movie.first]) {
                if (otherUser.first != user_id) {                           // Hedef kullanıcı hariç
                    similar_users[otherUser.first]++;
                }
            }
        }
    }

    // en benzer kullanicilari siraliyoruz
    vector<pair<int, int>> sorted_similar_users(similar_users.begin(), similar_users.end());
    sort(sorted_similar_users.begin(), sorted_similar_users.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.second > b.second;                                   // azalan siralama
         });

    vector<pair<int, int>> recommended_movies; // Önerilecek filmleri toplar
    size_t max_recommendations = 5; // Önerilecek maksimum film sayısı

    //  En benzer kullanıcıdan başlayarak öneri yapmaya çalış
    for (const auto& similar_user : sorted_similar_users) {
        int similar_user_ıd = similar_user.first;

        // En benzer kullanıcının izlediği filmlerden hedef kullanıcının izlemediklerini al
        for (const auto& movie : userToItems[similar_user_ıd]) {
            if (watched.find(movie.first) == watched.end()) { // Hedef kullanıcının izlemediği filmler
                recommended_movies.emplace_back(movie.first, movie.second);
                watched[movie.first] = true; // Önerilen filmleri işaretle
                if (recommended_movies.size() >= max_recommendations) break; // Maksimum sayıya ulaştık
            }
        }
        if (recommended_movies.size() >= max_recommendations) break; // Maksimum sayıya ulaştık
    }

    // Eğer hala önerilecek film kalmışsa sıradaki kullanıcıları kontrol et
    for (const auto& similar_user : sorted_similar_users) {
        if (recommended_movies.size() >= max_recommendations) break; // Maksimum sayıya ulaştık
        int similar_user_id = similar_user.first;

        for (const auto& movie : userToItems[similar_user_id]) {
            if (watched.find(movie.first) == watched.end()) { // Hedef kullanıcının izlemediği filmler
                recommended_movies.emplace_back(movie.first, movie.second);
                watched[movie.first] = true; // Önerilen filmleri işaretle
                if (recommended_movies.size() >= max_recommendations) break; // Maksimum sayıya ulaştık
            }
        }
    }

    // 5. Eğer öneri yapılabiliyorsa, filmleri yazdır
    if (!recommended_movies.empty()) {
        // Filmleri rating'e göre sıralama
        sort(recommended_movies.begin(), recommended_movies.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second > b.second; // Rating'e göre azalan sırada sıralama
             });

             cout<< "The most similar user : " << sorted_similar_users.front().first << endl;
        for (size_t i = 0; i < min(recommended_movies.size(), max_recommendations); ++i) {
            cout << "  Movie ID : " << recommended_movies[i].first << ", Rating: " << recommended_movies[i].second << endl;
        }
    } else {
        cout << "No movies to recommend for user " << user_id << "." << endl;
    }

    clock_t end = clock(); // Zaman ölçüm bitişi
    cout << "Execution time : " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
   
}

void Graph::shortest_path(int user_id) {
    clock_t start = clock();
    cout << "\nExecuting Shortest Path Algorithm for user " << user_id << endl;
    // 1. Kullanıcının izlediği filmleri işaretle
    unordered_map<int, bool> watched;
    if (userToItems.find(user_id) != userToItems.end()) {
        for (const auto& movie : userToItems[user_id]) {
            watched[movie.first] = true;
        }
    }

    // 2. Tüm düğümler için Dijkstra mesafelerini saklayacak yapı
    unordered_map<int, double> distances;       // Mesafeleri saklar
    unordered_map<int, bool> visited;           // Ziyaret edilen düğümleri işaretler
    for (const auto& [item, _] : itemToUsers) {
        distances[item + 1000] = MAXFLOAT;      // Başlangıçta tüm filmler için mesafeyi sonsuz yap
    }
    for (const auto& [user, _] : userToItems) {
        distances[user] = MAXFLOAT;             // Kullanıcı düğümleri için de aynı şekilde
    }
    distances[user_id] = 0.0; // Başlangıç düğümünün mesafesi 0

    // 3. Min-heap (priority queue) kullanımı
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;  // distance - mevcut node tutuluyor
    pq.push({0.0, user_id});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (visited[currentNode]) continue; // Ziyaret edildiyse atla
        visited[currentNode] = true;

        // Komşuları kontrol et
        if (userToItems.find(currentNode) != userToItems.end()) {
            // Kullanıcıdan filme geçiş
            for (const auto& [neighbor, rating] : userToItems[currentNode]) {
                double weight = 1.0 / rating;           // Ağırlık olarak rating'in tersi
                if (currentDist + weight < distances[neighbor]) {
                    distances[neighbor] = currentDist + weight;
                    pq.push({distances[neighbor], neighbor});
                }
            }
        } else if (itemToUsers.find(currentNode - 1000) != itemToUsers.end()) {
            // Filmden kullanıcıya geçiş
            for (const auto& [neighbor, rating] : itemToUsers[currentNode - 1000]) {
                double weight = 1.0 / rating;            // Ağırlık olarak rating'in tersi
                if (currentDist + weight < distances[neighbor]) {
                    distances[neighbor] = currentDist + weight;
                    pq.push({distances[neighbor], neighbor});
                }
            }
        }
    }

    // 4. İzlenmemiş filmleri topla ve öneri yap
    vector<pair<int, double>> recommendations;     // item_id - distance 
    for (const auto& [movie, users] : itemToUsers) {
        int movieNode = movie + 1000; // Film düğümünü al
        if (!watched[movie] && distances[movieNode] < MAXFLOAT) {
            recommendations.emplace_back(movie, distances[movieNode]);
        }
    }

    /// 5. İlk 5 elemanı sırala
    auto limit = min(recommendations.size(), size_t(5));
    partial_sort(recommendations.begin(), recommendations.begin() + limit, recommendations.end(), 
        [](const auto& a, const auto& b) {
        return a.second < b.second;             // Mesafeye göre artan sırada sıralama
    });

    // 6. İlk 5 filmi yazdır
    cout << "Recommended movies for user " << user_id << ":\n";
    for (size_t i = 0; i < limit; ++i) {
        cout << "  Movie ID : " << recommendations[i].first << ", Score: " << recommendations[i].second << endl;
    }

    clock_t end = clock(); // Zaman ölçüm bitişi
    cout << "Execution time : " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
   
}

void Graph::random_walk(int user_id) {
    clock_t start = clock();
    cout << "\nExecuting Random Walk Algorithm for user " << user_id << endl;
    // Kullanicinin izledikleri filmleri tutar
    vector<tuple<int, int, long>> user_movies; // item_id, rating, timestamp
    // Kullanıcının izlediği tüm filmleri user_items'tan al
    for (const auto& entry : user_items) {
        if (entry.first == user_id) {
            user_movies.emplace_back(get<0>(entry.second), get<1>(entry.second), get<2>(entry.second));
        }                             // item_id         , rating              , timestamp
    }

    if (user_movies.empty()) {
        cout << "User " << user_id << " has not rated any movies." << endl;
        return;
    }

    // Son 50 elemani siraliyoruz
    size_t limit = min(user_movies.size(), size_t(50));
    partial_sort(user_movies.begin(), user_movies.begin() + limit, user_movies.end(),
                      [](const tuple<int, int, long>& a, const tuple<int, int, long>& b) {
                          return get<2>(a) > get<2>(b);         // timestamp'e göre azalan sıralama
                      });

    
    vector<int> last_50_movies; // Sadece item_id'leri tutar
    for (size_t i = 0; i < limit; ++i) {
        last_50_movies.push_back(get<0>(user_movies[i]));  // item_id
    }

    // Bu filmleri izleyen kullanıcıları bul
    vector<int> related_users; // İlgili kullanıcılar
    for (const auto& item_id : last_50_movies) {
        // item_id'nin item_users içinde olup olmadığını kontrol et
        if (item_users.find(item_id) != item_users.end()) {
            const auto& user_info = item_users[item_id];     // Bu film için kullanıcı bilgisi al
            int user_id = std::get<0>(user_info);            // Kullanıcı ID'sini al

            // Eğer kullanıcı daha önce eklenmemişse listeye ekle
            if (std::find(related_users.begin(), related_users.end(), user_id) == related_users.end()) {
                related_users.push_back(user_id);
            }
        }
    }

      // Bu kullanıcıların izlediği diğer filmleri bul
    unordered_map<int, int> recommended_movies; // movie_id -> toplam rating
    for (const auto& related_user : related_users) {
        if (userToItems.find(related_user) != userToItems.end()) {
            for (const auto& movie : userToItems[related_user]) {
                int item_id = movie.first; // Kullanıcının izlediği filmin item_id'si
                int rating = movie.second; // Kullanıcının verdiği rating

                // Eğer önerilecek kullanıcı bu filmi izlememişse ve listeye eklenmemişse ekle
                if (std::find(last_50_movies.begin(), last_50_movies.end(), item_id) == last_50_movies.end()) {
                    recommended_movies[item_id] = rating; // Filmi ve rating'ini ekle
                }
            }
        }
    }

    // Kullanıcının izlediği filmleri öneri listesinden çıkar
    for (auto it = recommended_movies.begin(); it != recommended_movies.end(); ) {
        // Kullanıcının izlediği filmleri kontrol et
        bool already_watched = false;
        for (const auto& user_movie : user_movies) {
            int watched_item_id = std::get<0>(user_movie); // Kullanıcının izlediği filmin item_id'si
            if (it->first == watched_item_id) { // Önerilen film zaten izlenmiş
                already_watched = true;
                break;
            }
        }

        // Eğer kullanıcı filmi zaten izlediyse önerilerden çıkar
        if (already_watched) {
            it = recommended_movies.erase(it); // Sil ve bir sonraki elemana geç
        } else {
            ++it; // Diğer filme geç
        }
    }

    // random walk kullanarak oneri
    vector<int> suggested_movies; // Önerilen filmler
    srand(time(0)); // Rastgelelik için seed

    if (recommended_movies.empty()) {
        cout << "No movies to recommend." << endl;
        return;
    }
    
    auto it = recommended_movies.begin();
    advance(it, rand() % recommended_movies.size()); // Rastgele bir film seç
    int current_movie = it->first;

    for (int step = 0; step < 10; ++step) {        // 10 adim ilerliyor
        // Eğer film kullanıcılar tarafından izlendiyse, rastgele bir kullanıcıya geç
        if (item_users.find(current_movie) != item_users.end()) {
            const auto& user_info = item_users[current_movie]; // Filmden kullanıcıya geçiş
            int random_user = std::get<0>(user_info); // Kullanıcı ID'sini al

            // Bu kullanıcının izlediği filmleri al
            if (userToItems.find(random_user) != userToItems.end()) {
                const auto& movies = userToItems[random_user];
                int random_index = rand() % movies.size(); // Rastgele bir film seç
                int next_movie = movies[random_index].first;

                // Eğer önerilecek kullanıcı bu filmi izlememişse ekle
                bool already_watched = false;
                for (const auto& user_movie : user_movies) {
                    if (std::get<0>(user_movie) == next_movie) {
                        already_watched = true;
                        break;
                    }
                }

                if (!already_watched) {
                    suggested_movies.push_back(next_movie);
                }

                // Geçiş yap
                current_movie = next_movie;
            }
        }
    }

    // Sonuçları yazdır

    for (const auto& movie : suggested_movies) {
        cout << "  Movie ID : " << movie << endl;
    }

    clock_t end = clock(); // Zaman ölçüm bitişi
    cout << "Execution time : " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
}
