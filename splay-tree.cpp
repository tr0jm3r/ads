#include <math.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include<sstream>
#include<string>

struct vertex {
  int64_t key;
  std::string val;
  vertex* parent = nullptr;
  vertex* right = nullptr;
  vertex* left = nullptr;
  vertex() {
    delete left;
    delete right;
  }
};
struct sptree {
  vertex* root = nullptr;
  void rightrot(vertex* one) {
    vertex* two = one->left;
    one->left = two->right;
    if (two->right != nullptr) {
      two->right->parent = one;
    }
    two->parent = one->parent;
    if (one == root) {
      root = two;
    } else if (one->parent->left == one && one->parent != nullptr) {
      one->parent->left = two;
    } else if (one->parent != nullptr) {
      one->parent->right = two;
    }
    two->right = one;
    one->parent = two;
  }
  void leftrot(vertex* one) {
    vertex* two = one->right;
    one->right = two->left;
    if (two->left != nullptr) {
      two->left->parent = one;
    }
    two->parent = one->parent;
    if (one == root) {
      root = two;
    } else if (one->parent != nullptr && one->parent->left == one) {
      one->parent->left = two;
    } else if (one->parent != nullptr) {
      one->parent->right = two;
    }
    two->left = one;
    one->parent = two;
  }
  void splay(vertex* one) {
    if (one->parent == nullptr) return;
    if (one->parent->parent == nullptr) {
      if (one->parent->left == one) {
        rightrot(one->parent);
        return;
      } else {
        leftrot(one->parent);
        return;
      }
    } else if (one->parent->parent != nullptr) {
      if (one->parent->left == one &&
          one->parent->parent->left == one->parent) {
        rightrot(one->parent->parent);
        rightrot(one->parent);
        splay(one);
        return;
      }
      if (one->parent->right == one &&
          one->parent->parent->right == one->parent) {
        leftrot(one->parent->parent);
        leftrot(one->parent);
        splay(one);
        return;
      }
      if (one->parent->right == one &&
          one->parent->parent->left == one->parent) {
        leftrot(one->parent);
        rightrot(one->parent);
        splay(one);
        return;
      }
      if (one->parent->left == one &&
          one->parent->parent->right == one->parent) {
        rightrot(one->parent);
        leftrot(one->parent);
        splay(one);
        return;
      }
    }
  }
  vertex* max() {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      prev = curr;
      curr = curr->right;
    }
    if (prev == nullptr) {
      return nullptr;
    }
    splay(prev);
    return prev;
  }
  vertex* min() {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      prev = curr;
      curr = curr->left;
    }
    if (prev == nullptr) {
      return nullptr;
    }
    splay(prev);
    return prev;
  }
  vertex* merge(vertex* ltree, vertex* rtree) {
    ltree->parent = nullptr;
    vertex* curr = ltree;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      prev = curr;
      curr = curr->right;
    }
    splay(prev);
    prev->right = rtree;
    if (rtree != nullptr) {
      rtree->parent = prev;
    }
    return prev;
  }
  bool del(int64_t vx) {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      if (curr->key < vx) {
        prev = curr;
        curr = curr->right;
        continue;
      }
      if (curr->key == vx) {
        splay(curr);
        if (curr->right == nullptr && curr->left == nullptr) {
          delete curr;
          root = nullptr;
          return 1;
        } else if (curr->right == nullptr) {
          root = curr->left;
          root->parent = nullptr;
          curr->left = nullptr;
          delete curr;
          return 1;
        } else if (curr->left == nullptr) {
          root = curr->right;
          root->parent = nullptr;
          curr->right = nullptr;
          delete curr;
          return 1;
        } else {
          vertex* n_root = merge(curr->left, curr->right);
          curr->left = nullptr;
          curr->right = nullptr;
          delete root;
          root = n_root;
          return 1;
        }
      }
      if (curr->key > vx) {
        prev = curr;
        curr = curr->left;
        continue;
      }
    }
    return 0;
  }
  vertex* search(int64_t vx) {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      if (curr->key < vx) {
        prev = curr;
        curr = curr->right;
        continue;
      }
      if (curr->key == vx) {
        splay(curr);
        return curr;
      }
      if (curr->key > vx) {
        prev = curr;
        curr = curr->left;
        continue;
      }
    }
    if (prev == nullptr)
      return prev;
    else {
      splay(prev);
      return nullptr;
    }
    return nullptr;
  }
  bool set(int64_t vxk, std::string vxv) {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      if (curr->key < vxk) {
        prev = curr;
        curr = curr->right;
        continue;
      }
      if (curr->key == vxk) {
        splay(curr);
        curr->val = vxv;
        return curr;
      }
      if (curr->key > vxk) {
        prev = curr;
        curr = curr->left;
        continue;
      }
    }
    return false;
  }
  bool insert(int64_t vxk, std::string value) {
    vertex* curr = root;
    vertex* prev = nullptr;
    while (curr != nullptr) {
      if (curr->key < vxk) {
        prev = curr;
        curr = curr->right;
        continue;
      }
      if (curr->key == vxk) return 0;
      if (curr->key > vxk) {
        prev = curr;
        curr = curr->left;
        continue;
      }
    }
    if (prev == nullptr) {
      root = new vertex();
      root->key = vxk;
      root->val = value;
    } else {
      if (prev->key < vxk) {
        prev->right = new vertex();
        prev->right->key = vxk;
        prev->right->val = value;
prev->right->parent = prev;
splay(prev->right);
}
if (prev->key > vxk) {
  prev->left = new vertex();
  prev->left->key = vxk;
  prev->left->val = value;
  prev->left->parent = prev;
  splay(prev->left);
}
}
return 1;
}
void print(std::ostream& out) {
  vertex* curr = root;
  size_t ctr = 1, last = 1;
  bool nl = 0, np = 0;
  if (!curr) {
    out << '_' << std::endl;
    return;
  }
  std::queue<vertex*> spisok;
  spisok.push(curr);
  while (!spisok.empty()) {
    if (np) {
      out << ' ';
    } else {
      np = 1;
    }
    if (spisok.front()) {
      if (spisok.front()->parent) {
        out << '[' << spisok.front()->key << ' ' << spisok.front()->val << ' '
            << spisok.front()->parent->key << ']';
      }
      if (!spisok.front()->parent) {
        out << '[' << spisok.front()->key << ' ' << spisok.front()->val << ']';
      }
      if (spisok.front()->left || spisok.front()->right) nl = 1;
    } else
      out << '_';
    ctr--;
    if (!ctr) {
      if (nl) {
        last *= 2;
        ctr = last;
        out << std::endl;
        nl = 0;
        np = 0;
      } else {
        out << std::endl;
        break;
      }
    }
    if (spisok.front()) {
      spisok.push(spisok.front()->left);
      spisok.push(spisok.front()->right);
    } else {
      spisok.push(0);
      spisok.push(0);
    }
    spisok.pop();
  }
}
sptree() { delete root; }
}
;
int main() {
  std::string str, v;
  sptree tree;
  int64_t k;
  while (std::cin >> str) {
    if (str.empty()) continue;
    if (str == "add") {
      if (std::cin.peek() != '\n') {
        std::cin >> k;
        if (std::cin.peek() != '\n') {
          std::cin >> v;
          if (std::cin.peek() == '\n') {
            if (!tree.insert(k, v)) std::cout << "error\n";
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
            if (!tree.set(k, v)) std::cout << "error\n";
          } else
            std::cout << "error\n";
        } else
          std::cout << "error\n";
      } else
        std::cout << "error\n";
    } else if (str == "delete" && std::cin.peek() != '\n') {
      std::cin >> k;
      if (std::cin.peek() == '\n') {
        if (!tree.del(k)) std::cout << "error\n";
      } else
        std::cout << "error\n";
    } else if (str == "search" && std::cin.peek() != '\n') {
      std::cin >> k;
      if (std::cin.peek() == '\n') {
        vertex* vx_ = tree.search(k);
        if (vx_ != nullptr)
          std::cout << 1 << ' ' << vx_->val << std::endl;
        else
          std::cout << 0 << std::endl;
      } else
        std::cout << "error\n";
    } else if (str == "print" && std::cin.peek() == '\n') {
      tree.print(std::cout);
    } else if (str == "min") {
      vertex* m_ = tree.min();
      if (m_ == nullptr)
        std::cout << "error\n";
      else
        std::cout << m_->key << ' ' << m_->val << '\n';
    } else if (str == "max") {
      vertex* m_ = tree.max();
      if (m_ == nullptr)
        std::cout << "error\n";
      else
        std::cout << m_->key << ' ' << m_->val << '\n';
    } else {
      if (!str.empty()) std::cout << "error\n";
    }
  }
  return 0;
}