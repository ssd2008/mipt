#include <iostream>

// Программа: храним трёхмерную карту измерений и ищем максимум.
// void* нужен как нетипизированный блок памяти, который мог прийти из C API.
// int*** создаёт понятный трёхмерный вид этого блока: слой, строка, столбец.ю

int main() {
  int levels;
  int rows;
  int columns;
  std::cin >> levels >> rows >> columns;
  if (levels <= 0 || rows <= 0 || columns <= 0) {
    return 0;
  }

  int cellCount = levels * rows * columns;
  void* rawData = new int[cellCount];
  int* data = static_cast<int*>(rawData);

  int*** measurements = new int**[levels];
  for (int level = 0; level < levels; ++level) {
    measurements[level] = new int*[rows];
    for (int row = 0; row < rows; ++row) {
      int offset = (level * rows + row) * columns;
      measurements[level][row] = data + offset;
    }
  }

  for (int level = 0; level < levels; ++level) {
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        std::cin >> measurements[level][row][column];
      }
    }
  }

  int maxLevel = 0;
  int maxRow = 0;
  int maxColumn = 0;
  for (int level = 0; level < levels; ++level) {
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        if (measurements[level][row][column] >
            measurements[maxLevel][maxRow][maxColumn]) {
          maxLevel = level;
          maxRow = row;
          maxColumn = column;
        }
      }
    }
  }

  std::cout << measurements[maxLevel][maxRow][maxColumn] << '\n';
  std::cout << maxLevel << ' ' << maxRow << ' ' << maxColumn << '\n';

  for (int level = 0; level < levels; ++level) {
    delete[] measurements[level];
  }
  delete[] measurements;
  delete[] data;
  return 0;
}
