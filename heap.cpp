#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct vertex {
  int64_t key;
  std::string value;
  vertex(int64_t key_, std::string value_) : key(key_), value(value_)  {}
};
struct min_heap {
  std::vector<vertex> mas;
  std::unordered_map<int64_t, size_t> tab;
  size_t parent(size_t i) {
    if (i == 0) return 0;
    return (i - 1)/2;
  }
  size_t left(size_t i) { return 2 * i + 1; }
  size_t right(size_t i) { return 2 * i + 2; }
  void add(vertex& n) {
    size_t i, par;
    i = mas.size();
    par = parent(i);
    mas.push_back(n);
    tab[n.key] = i;
    heapify_up(i);
    while (par >= 0 && i > 0) {
      if (mas[i].key < mas[par].key) {
        std::swap(mas[i], mas[par]);
        std::swap(tab[mas[i].key], tab[mas[par].key]);
      }
      i = par;
      par = parent(i);
    }
  }
  void heapify(size_t i) {
    size_t l, r, s;
    while (left(i) < mas.size()) {
      l = left(i), r = right(i), s = i;
      if (r < mas.size() && mas[r].key < mas[s].key) {
        s = r;
      }
      if (l < mas.size() && mas[l].key < mas[s].key) {
        s = l;
      }
      if (s == i) break;
      std::swap(mas[i], mas[s]);
      std::swap(tab[mas[i].key], tab[mas[s].key]);
      i = s;
    }
  }
  void heapify_up(size_t i) {
    size_t p;
    while (parent(i) >= 0) {
      p = parent(i);
      if (mas[p].key > mas[i].key) {
        std::swap(mas[i], mas[p]);
        std::swap(tab[mas[p].key], tab[mas[i].key]);
        i = p;
      } else
        break;
    }
  }
  vertex extract() {
    vertex temp = mas[0];
    tab.erase(mas[0].key);
    mas[0] = mas[mas.size() - 1];
    tab[mas[0].key] = 0;
    mas.pop_back();
    heapify(0);
    return temp;
  }
  bool del(size_t i) {
    if (i == mas.size() - 1) {
      tab.erase(mas[i].key);
      mas.pop_back();
      return true;
    }
    if (i < mas.size()) {
      tab.erase(mas[i].key);
      mas[i] = mas[mas.size() - 1];
      tab[mas[i].key] = i;
      mas.pop_back();
      if (mas[parent(i)].key > mas[i].key)
        heapify_up(i);
      else
        heapify(i);
      return true;
    } else {
      return false;
    }
  }
  std::pair<bool, std::pair<size_t, std::string>> search(int64_t k) {
    if (tab.find(k) != tab.end())
      return std::make_pair(true, std::make_pair(tab[k], mas[tab[k]].value));
    else
      return std::make_pair(false, std::make_pair(0, ""));
  }
  bool set(int64_t k, std::string val) {
    if (tab.find(k) != tab.end())
      mas[tab[k]].value = val;
    else
      return false;
    return true;
  }
  void print(std::ostream& out) {
    if (mas.empty()) {
      out << '_' << std::endl;
      return;
    }
    size_t i = 0, ll = left(i), rr = right(i);
    out << '[' << mas[i].key << ' ' << mas[i].value << ']' << std::endl;
    i = ll;
    while (i < mas.size()) {
      while (i <= rr && i < mas.size()) {
        out << '[' << mas[i].key << ' ' << mas[i].value << ' '
            << mas[parent(i)].key << ']' << ' ';
        ++i;
      }
      int lot = 0;
      for (int j = (int)log2(i); j>= 0; --j) lot = pow(2, j);
      for (; i < lot; i) out << '_' << ' ';
      ll = left(i);
      rr = right(rr);
      out << std::endl;
    }
  }
  size_t min() {
    size_t temp = 0;
    for (size_t i = 0; i < mas.size(); ++i)
      if (mas[i].key < mas[temp].key) temp = i;
    return temp;
  }
  size_t max() {
    size_t temp = 0;
    for (size_t i = 0; i < mas.size(); ++i)
      if (mas[i].key > mas[temp].key) temp = i;
    return temp;
  }
};
int main() {
  std::string str, v;
  min_heap hip;
  hip.mas = {vertex(0,"")};
  hip.mas.clear();
  int64_t k;
  while (std::cin >> str) {
    if (str.empty()) continue;
    if (str == "add") {
      if (std::cin.peek() != '\n') {
        std::cin >> k;
        if (std::cin.peek() != '\n') {
          std::cin >> v;
          if (std::cin.peek() == '\n') {
            if (hip.tab.find(k) != hip.tab.end()) {
              std::cout << "error\n";
              continue;
            }
            vertex n(k,v);
            hip.add(n);
          } else
            std::cout << "error\n";
        } else
          std::cout << "error\n";
      } else
        std::cout << "error\n";
    } else if (str == "set") {
      if (std::cin.peek() != '\n') {
        std::cin >> k;
        if (std::cin.peek() != '\n') {
          std::cin >> v;
          if (std::cin.peek() == '\n') {
            if (!hip.set(k, v)) std::cout << "error\n";
          } else
            std::cout << "error\n";
        } else
          std::cout << "error\n";
      } else
        std::cout << "error\n";
    } else if (str == "delete" && std::cin.peek() != '\n') {
      std::cin >> k;
      if (std::cin.peek() == '\n') {
        if (hip.tab.find(k) == hip.tab.end()) {
          std::cout << "error\n";
          continue;
        }
        if (!hip.del(hip.tab[k])) std::cout << "error\n";
      } else
        std::cout << "error\n";
    } else if (str == "search" && std::cin.peek() != '\n') {
      std::cin >> k;
      if (std::cin.peek() == '\n') {
        std::pair<bool, std::pair<int64_t, std::string>> vx_ = hip.search(k);
        if (vx_.first == true)
          std::cout << 1 << ' ' << vx_.second.first << ' ' << vx_.second.second
                    << std::endl;
        else
          std::cout << 0 << std::endl;
      } else
        std::cout << "error\n";
    } else if (str == "print" && std::cin.peek() == '\n') {
      hip.print(std::cout);
    } else if (str == "extract") {
      if (hip.mas.size() < 1)
        std::cout << "error\n";
      else {
        vertex m_ = hip.extract();
        std::cout << m_.key << ' ' << m_.value << '\n';
      }
    } else if (str == "min") {
      if (hip.mas.size() == 0) {
        std::cout << "error\n";
        continue;
      }
      size_t mn = hip.min();
      std::cout << hip.mas[mn].key << ' ' << mn << ' ' << hip.mas[mn].value
                << std::endl;
    } else if (str == "max") {
      if (hip.mas.size() == 0) {
        std::cout << "error\n";
        continue;
      }
      size_t mx = hip.max();
      std::cout << hip.mas[mx].key << ' ' << mx << ' ' << hip.mas[mx].value
                << std::endl;
    } else {
      if (!str.empty()) std::cout << "error\n";
    }
  }
  return 0;
}
