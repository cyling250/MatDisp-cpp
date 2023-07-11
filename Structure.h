#ifndef STRUCTURE_H
#define STRUCTURE_H
#include<vector>
#include"Node.h"
#include"Element.h"
typedef __int64 DDWORD;
class Structure {
public:
	//结构类需要管理杆件与结点类，但不能由结构类构造
	vector<DDWORD> nodes;
	vector<DDWORD> elements;

	int size_of_K;//整体刚度矩阵维度
	vector<int> in_result;//位移定位矩阵，协助内力计算
	vector<vector<double>> K;//整体刚度矩阵
	vector<double> load;//整体荷载列阵
	vector<double>result;//整体结果位移矩阵

	Structure();
	void link(Node&, Node&, Element&, bool link_way1 = false, bool link_way2 = false, double E = 1, double I = 1, double A = 1);//链接杆件
	int index_node(Node*);//在结点数组中查找结点，找到则返回结点编号，否则返回NULL
	int get_freedom();//为每根杆件生成定位向量
	void get_K();//构建整体刚度矩阵
	void get_load();//构建荷载列阵
	void add_restrain();//添加约束，对整体刚度矩阵与整体荷载列阵划行划列
	~Structure();
};
#endif