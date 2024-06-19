/**
  ******************************************************************************
  请输入描述
  * @file           : Element.cpp
  * @author         : 35160
  * @date           : 2024/6/19
  ******************************************************************************
  */
#include "Element.h"
#include "Mat.h"
#include <cmath>

// 初始化静态成员
int Element::count = 0;
int Element::index = 0;

Element::Element(
        Node* node1,
        Node* node2,
        bool linkWay1,
        bool linkWay2,
        double E,
        double I,
        double A)
        :nodes{nullptr, nullptr}, L(0), linkWay(2), freedom(6), E(E), I(I), A(A), restrain(6), load(6), delta(6),
         force(6) {
    if (node1 == nullptr || node2 == nullptr)
        throw std::runtime_error("Can't initialize node with nullptr");
    this->nodes[0] = node1;
    this->nodes[1] = node2;
    this->linkWay[0] = linkWay1;
    this->linkWay[1] = linkWay2;

    this->getLength(); // 计算长度
    this->cosA = (this->nodes[1]->x - this->nodes[0]->x) / this->L;
    this->sinA = (this->nodes[1]->y - this->nodes[0]->y) / this->L;
    this->T = {
            {cosA, sinA, 0, 0, 0, 0},
            {-sinA, cosA, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, cosA, sinA, 0},
            {0, 0, 0, -sinA, cosA, 0},
            {0, 0, 0, 0, 0, 1}
    }; // 坐标变换矩阵

    this->globalKe = this->T * this->getLocalKe() * this->T; // 计算整体坐标系下单元刚度矩阵

    this->idx = Element::index;
    Element::count++;
    Element::index++;
}

Element::Element(
        Node&& node1,
        Node&& node2,
        bool linkWay1,
        bool linkWay2,
        double E,
        double I,
        double A)
        :nodes{nullptr, nullptr}, L(0), linkWay(2), freedom(6), E(E), I(I), A(A), restrain(6), load(6), delta(6),
         force(6) {
    this->nodes[0] = &node1;
    this->nodes[1] = &node2;

    this->linkWay[0] = linkWay1;
    this->linkWay[1] = linkWay2;

    this->getLength(); // 计算长度
    this->cosA = (this->nodes[1]->x - this->nodes[0]->x) / this->L;
    this->sinA = (this->nodes[1]->y - this->nodes[0]->y) / this->L;
    this->T = {
            {cosA, sinA, 0, 0, 0, 0},
            {-sinA, cosA, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, cosA, sinA, 0},
            {0, 0, 0, -sinA, cosA, 0},
            {0, 0, 0, 0, 0, 1}
    }; // 坐标变换矩阵

    this->globalKe = this->T * this->getLocalKe() * this->T; // 计算整体坐标系下单元刚度矩阵

    this->idx = Element::index;
    Element::count++;
    Element::index++;
}

double Element::getLength() {
    if (this->L == 0) {
        // 如果未计算L
        this->L = sqrt(
                pow((this->nodes[0]->x - this->nodes[1]->x), 2) +
                        pow((this->nodes[0]->x - this->nodes[1]->x), 2)
        );
    }
    if (this->L == 0)throw std::runtime_error("invalid element with length 0");
    return this->L;
}

Element::~Element() {
    Element::count--;
}

bool Element::operator<(const Element& element) const {
    return (this->L < element.L) || // 如果长度不相等
            (this->nodes[0] < element.nodes[0] ||
                    this->nodes[0] >= element.nodes[0] && this->nodes[1] < element.nodes[1]); // 长度相等则比较节点
}

std::vector<std::vector<double>> Element::getLocalKe() {
    if (this->linkWay[0] && this->linkWay[1])
        return {
                {E * A / L, 0, 0, -E * A / L, 0, 0},
                {0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0},
                {-E * A / L, 0, 0, E * A / L, 0, 0},
                {0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0}
        };
    else
        return {
                {E * A / L, 0, 0, -E * A / L, 0, 0},
                {0, 12 * E * I / pow(L, 3), 6 * E * I / pow(L, 2),
                        0, -12 * E * I / pow(L, 3), 6 * E * I / pow(L, 2)},
                {0, 6 * E * I / pow(L, 2), 4 * E * I / L, 0, -6 * E * I / pow(L, 2), 2 * E * I / L},
                {-E * A / L, 0, 0, E * A / L, 0, 0},
                {0, -12 * E * I / pow(L, 3), -6 * E * I / pow(L, 2),
                        0, 12 * E * I / pow(L, 3), -6 * E * I / pow(L, 2)},
                {0, 6 * E * I / pow(L, 2), 2 * E * I / L, 0, -6 * E * I / pow(L, 2), 4 * E * I / L}
        };
}

void Element::addRestrain(const std::vector<bool>& data) {
    this->restrain = data;
}

void Element::addLoad(const std::vector<double>& data) {
    if (data[0] == 0) {
        // 0表示杆上集中荷载，杆上集中荷载的形式[0,Fp,a],Fp是力的大小，a是集中力到杆件正方向左端的距离
        double b = this->L - data[2];
        double a = data[2];
        double fp = data[1];
        std::vector<double> arr = {
                0, -fp * pow(b, 2) * (1 + 2 * a / L) / pow(L, 2), -fp * a * pow(b, 2) / pow(L, 2),
                0, -fp * pow(a, 2) * (1 + 2 * b / L) / pow(L, 2), fp * a * pow(b, 2) / pow(L, 2)
        };
        this->load += arr;
    } else if (data[0] == 1) {
        // 1表示杆端弯矩，Load[1]存放杆左端弯矩,Load[2]存放杆右端弯矩
        this->load[2] -= data[1];
        this->load[5] -= data[2];
    } else if (data[0] == 2) {
        // 2表示均布荷载，Load[1]存放均布荷载的大小
        double q = data[1];
        std::vector<double> arr = {
                0., -q * L / 2., -q * pow(L, 2) / 12.,
                0., -q * L / 2., q * pow(L, 2) / 12.
        };
        this->load += arr;
    } else if (data[0] == 3) {
        // 3表示具有等斜度的均布荷载
        double q = data[1];
        std::vector<double> arr = {
                0., -q * L / 2. * cosA, -q * pow(L, 2) / 12. * pow(cosA, 2),
                0., -q * L / 2. * cosA, q * pow(L, 2) / 12. * pow(cosA, 2)
        };
        this->load += this->T * arr;
    }
}

std::ostream& operator<<(std::ostream& stream, const Element& element) {
    stream << *element.nodes[0] << " -> " << *element.nodes[1];
    return stream;
}