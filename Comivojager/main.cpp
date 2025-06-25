#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>
#include <mpi.h>
#include <SFML/Graphics.hpp>



struct City {
    int id;
    double x;
    double y;
};
void drawTour(const std::vector<City>& cities, const std::vector<int>& tour, const std::string& windowTitle) {
    const int windowSize = 600;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), windowTitle);

    // Znajdź zakres współrzędnych, aby przeskalować punkty do okna
    double minX = cities[0].x, maxX = cities[0].x;
    double minY = cities[0].y, maxY = cities[0].y;
    for (const auto& city : cities) {
        if (city.x < minX) minX = city.x;
        if (city.x > maxX) maxX = city.x;
        if (city.y < minY) minY = city.y;
        if (city.y > maxY) maxY = city.y;
    }

    double scaleX = (maxX - minX) > 0 ? (windowSize - 40) / (maxX - minX) : 1;
    double scaleY = (maxY - minY) > 0 ? (windowSize - 40) / (maxY - minY) : 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        // Rysuj linie łączące miasta wg kolejności w tour
        sf::VertexArray lines(sf::LinesStrip, tour.size() + 1);
        for (size_t i = 0; i < tour.size(); ++i) {
            const City& c = cities[tour[i]];
            float x = static_cast<float>((c.x - minX) * scaleX + 20);
            float y = static_cast<float>((c.y - minY) * scaleY + 20);
            lines[i].position = sf::Vector2f(x, y);
            lines[i].color = sf::Color::Blue;
        }
        // Zamykamy trasę (wracamy do pierwszego miasta)
        const City& firstCity = cities[tour[0]];
        lines[tour.size()].position = sf::Vector2f(
            static_cast<float>((firstCity.x - minX) * scaleX + 20),
            static_cast<float>((firstCity.y - minY) * scaleY + 20));
        lines[tour.size()].color = sf::Color::Blue;

        window.draw(lines);

        // Rysuj miasta jako kropki
        for (int idx : tour) {
            const City& c = cities[idx];
            float x = static_cast<float>((c.x - minX) * scaleX + 20);
            float y = static_cast<float>((c.y - minY) * scaleY + 20);
            sf::CircleShape circle(4);
            circle.setFillColor(sf::Color::Red);
            circle.setPosition(x - circle.getRadius(), y - circle.getRadius());
            window.draw(circle);
        }

        window.display();
    }
}

std::vector<City> load_tsp_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<City> cities;
    bool in_section = false;

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return cities;
    }

    while (std::getline(file, line)) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            in_section = true;
            continue;
        }
        if (line.find("EOF") != std::string::npos) {
            break;
        }
        if (in_section) {
            std::istringstream iss(line);
            City c;
            if (!(iss >> c.id >> c.x >> c.y)) {
                continue;
            }
            cities.push_back(c);
        }
    }
    return cities;
}

double calculateDistance(const City &a, const City &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

double tourLength(const std::vector<int> &tour, const std::vector<City> &cities) {
    double total = 0.0;
    for (size_t i = 0; i < tour.size(); ++i) {
        const City &from = cities[tour[i]];
        const City &to = cities[tour[(i + 1) % tour.size()]];
        total += calculateDistance(from, to);
    }
    return total;
}

std::vector<int> generateNeighbor(const std::vector<int> &tour, std::mt19937 &rng) {
    std::vector<int> newTour = tour;
    std::uniform_int_distribution<int> dist(0, tour.size() - 1);
    int i = dist(rng), j = dist(rng);
    while (i == j) {
        j = dist(rng);
    }
    std::swap(newTour[i], newTour[j]);
    return newTour;
}

std::vector<int> SA(const std::vector<City> &cities, int rank) {
    std::mt19937 rng(static_cast<unsigned int>(std::time(0)) + rank);

    std::vector<int> currentTour(cities.size());
    for (size_t i = 0; i < cities.size(); ++i) {
        currentTour[i] = i;
    }
    std::shuffle(currentTour.begin(), currentTour.end(), rng);

    double currentCost = tourLength(currentTour, cities);

    double T = 1000.0;
    double coolingRate = 0.995;
    double minTemperature = 1e-4;

    std::vector<int> bestTour = currentTour;
    double bestCost = currentCost;

    while (T > minTemperature) {
        std::vector<int> neighborTour = generateNeighbor(currentTour, rng);
        double neighborCost = tourLength(neighborTour, cities);
        double delta = neighborCost - currentCost;

        if (delta < 0 || std::exp(-delta / T) > ((double)rng() / rng.max())) {
            currentTour = neighborTour;
            currentCost = neighborCost;
        }

        if (currentCost < bestCost) {
            bestTour = currentTour;
            bestCost = currentCost;
        }

        T *= coolingRate;
    }

    return bestTour;
}

void saveTour(const std::vector<int> &tour, const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file to save tour: " << filename << std::endl;
        return;
    }
    for (int city : tour) {
        file << city + 1 << std::endl;
    }
    file.close();
}

int main(int argc, char **argv) {
    std::vector<City> cities = load_tsp_file("../att532.tsp");
    int numCities = static_cast<int>(cities.size());

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Bcast(&numCities, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (numCities == 0) {
        if(rank == 0) {
            std::cerr << "Error: Number of cities is zero. Exiting." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    if (rank != 0) {
        cities.resize(numCities);
    }

    std::vector<double> buffer(numCities * 3);
    if (rank == 0) {
        for (int i = 0; i < numCities; ++i) {
            buffer[i * 3 + 0] = static_cast<double>(cities[i].id);
            buffer[i * 3 + 1] = cities[i].x;
            buffer[i * 3 + 2] = cities[i].y;
        }
    }

    MPI_Bcast(buffer.data(), numCities * 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        for (int i = 0; i < numCities; ++i) {
            cities[i].id = static_cast<int>(buffer[i * 3 + 0]);
            cities[i].x = buffer[i * 3 + 1];
            cities[i].y = buffer[i * 3 + 2];
        }
    }

    // Obliczamy koszt podstawowej trasy (po kolei)
    std::vector<int> baseTour(numCities);
    for (int i = 0; i < numCities; ++i) baseTour[i] = i;
    double baseCost = tourLength(baseTour, cities);

    // Teraz rank 0 zbierze koszty podstawowej trasy od wszystkich procesów (wszystkie powinny mieć ten sam wynik)
    double globalBaseCost;
    MPI_Reduce(&baseCost, &globalBaseCost, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "[Rank 0] Cost of base (sequential) tour: " << globalBaseCost << std::endl;
    }

    std::vector<int> localTour = SA(cities, rank);
    double localCost = tourLength(localTour, cities);

    struct {
        double cost;
        int rank;
    } localResult = { localCost, rank }, bestResult;

    MPI_Allreduce(&localResult, &bestResult, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

    std::vector<int> bestTour(numCities);
    if (rank == bestResult.rank) {
        MPI_Send(localTour.data(), numCities, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        MPI_Recv(bestTour.data(), numCities, MPI_INT, bestResult.rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        saveTour(bestTour, "best_tour.txt");
        std::cout << "[Rank 0] Best tour cost: " << bestResult.cost << std::endl;
        std::cout << "[Rank 0] Best tour from process: " << bestResult.rank << std::endl;
        drawTour(cities, baseTour, "Base Tour");
        drawTour(cities, bestTour, "Best Tour");
    }

    MPI_Finalize();
    
    return 0;
}