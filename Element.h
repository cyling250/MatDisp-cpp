#ifndef ELEMENT_H
#define ELEMENT_H
#include"Node.h"
#include<vector>
typedef __int64 DDWORD;
using namespace std;
class Element {
public:
	static int number;//单元总数，声明为静态数据成员
	int NO;//是否被Structure类管理的标志
	vector<DDWORD> nodes;//结点指针
	vector<bool> link_way;
	vector<int> freedom;//自由度数组
	double E;//弹性模量
	double I;//截面惯性矩
	double L;//杆件长度
	double A;//截面面积
	vector<vector<double>> k_e; //杆件的局部刚度矩阵
	vector<vector<double>> T; //杆件的坐标转换矩阵
	vector<int> restrain;//杆件约束
	vector<double> load;//杆件荷载
	vector<double> delta;//杆端位移
	vector<double> force;//杆端内力

	Element();
	void add_restrain(vector<int>);//Add restrain to element
	void set_load(vector<double>);//Element Load
	void print();
	~Element();
};
#endif