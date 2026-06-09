// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <cstdlib>

#include "train.h"

// Constants
const int N_MIN = 2;
const int N_MAX = 80;
const int RANDOM_TRIALS = 20;
const uint64_t RANDOM_SEED = 42ULL;

// Generate range of numbers
std::vector<int> range(int from, int toInclusive) {
    std::vector<int> result;
    for (int i = from; i <= toInclusive; ++i) {
        result.push_back(i);
    }
    return result;
}

// Forward declaration
std::vector<double> solve3(const std::vector<std::vector<double>>& m,
                           const std::vector<double>& v);

// Measure number of operations
double measure(int n, bool allOff, bool allOn, std::mt19937& rng,
               int trials) {
    int64_t sum = 0;
    for (int t = 0; t < trials; ++t) {
        Train train;
        for (int i = 0; i < n; ++i) {
            if (allOff) {
                train.addCar(false);
            } else if (allOn) {
                train.addCar(true);
            } else {
                std::uniform_int_distribution<int> dist(0, 1);
                train.addCar(dist(rng) == 1);
            }
        }
        int len = train.getLength();  // Сохраняем результат
        int ops = train.getOpCount();  // Сохраняем результат
        sum += ops;
        // Можно добавить проверку
        if (len != n) {
            std::cerr << "Warning: length mismatch! Expected " 
                      << n << ", got " << len << std::endl;
        }
    }
    return static_cast<double>(sum) / trials;
}

// Linear trend (y = a*x + b)
std::vector<double> linearTrend(const std::vector<int>& x,
                                const std::vector<double>& y) {
    int n = x.size();
    double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;

    for (int i = 0; i < n; ++i) {
        sumX += x[i];
        sumY += y[i];
        sumXX += static_cast<double>(x[i]) * x[i];
        sumXY += static_cast<double>(x[i]) * y[i];
    }

    double denom = n * sumXX - sumX * sumX;
    double a = (n * sumXY - sumX * sumY) / denom;
    double b = (sumY - a * sumX) / n;

    std::vector<double> trend(n);
    for (int i = 0; i < n; ++i) {
        trend[i] = a * x[i] + b;
    }
    return trend;
}

// Quadratic trend (y = a*x^2 + b*x + c)
std::vector<double> quadraticTrend(const std::vector<int>& x,
                                   const std::vector<double>& y) {
    int n = x.size();
    double s0 = n, s1 = 0, s2 = 0, s3 = 0, s4 = 0;
    double t0 = 0, t1 = 0, t2 = 0;

    for (int i = 0; i < n; ++i) {
        double xi = x[i];
        double xi2 = xi * xi;
        double yi = y[i];
        s1 += xi;
        s2 += xi2;
        s3 += xi2 * xi;
        s4 += xi2 * xi2;
        t0 += yi;
        t1 += xi * yi;
        t2 += xi2 * yi;
    }

    // Solve 3x3 system using Gaussian elimination
    std::vector<std::vector<double>> m = {{s0, s1, s2},
                                           {s1, s2, s3},
                                           {s2, s3, s4}};
    std::vector<double> v = {t0, t1, t2};
    std::vector<double> coeff = solve3(m, v);

    double a = coeff[0], b = coeff[1], c = coeff[2];
    std::vector<double> trend(n);
    for (int i = 0; i < n; ++i) {
        double xi = x[i];
        trend[i] = a * xi * xi + b * xi + c;
    }
    return trend;
}

// Solve 3x3 system
std::vector<double> solve3(const std::vector<std::vector<double>>& m,
                           const std::vector<double>& v) {
    const int n = 3;
    std::vector<std::vector<double>> a(n, std::vector<double>(n + 1));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = m[i][j];
        }
        a[i][n] = v[i];
    }

    for (int col = 0; col < n; ++col) {
        int pivot = col;
        for (int row = col + 1; row < n; ++row) {
            if (std::abs(a[row][col]) > std::abs(a[pivot][col])) {
                pivot = row;
            }
        }

        std::swap(a[col], a[pivot]);

        double div = a[col][col];
        if (std::abs(div) < 1e-12) {
            throw std::runtime_error(
                "Singular matrix in quadratic fit");
        }

        for (int j = col; j <= n; ++j) {
            a[col][j] /= div;
        }

        for (int row = 0; row < n; ++row) {
            if (row == col) continue;
            double factor = a[row][col];
            for (int j = col; j <= n; ++j) {
                a[row][j] -= factor * a[col][j];
            }
        }
    }

    return {a[0][n], a[1][n], a[2][n]};
}

// Write to CSV
void writeCsv(const std::string& path, const std::vector<int>& ns,
              const std::vector<double>& off,
              const std::vector<double>& on,
              const std::vector<double>& random) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << path << std::endl;
        return;
    }
    file << "n,all_off,all_on,random_avg\n";
    for (size_t i = 0; i < ns.size(); ++i) {
        file << ns[i] << ','
             << static_cast<int>(std::round(off[i])) << ','
             << static_cast<int>(std::round(on[i])) << ','
             << static_cast<int>(std::round(random[i])) << '\n';
    }
}

// Save plot data
void savePlotData(const std::string& outPath, const std::vector<int>& ns,
                  const std::vector<double>& off,
                  const std::vector<double>& on,
                  const std::vector<double>& random,
                  const std::vector<double>& trendOff,
                  const std::vector<double>& trendOn,
                  const std::vector<double>& trendRandom) {
    std::ofstream file(outPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << outPath << std::endl;
        return;
    }
    file << "# Train Length vs Operations Plot Data\n";
    file << "# Format: n all_off all_on random_avg ";
    file << "trend_off trend_on trend_random\n";

    for (size_t i = 0; i < ns.size(); ++i) {
        file << ns[i] << " "
             << off[i] << " "
             << on[i] << " "
             << random[i] << " "
             << trendOff[i] << " "
             << trendOn[i] << " "
             << trendRandom[i] << "\n";
    }

    file << "\n# Trend lines formulas:\n";

    int n = ns.size();
    double sumX = 0, sumYOff = 0, sumYOn = 0, sumYRandom = 0;
    double sumXX = 0, sumXYOff = 0, sumXYOn = 0, sumXYRandom = 0;

    for (int i = 0; i < n; ++i) {
        sumX += ns[i];
        sumYOff += off[i];
        sumYOn += on[i];
        sumYRandom += random[i];
        sumXX += static_cast<double>(ns[i]) * ns[i];
        sumXYOff += static_cast<double>(ns[i]) * off[i];
        sumXYOn += static_cast<double>(ns[i]) * on[i];
        sumXYRandom += static_cast<double>(ns[i]) * random[i];
    }

    double denom = n * sumXX - sumX * sumX;
    if (std::abs(denom) > 1e-12) {
        double aOff = (n * sumXYOff - sumX * sumYOff) / denom;
        double bOff = (sumYOff - aOff * sumX) / n;
        double aOn = (n * sumXYOn - sumX * sumYOn) / denom;
        double bOn = (sumYOn - aOn * sumX) / n;
        double aRandom = (n * sumXYRandom - sumX * sumYRandom) / denom;
        double bRandom = (sumYRandom - aRandom * sumX) / n;

        file << "\n# Linear trends:\n";
        file << "# all_off: y = " << aOff << " * x + " << bOff << "\n";
        file << "# all_on: y = " << aOn << " * x + " << bOn << "\n";
        file << "# random: y = " << aRandom << " * x + " << bRandom << "\n";
    }
}

int main() {
    std::cout << "Starting computational experiment...\n";

    // Create result directory
    if (system("mkdir -p result") == -1) {
        std::cerr << "Warning: Could not create result directory\n";
    }

    std::mt19937 rng(static_cast<unsigned int>(RANDOM_SEED));
    std::vector<int> ns = range(N_MIN, N_MAX);
    std::vector<double> offMeasurements, onMeasurements, randomMeasurements;

    std::cout << "Measuring for different train lengths...\n";

    for (int n : ns) {
        std::cout << "  n = " << n << "... " << std::flush;

        double offAvg = measure(n, true, false, rng, 1);
        double onAvg = measure(n, false, true, rng, 1);
        double randomAvg = measure(n, false, false, rng, RANDOM_TRIALS);

        offMeasurements.push_back(offAvg);
        onMeasurements.push_back(onAvg);
        randomMeasurements.push_back(randomAvg);

        std::cout << "done (off=" << offAvg
                  << ", on=" << onAvg
                  << ", random=" << randomAvg << ")\n";
    }

    std::cout << "\nCalculating trends...\n";

    std::vector<double> trendOff = linearTrend(ns, offMeasurements);
    std::vector<double> trendOn = linearTrend(ns, onMeasurements);
    std::vector<double> trendRandom = linearTrend(ns, randomMeasurements);

    std::cout << "Saving CSV data...\n";
    writeCsv("result/data.csv", ns, offMeasurements, onMeasurements,
             randomMeasurements);

    std::cout << "Saving plot data...\n";
    savePlotData("result/plot.txt", ns, offMeasurements, onMeasurements,
                 randomMeasurements, trendOff, trendOn, trendRandom);

    std::cout << "\nExperiment completed!\n";
    std::cout << "Results saved to:\n";
    std::cout << "  - result/data.csv\n";
    std::cout << "  - result/plot.txt\n";
    std::cout << "\nTo generate a plot, use gnuplot or Python "
              << "with the data files.\n";

    return 0;
}
