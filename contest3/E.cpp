#include <deque>
#include <iostream>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;
  std::deque<int> l;
  std::deque<int> r;

  for (int i = 0; i < n; ++i) {
    char op;
    std::cin >> op;

    if (op == '-') {
      std::cout << l.front() << '\n';
      l.pop_front();
    } else {
      int x;
      std::cin >> x;
      if (op == '+') {
        r.push_back(x);
      } else {
        r.push_front(x);
      }
    }

    if (l.size() < r.size()) {
      l.push_back(r.front());
      r.pop_front();
    }
    if (l.size() > r.size() + 1) {
      r.push_front(l.back());
      l.pop_back();
    }
  }
}
