// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <filesystem>
#include <SFML/Graphics.hpp>

#include "train.h"

namespace fs = std::filesystem;

// Константы
const int N_MIN = 2;
const int N_MAX = 80;
const int RANDOM_TRIALS = 20;
const unsigned long RANDOM_SEED = 42L;

// Цвета (RGB)
const sf::Color COLOR_OFF(46, 134, 193);
const sf::Color COLOR_ON(231, 76, 60);
const sf::Color COLOR_RANDOM(39, 174, 96);

// Генерация диапазона чисел
std::vector<int> range(int from, int toInclusive) {
    std::vector<int> result;
    for (int i = from; i <= toInclusive; ++i) {
        result.push_back(i);
    }
    return result;
}

// Измерение количества операций
double measure(int n, bool allOff, bool allOn, std::mt19937& rng, int trials) {
    long long sum = 0;
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
        train.getLength();
        sum += train.getOpCount();
    }
    return static_cast<double>(sum) / trials;
}

// Линейный тренд (y = a*x + b)
std::vector<double> linearTrend(const std::vector<int>& x, const std::vector<double>& y) {
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

// Квадратичный тренд (y = a*x^2 + b*x + c)
std::vector<double> quadraticTrend(const std::vector<int>& x, const std::vector<double>& y) {
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

    // Решение системы уравнений 3×3 методом Гаусса
    std::vector<std::vector<double>> m = {{s0, s1, s2}, {s1, s2, s3}, {s2, s3, s4}};
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

// Решение системы 3×3
std::vector<double> solve3(std::vector<std::vector<double>>& m, std::vector<double>& v) {
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
            throw std::runtime_error("Singular matrix in quadratic fit");
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

// Запись в CSV
void writeCsv(const std::string& path, const std::vector<int>& ns,
             const std::vector<double>& off, const std::vector<double>& on,
             const std::vector<double>& random) {
    std::ofstream file(path);
    file << "n,all_off,all_on,random_avg\n";
    for (size_t i = 0; i < ns.size(); ++i) {
        file << ns[i] << ','
             << static_cast<int>(std::round(off[i])) << ','
             << static_cast<int>(std::round(on[i])) << ','
             << static_cast<int>(std::round(random[i])) << '\n';
    }
}

// Отрисовка графика
void renderPlot(const std::string& outPath, const std::vector<int>& ns,
                const std::vector<double>& off, const std::vector<double>& on,
                const std::vector<double>& random,
                const std::vector<double>& trendOff,
                const std::vector<double>& trendOn,
                const std::vector<double>& trendRandom) {
    const int width = 1100;
    const int height = 700;
    const int padL = 90;
    const int padR = 40;
    const int padT = 60;
    const int padB = 90;

    int plotW = width - padL - padR;
    int plotH = height - padT - padB;

    // Находим максимальное значение Y для масштабирования
    double maxY = 0;