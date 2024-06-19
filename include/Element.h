/**
  ******************************************************************************
  请输入描述
  * @file           : Element.h
  * @author         : 35160
  * @date           : 2024/6/19
  ******************************************************************************
  */
//
// Created by 35160 on 2024/6/19.
//

#ifndef MATDISP_INCLUDE_ELEMENT_H
#define MATDISP_INCLUDE_ELEMENT_H

#include<vector>
#include<iostream>
#include"Node.h"

class Element {
    /**
     * 索引
     */
    static int index;
    /**
     * 个数
     */
    static int count;
public:
    /**
     * 索引
     */
    int idx;
    /**
     * 所包含的两个节点的指针
     * 必须用指针 因为需要保证一个节点是唯一的
     */
    Node* nodes[2];
    /**
     * 两个节点的连接方式
     */
    std::vector<bool> linkWay;
    /**
     * 定位向量及自由度
     */
    std::vector<int> freedom;
    /**
     * 弹性模量
     */
    double E;
    /**
     * 转动惯量
     */
    double I;
    /**
     * 长度
     */
    double L;
    /**
     * 截面积
     */
    double A;
    /**
     * 杆件方向与x轴夹角余弦值
     */
    double cosA;
    /**
    * 杆件方向与x轴夹角正弦值
    */
    double sinA;
    /**
     * 整体坐标系 单元刚度矩阵
     */
    std::vector<std::vector<double>> globalKe;
    /**
     * 坐标变换矩阵
     */
    std::vector<std::vector<double>> T;
    /**
     * 约束条件
     */
    std::vector<bool> restrain;
    /**
     * 局部坐标下荷载列阵
     */
    std::vector<double> load;
    /**
     * 杆端位移
     */
    std::vector<double> delta;
    /**
     * 杆端内力
     */
    std::vector<double> force;

    /**
     * 构造函数
     * @param node1 开始node的指针
     * @param node2 终止node的指针
     * @param linkWay1 与node1的连接方式 False为刚接 True为铰接
     * @param linkWay2 与node2的连接方式
     * @param E 弹性模量
     * @param I 转动惯量
     * @param A 截面面积
     */
    explicit Element(
            Node* node1 = nullptr,
            Node* node2 = nullptr,
            bool linkWay1 = false,
            bool linkWay2 = false,
            double E = 1,
            double I = 1,
            double A = 1);

    /**
     * 构造函数
     */
    Element(
            Node&& node1,
            Node&& node2,
            bool linkWay1 = false,
            bool linkWay2 = false,
            double E = 1,
            double I = 1,
            double A = 1
    );

    /**
     * 获取杆件长度
     * @return
     */
    double getLength();

    /**
     * 获取局部坐标系下的单元刚度矩阵
     * @return
     */
    std::vector<std::vector<double>> getLocalKe();

    /**
     * 为杆件施加约束
     */
    void addRestrain(const std::vector<bool>&);

    /**
     * 为杆件施加荷载
     */
    void addLoad(const std::vector<double>&);

    /**
     * < 运算符重载 使其满足Set
     */
    bool operator<(const Element& node) const;

    ~Element();
};

/**
 * << 运算符重载
 */
std::ostream& operator<<(std::ostream& stream, const Element& element);

#endif //MATDISP_INCLUDE_ELEMENT_H
