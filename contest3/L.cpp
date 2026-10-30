#include <iostream>
#include <vector>

// надеюсь сделать просто две кучи это не читерство :)
// еще можно сделать min-max кучу как в теордз

std::vector<int> values;
std::vector<int> min_heap;
std::vector<int> max_heap;
std::vector<int> min_pos;
std::vector<int> max_pos;

void SwapMin(int i, int j) {
  int id = min_heap[i];
  min_heap[i] = min_heap[j];
  min_heap[j] = id;
  min_pos[min_heap[i]] = i;
  min_pos[min_heap[j]] = j;
}

void SwapMax(int i, int j) {
  int id = max_heap[i];
  max_heap[i] = max_heap[j];
  max_heap[j] = id;
  max_pos[max_heap[i]] = i;
  max_pos[max_heap[j]] = j;
}

void UpMin(int i) {
  while (i > 0 && values[min_heap[i]] < values[min_heap[i / 2]]) {
    SwapMin(i, i / 2);
    i = i / 2;
  }
}

void DownMin(int i) {
  int n = static_cast<int>(min_heap.size());
  while (2 * i < n) {
    int j = 2 * i;
    if (j + 1 < n && values[min_heap[j + 1]] < values[min_heap[j]]) {
      ++j;
    }
    if (values[min_heap[i]] <= values[min_heap[j]]) {
      break;
    }
    SwapMin(i, j);
    i = j;
  }
}

void UpMax(int i) {
  while (i > 0 && values[max_heap[i]] > values[max_heap[i / 2]]) {
    SwapMax(i, i / 2);
    i = i / 2;
  }
}

void DownMax(int i) {
  int n = static_cast<int>(max_heap.size());
  while (2 * i < n) {
    int j = 2 * i;
    if (j + 1 < n && values[max_heap[j + 1]] > values[max_heap[j]]) {
      ++j;
    }
    if (values[max_heap[i]] >= values[max_heap[j]]) {
      break;
    }
    SwapMax(i, j);
    i = j;
  }
}

void EraseMin(int i) {
  int last = static_cast<int>(min_heap.size()) - 1;
  SwapMin(i, last);
  int id = min_heap.back();
  min_heap.pop_back();
  min_pos[id] = -1;

  if (i < static_cast<int>(min_heap.size())) {
    DownMin(i);
    UpMin(i);
  }
}

void EraseMax(int i) {
  int last = static_cast<int>(max_heap.size()) - 1;
  SwapMax(i, last);
  int id = max_heap.back();
  max_heap.pop_back();
  max_pos[id] = -1;

  if (i < static_cast<int>(max_heap.size())) {
    DownMax(i);
    UpMax(i);
  }
}

void Insert(int x) {
  int id = static_cast<int>(values.size());
  values.push_back(x);

  min_heap.push_back(id);
  max_heap.push_back(id);

  min_pos.push_back(static_cast<int>(min_heap.size()) - 1);
  max_pos.push_back(static_cast<int>(max_heap.size()) - 1);

  UpMin(min_pos[id]);
  UpMax(max_pos[id]);

  std::cout << "ok\n";
}

void ExtractMin() {
  if (min_heap.empty()) {
    std::cout << "error\n";
    return;
  }

  int id = min_heap[0];
  std::cout << values[id] << '\n';

  EraseMin(0);
  EraseMax(max_pos[id]);
}

void ExtractMax() {
  if (max_heap.empty()) {
    std::cout << "error\n";
    return;
  }

  int id = max_heap[0];
  std::cout << values[id] << '\n';

  EraseMax(0);
  EraseMin(min_pos[id]);
}

void Clear() {
  while (!min_heap.empty()) {
    min_heap.pop_back();
  }
  while (!max_heap.empty()) {
    max_heap.pop_back();
  }
  std::cout << "ok\n";
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int q;
  std::cin >> q;

  for (int i = 0; i < q; ++i) {
    int op;
    std::cin >> op;

    if (op == 0) {
      int x;
      std::cin >> x;
      Insert(x);
    } else if (op == 1) {
      ExtractMin();
    } else if (op == 2) {
      if (min_heap.empty()) {
        std::cout << "error\n";
      } else {
        std::cout << values[min_heap[0]] << '\n';
      }
    } else if (op == 3) {
      ExtractMax();
    } else if (op == 4) {
      if (max_heap.empty()) {
        std::cout << "error\n";
      } else {
        std::cout << values[max_heap[0]] << '\n';
      }
    } else if (op == 5) {
      std::cout << min_heap.size() << '\n';
    } else if (op == 6) {
      Clear();
    }
  }
}
