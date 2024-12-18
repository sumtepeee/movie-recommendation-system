# User-Based Movie Recommendation System

## Overview
This project is a C++ implementation of a **User-Based Movie Recommendation System**. It processes user-movie interaction data and provides recommendations based on various algorithms. The system supports dynamic user input, data-driven decisions, and performance tracking.

## Features
1. **Recommendation Random**: Randomly recommends movies not watched by the user.
2. **Popular Recommendation**: Recommends the most popular unwatched movies.
3. **Similar User**: Finds the most similar user and recommends movies they liked.
4. **Shortest Path**: Finds the shortest connection between users and items in a bipartite graph.
5. **Random Walk Algorithm**: Uses random walks to generate movie recommendations.
6. **Change User ID**: Allows switching to another user during runtime.

## Files
- `main.cpp`: Entry point of the program.
- `library.cpp`: Implementation of recommendation algorithms.
- `library.h`: Header file defining the `Graph` class and methods.
- `u.data`: Input dataset containing user-movie interactions.

## How to Run
1. Compile the project:
   ```bash
   g++ -std=c++20 main.cpp library.cpp  // u.data must be in the same folder
