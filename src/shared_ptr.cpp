#include <iostream>
#include <memory>

class Node {
public:
  std::shared_ptr<Node> next;
  int value;

  Node(int val) : value(val) {
  }
};

void createCycle() {
  std::shared_ptr<Node> node1 = std::make_shared<Node>(1);
  std::shared_ptr<Node> node2 = std::make_shared<Node>(2);

  node1->next = node2;
  node2->next = node1; // 循环引用

  std::cout << node1.use_count() << std::endl;
  std::cout << node2.use_count() << std::endl;
  // 这里会导致内存泄漏，因为 node1 和 node2 互相持有
}

void createWeakReference() {
  std::shared_ptr<Node> node1 = std::make_shared<Node>(1);
  std::weak_ptr<Node> weakNode = node1; // 使用 weak_ptr

  {
    std::shared_ptr<Node> node2 = node1; // node2 共享所有权
    std::cout << "Node value: " << node2->value << std::endl;
  } // node2 超出作用域，node1 的引用计数减一

  node1.reset(); // 销毁 node1
  if (auto sharedNode = weakNode.lock()) {
    // 尝试升级
    std::cout << "Node value after node2 goes out of scope: " << sharedNode->value << std::endl;
  } else {
    std::cout << "Node has been destroyed." << std::endl;
  }
}

int main() {
  createCycle(); // 演示循环引用
  createWeakReference(); // 演示 weak_ptr
  return 0;
}
