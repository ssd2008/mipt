#include <iostream>
#include <string>
#include <vector>

std::vector<long long> v(1);
std::vector<int> id(1);
std::vector<int> pos;

void SwapProMax(int i, int j) {
  std::swap(v[i], v[j]);
  std::swap(id[i], id[j]);
  pos[id[i]] = i;
  pos[id[j]] = j;
}

void PushUp(int i) {
  if (i > 1 && v[i] < v[i / 2]) {
    SwapProMax(i, i / 2);
    PushUp(i / 2);
  }
}

void PushDown(int i) {
  int n = v.size();
  int child = i * 2;

  if (child >= n) {
    return;
  }
  if (child + 1 < n && v[child + 1] < v[child]) {
    child++;
  }

  if (v[child] < v[i]) {
    SwapProMax(i, child);
    PushDown(child);
  }
}

void Insert(long long x, int pos_id) {
  v.push_back(x);
  id.push_back(pos_id);
  pos[pos_id] = v.size() - 1;
  PushUp(v.size() - 1);
}

void DecreaseKey(int i, long long d) {
  int j = pos[i];
  v[j] -= d;
  PushUp(j);
}

void GetMin() { std::cout << v[1] << '\n'; }

void ExtractMin() {
  SwapProMax(1, v.size() - 1);
  pos[id.back()] = -1;
  v.pop_back();
  id.pop_back();

  if (v.size() > 1) {
    PushDown(1);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int q;
  std::cin >> q;
  pos.resize(q + 1, -1);

  for (int j = 0; j < q; ++j) {
    std::string s;
    std::cin >> s;

    if (s == "insert") {
      long long x;
      std::cin >> x;
      Insert(x, j + 1);
    } else if (s == "decreaseKey") {
      int i;
      long long d;
      std::cin >> i >> d;
      DecreaseKey(i, d);
    } else if (s == "getMin") {
      GetMin();
    } else if (s == "extractMin") {
      ExtractMin();
    }
  }
}
