#ifndef NODE_H
#define NODE_H
#include<Windows.h>
#include"Element.h"
#include<vector>
typedef __int64 DDWORD;
using namespace std;
class Node {
public:
	static int number;//节点总数
	int NO;//节点编号
	bool flag;//有效性指标,false表示该结点不可用,true表示可用
	//只要结点的坐标确定，结点就变为可用
	vector<double> loca;//存放节点坐标,初始化为[0,0]
	vector<DDWORD> elements;//单元编号指针，指向单元的编号数组
	vector<bool> link_way;//连接方式指针，刚接是false，铰接是true
	vector<double> load;//存放节点荷载，节点平动x，平动y和转动
	vector<int> freedom;//节点自由度编号，一般有三个元素，半铰接点有特殊处理函数

	Node();//空构造函数，允许批量节点数组存在
	Node(double,double);//必须通过设置节点坐标来初始化节点
	void set_loca(double, double);//设置节点坐标
	bool is_splice();//判断是否是全铰节点
	bool is_rigid();//判断是否是全刚节点
	void set_load(double load1 = 0, double loda2 = 0, double load3 = 0);//给结点添加荷载
	void print();
	~Node();
};
#endif