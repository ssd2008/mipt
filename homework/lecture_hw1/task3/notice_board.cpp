#include <cstring>
#include <iostream>

const int kRows = 5;
const int kColumns = 10;

bool isInside(int x, int y) {
  return x >= 0 && x < kColumns && y >= 0 && y < kRows;
}

void clear(char board[][kColumns]) {
  for (int y = 0; y < kRows; ++y) {
    for (int x = 0; x < kColumns; ++x) {
      board[y][x] = ' ';
    }
  }
}

bool put(char board[][kColumns], int x, int y, char symbol) {
  if (!isInside(x, y)) {
    return false;
  }
  board[y][x] = symbol;
  return true;
}

bool text(char board[][kColumns], int x, int y, const char* word) {
  int length = std::strlen(word);
  if (length == 0 || !isInside(x, y) || !isInside(x + length - 1, y)) {
    return false;
  }

  for (int i = 0; i < length; ++i) {
    put(board, x + i, y, word[i]);
  }
  return true;
}

void print(const char board[][kColumns]) {
  for (int y = 0; y < kRows; ++y) {
    for (int x = 0; x < kColumns; ++x) {
      std::cout << board[y][x];
    }
    std::cout << '\n';
  }
}

int main() {
  char board[kRows][kColumns];
  clear(board);

  char command[16];
  while (true) {
    std::cin.width(sizeof(command));
    if (!(std::cin >> command)) {
      break;
    }

    if (std::strcmp(command, "quit") == 0) {
      break;
    }

    if (std::strcmp(command, "clear") == 0) {
      clear(board);
    } else if (std::strcmp(command, "put") == 0) {
      int x;
      int y;
      char symbol;
      std::cin >> x >> y >> symbol;
      if (!put(board, x, y, symbol)) {
        std::cout << "error\n";
      }
    } else if (std::strcmp(command, "text") == 0) {
      int x;
      int y;
      char word[256];
      std::cin >> x >> y;
      std::cin.width(sizeof(word));
      std::cin >> word;
      if (!text(board, x, y, word)) {
        std::cout << "error\n";
      }
    } else if (std::strcmp(command, "print") == 0) {
      print(board);
    } else {
      std::cout << "error\n";
    }
  }

  return 0;
}
