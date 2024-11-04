/* TSP Project by Sonmathi Lakshya R J*/




#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <ctime>


struct City {
    double x, y;
};


double calculateDistance(const City &a, const City &b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}


double calculateTotalCost(const std::vector<City> &cities, const std::vector<int> &tour) {
    double cost = 0.0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        cost += calculateDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    cost += calculateDistance(cities[tour.back()], cities[tour[0]]);
    return cost;
}


void generateNeighbor(std::vector<int> &tour) {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    int n = tour.size();
    std::uniform_int_distribution<int> dist(0, n - 1);

    int i = dist(rng);
    int j = dist(rng);
    while (i == j) {
        j = dist(rng);
    }
    std::swap(tour[i], tour[j]);
}

std::vector<int> simulatedAnnealing(const std::vector<City> &cities, int iterations, double initialTemp, double coolingRate) {
    int n = cities.size();
    std::vector<int> currentTour(n);
    std::iota(currentTour.begin(), currentTour.end(), 0); 


    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(currentTour.begin(), currentTour.end(), g);

    double currentCost = calculateTotalCost(cities, currentTour);
    std::vector<int> bestTour = currentTour;
    double bestCost = currentCost;

    double temperature = initialTemp;

    for (int iter = 0; iter < iterations; iter++) {
        std::vector<int> newTour = currentTour;
        generateNeighbor(newTour);
        double newCost = calculateTotalCost(cities, newTour);

        if (newCost < currentCost || std::exp((currentCost - newCost) / temperature) > (double)rand() / RAND_MAX) {
            currentTour = newTour;
            currentCost = newCost;
            if (currentCost < bestCost) {
                bestTour = currentTour;
                bestCost = currentCost;
            }
        }

        temperature *= coolingRate; 
    }

    std::cout << "Final Cost: " << bestCost << std::endl;
    std::cout << "Best Tour: ";
    for (int city : bestTour) {
        std::cout << city << " ";
    }
    std::cout << std::endl;

    return bestTour;
}


std::vector<City> loadCitiesFromFile(const std::string &filename) {
    std::vector<City> cities;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line == "NODE_COORD_SECTION") break;
    }

    while (std::getline(file, line)) {
        if (line == "EOF") break; 
        std::istringstream iss(line);
        int index;
        double x, y;
        if (iss >> index >> x >> y) {
            cities.push_back({x, y});
        }
    }

    file.close();
    return cities;
}


int main() {
    try {
     
        std::string filename = "rbu737.tsp";
        std::vector<City> cities = loadCitiesFromFile(filename);


        int iterations = 10000;
        double initialTemp = 10000.0;
        double coolingRate = 0.995;


        simulatedAnnealing(cities, iterations, initialTemp, coolingRate);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;   
    }

    return 0;
}
