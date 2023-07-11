#include "Node.h"
#include"common.h"
int Node::number = 0;//初始化静态数据成员为0，没有节点
Node::Node()
{
	//仅仅为了支持数组形式的创建方法而写的函数
	this->NO = 0;//是否被Structure类管理的标志
	this->flag = false;//结点是否有效的标志
	this->loca = vector<double>(2);
	this->load = vector<double>(3);
}

Node::Node(double const node1, double const node2)
{
	//仅仅初始化结点的坐标，当结点没有被Structure类纳入管理，始终不分配结点编号
	this->loca.push_back(node1);
	this->loca.push_back(node2);
	this->load = vector<double>(3);
	this->flag = true;
}

void Node::set_loca(double const node1, double const node2)
{
	//仅仅初始化结点的坐标，当结点没有被Structure类纳入管理，始终不分配结点编号
	this->loca[0] = node1; this->loca[1] = node2;
	this->flag = true;
}

bool Node::is_splice()
{
	for (int i = 0; i < link_way.size(); i++) {
		//如果有一个刚接，则不是全铰接点，处理方式不需要特殊处理
		if (!this->link_way[i])return false;//非全铰接点
	}
	return true;//全铰接点
}

bool Node::is_rigid()
{
	for (int i = 0; i < link_way.size(); i++) {
		//如果有一个铰接，则不是全刚接点
		if (this->link_way[i])return false;//非全刚接点
	}
	return true;//全刚接点
}

void Node::set_load(double load1,double load2,double load3)
{
	//添加荷载
	this->load[0] = load1;
	this->load[1] = load2;
	this->load[2] = load3;
	if (this->is_rigid() && load3 != 0) {
		printf("Warning: can't add moment to anti-rigid node.\n");
		this->load[2] = 0;
	}
}

void Node::print()
{
	if (this->NO == 0) {
		//只有被纳入管理的结点才能被打印
		printf("Can't print an uninitialized node.\n");
	}
	else {
		printf("Node:%d(%.2f,%.2f)\n", this->NO, this->loca[0], this->loca[1]);
	}
}

Node::~Node()
{
}