#include <iomanip>
#include <iostream>

double getAverage(const int* temperatures, int size) {
  long long sum = 0;
  for (int i = 0; i < size; ++i) {
    sum += temperatures[i];
  }
  return static_cast<double>(sum) / size;
}

void getMinMax(const int* temperatures, int size, int& minTemperature,
               int& maxTemperature) {
  minTemperature = temperatures[0];
  maxTemperature = temperatures[0];

  for (int i = 1; i < size; ++i) {
    if (temperatures[i] < minTemperature) {
      minTemperature = temperatures[i];
    }
    if (temperatures[i] > maxTemperature) {
      maxTemperature = temperatures[i];
    }
  }
}

int countFrostyDays(const int* temperatures, int size) {
  int frostyDays = 0;
  for (int i = 0; i < size; ++i) {
    if (temperatures[i] < 0) {
      ++frostyDays;
    }
  }
  return frostyDays;
}

int main() {
  int days;
  std::cin >> days;
  if (days <= 0) {
    return 0;
  }

  int* temperatures = new int[days];
  for (int i = 0; i < days; ++i) {
    std::cin >> temperatures[i];
  }

  int minTemperature;
  int maxTemperature;
  getMinMax(temperatures, days, minTemperature, maxTemperature);

  std::cout << std::fixed << std::setprecision(1)
            << getAverage(temperatures, days) << '\n';
  std::cout << minTemperature << ' ' << maxTemperature << '\n';
  std::cout << countFrostyDays(temperatures, days) << '\n';

  delete[] temperatures;
  return 0;
}
