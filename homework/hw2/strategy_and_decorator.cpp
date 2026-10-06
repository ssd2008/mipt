#include <iostream>
#include <functional>

// Стратегия
using Strategy = std::function<int(int, int)>;

class Calculator {
private:
  Strategy strategy_;

public:
  Calculator(Strategy strategy)
      : strategy_(strategy) {}

  void setStrategy(Strategy strategy) {
    strategy_ = strategy;
  }

  int calculate(int a, int b) const {
    return strategy_(a, b);
  }
};

// Декоратор
using Operation = std::function<int(int)>;

Operation addLogging(Operation operation) {
  return [operation](int x) {
    std::cout << "Input: " << x << '\n';
    int result = operation(x);
    std::cout << "Result: " << result << '\n';
    return result;
  };
}

// Ещё декоратор: напоминаю какая группа самая мощная на физтехе
Operation ResultWithBase(Operation operation) {
  return [operation](int x) {
    std::cout << "вместе стая поросят - айайай 360\n";
    return operation(x);
  };
}


int main() {
  // Стратегия
  Strategy sum = [](int a, int b) {
    return a + b;
  };
  Strategy multiply = [](int a, int b) {
    return a * b;
  };

  Calculator calculator(sum);
  std::cout << "Strategy:\n";
  std::cout << calculator.calculate(3, 4) << '\n';

  calculator.setStrategy(multiply);
  std::cout << calculator.calculate(3, 4) << '\n';


  // Декоратор
  Operation square = [](int x) {
    return x * x;
  };
  Operation decoratedSquare = addLogging(square);
  std::cout << "\nDecorator:\n";
  decoratedSquare(5);


  Operation fullyDecorated = addLogging(
      ResultWithBase(square)
  );
  std::cout << "\nSeveral decorators:\n";
  fullyDecorated(5);

  return 0;
}