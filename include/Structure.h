/**
  ******************************************************************************
  请输入描述
  * @file           : Structure.h
  * @author         : 35160
  * @date           : 2024/6/19
  ******************************************************************************
  */

#ifndef MATDISP_STRUCTURE_H
#define MATDISP_STRUCTURE_H

#include"Element.h"
#include<set>
#include<vector>
#include<map>

class Structure {
public:
    /**
     * 单元集合
     */
    std::set<Element> elements;
    /**
     * 刚度矩阵的大小
     */
    int sizeOfK;
    /**
     * 位移定位矩阵
     */
    std::vector<bool> inResult;
    /**
     * 原始的整体刚度矩阵
     */
    std::vector<std::vector<double>> K;
    /**
     * 原始的荷载列阵
     */
    std::vector<double> load;
    /**
     * 经过划行划列后的整体刚度矩阵
     */
    std::vector<std::vector<double>> calcK;
    /**
    * 经过划行划列后的荷载列阵
    */
    std::vector<double> calcLoad;
    /**
     * 位移结果矩阵
     */
    std::vector<double> deltaDisp;

    Structure();

    /**
     * 连接两个点, 构造杆件
     * @param linkWay1
     * @param linkWay2
     * @param E
     * @param I
     * @param A
     */
    Element* link(
            Node* node1 = nullptr,
            Node* node2 = nullptr,
            bool linkWay1 = false,
            bool linkWay2 = false,
            double E = 1,
            double I = 1,
            double A = 1);

    /**
     * 获得当前的连接Map
     * <Node*, Set<Element*>>
     * 此map需要能直接操作结构数据
     */
    std::map<Node*, std::set<Element*>> getLinkDict();

    /**
     * 为每个杆件生成定位向量并且编号
     * @return
     */
    int getFreedom();

    /**
     * 计算整体刚度矩阵
     * @param forceCalc 是否强制更新
     * @return 整体刚度矩阵*
     */
    const std::vector<std::vector<double>>* getEntireK(bool forceCalc = false);

    /**
     * 按照施加的荷载 生成整体荷载列阵
     */
    void loadProcess();

    /**
     *  调用前置条件: 整体刚度矩阵已经形成 约束已经添加
     *  对整体刚度矩阵和荷载列阵进行划行划列
     */
    void freedomProcess();

    /**
     * 求解
     */
    void resolve();

    ~Structure();
};

#endif //MATDISP_STRUCTURE_H
