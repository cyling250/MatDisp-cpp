/**
  ******************************************************************************
  节点类的实现
  * @file           : Node.cpp
  * @author         : 35160
  * @date           : 2024/6/19
  ******************************************************************************
  */
#include "Node.h"
#include "Mat.h"

// 初始化类的静态成员变量
int Node::index = 0;
int Node::count = 0;

Node::Node() {
    this->x = 0;
    this->y = 0;
    this->freedom = std::vector<int>(3);
    this->load = std::vector<double>(3);

    this->idx = Node::index;
    Node::index++;
    Node::count++;
}

Node::Node(const Node& node) {
    this->x = node.x;
    this->y = node.y;
    this->freedom = std::vector<int>(3);
    this->load = std::vector<double>(3);

    this->idx = node.idx;
    Node::count++;
}

Node::Node(double const x, double const y) {
    this->x = x;
    this->y = y;
    this->freedom = std::vector<int>(3);
    this->load = std::vector<double>(3);

    this->idx = Node::index;
    Node::index++;
    Node::count++;
}

void Node::set_load(const std::vector<double>& add_load) {
    this->load += add_load;
}

Node::~Node() {
    Node::count--;
}

bool Node::operator<(const Node& node) const {
    // node的<运算符重载采用先比较x 再比较y的方法，主要是为了在Set中区别node
    return this->x < node.x || (this->x >= node.x && this->y < node.y);
}

std::ostream& operator<<(std::ostream& stream, const Node& node) {
    return stream << "[" << node.x << ", " << node.y << "]";
}