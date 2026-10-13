#include <cstring>
#include <iostream>
#include <limits>

void deposit(int& balance) {
  int amount;
  std::cin >> amount;
  if (amount <= 0) {
    std::cout << "error\n";
    return;
  }
  balance += amount;
}

void withdraw(int& balance) {
  int amount;
  std::cin >> amount;
  if (amount <= 0 || amount > balance) {
    std::cout << "error\n";
    return;
  }
  balance -= amount;
}

void printBalance(int& balance) { std::cout << balance << '\n'; }

void reset(int& balance) { balance = 0; }

void doubleBalance(int& balance) { balance *= 2; }

int main() {
  using Operation = void (*)(int&);

  const char* commands[] = {"deposit", "withdraw", "balance", "reset",
                            "double"};
  Operation operations[] = {deposit, withdraw, printBalance, reset,
                            doubleBalance};
  const int commandCount = sizeof(commands) / sizeof(commands[0]);
  int balance = 0;
  char command[32];

  while (true) {
    std::cin.width(sizeof(command));
    if (!(std::cin >> command)) {
      break;
    }
    if (std::strcmp(command, "stop") == 0) {
      break;
    }

    bool found = false;
    for (int i = 0; i < commandCount; ++i) {
      if (std::strcmp(command, commands[i]) == 0) {
        operations[i](balance);
        found = true;
        break;
      }
    }

    if (!found) {
      std::cout << "error\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return 0;
}
