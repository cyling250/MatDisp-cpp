/**
  ******************************************************************************
  请输入描述
  * @file           : Node.h
  * @author         : 35160
  * @date           : 2024/6/19
  ******************************************************************************
  */

#ifndef MATDISP_INCLUDE_NODE_H
#define MATDISP_INCLUDE_NODE_H

#include<vector>
#include<ostream>

class Node {
    /**
    * 个数
    */
    static int count;
    /**
     * 索引
     */
    static int index;
public:
    /**
     * 索引
     */
    int idx{};
    /**
     * x坐标
     */
    double x;
    /**
     * y坐标
     */
    double y;
    /**
     * 节点自由度编号
     * 一般有三个元素 半铰接点有特殊处理函数
     */
    std::vector<int> freedom;
    /**
     * 荷载列阵
     * 节点上的荷载只考虑前三个 节点平动和节点转动
     */
    std::vector<double> load;

    Node();

    Node(double, double);

    /**
     * 深拷贝构造
     */
    Node(const Node&);

    /**
     * 为节点施加荷载
     */
    void set_load(const std::vector<double>&);

    ~Node();

    /**
     * < 运算符重载 使其满足Set
     */
    bool operator<(const Node& node) const;
};

/**
 * << 运算符重载
 * @param stream 输出流
 * @param node 节点
 * @return 输出流
 */
std::ostream& operator<<(std::ostream& stream, const Node& node);

#endif //MATDISP_INCLUDE_NODE_H
