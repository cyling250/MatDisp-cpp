#include "Element.h"
#include"common.h"
#include"Matrix.h"
int Element::number = 0;//初始化静态数据成员
Element::Element()
{
	//为了方便构造单元数组，需要写的构造函数
	this->NO = 0;//初始化单元编号为0
	this->link_way = vector<bool>(2);
	this->freedom = vector<int>(6);//Each element has 6 freedoms
	this->E = 0; this->I = 0; this->L = 0; this->A = 0;
	this->k_e = vector<vector<double>>(6,vector<double>(6));//k_e is 6*6
	this->T = vector<vector<double>>(6, vector<double>(6));//T is 6*6
	this->restrain = vector<int>(6);
	this->load = vector<double>(6);
	this->delta = vector<double>(6);
	this->force = vector<double>(6);
}

void Element::add_restrain(vector<int> restrain)
{
	this->restrain = restrain;
}

void Element::set_load(vector<double> load)
{
	if (load[0] == 0) {
		//杆上集中荷载的形式[0,Fp,a],Fp是力的大小，a是集中力到杆件正方向左端的距离
		double b = this->L - load[2];
		double a = load[2];
		double fp = load[1];
		vector<double>arr = {
			0, -fp * pow(b,2) * (1 + 2 * a / L) / pow(L,2), -fp * a * pow(b,2) / pow(L,2),
			0, -fp * pow(a,2) * (1 + 2 * b / L) / pow(L,2), fp * a * pow(b,2) / pow(L,2)
		};
		this->load += arr;
	}
	else if (load[0]==1) {
		//1表示杆端弯矩，Load[1]存放杆左端弯矩,Load[2]存放杆右端弯矩
		this->load[2] -= load[1];
		this->load[5] -= load[2];
	}
	else if (load[0] == 2) {
		//2表示均布荷载，Load[1]存放均布荷载的大小
		double q = load[1];
		vector<double>arr = {
			0., -q * L / 2., -q * pow(L,2) / 12.,
			0., -q * L / 2., q* pow(L,2) / 12.
		};
		this->load += arr;
	}
	else if (load[0] == 3) {
		//3表示具有等斜度的均布荷载
		double q = load[1];
		double cosa = this->T[0][0];
		vector<double>arr = {
			0., -q * L / 2. * cosa, -q * pow(L,2) / 12. * pow(cosa,2),
			0., -q * L / 2. * cosa, q * pow(L,2) / 12. * pow(cosa,2)
		};
		this->load = this->load + this->T * arr;
	}
}

void Element::print()
{
	if (this->NO == 0) {
		//只有被纳入管理的杆件才能被打印
		printf("Can't print an uninitialized element.\n");
	}
	else {
		printf("Element:%d\nLink nodes:\n", this->NO);
		((Node*)this->nodes[0])->print();
		((Node*)this->nodes[1])->print();
		printf("link_way:%s,", this->link_way[0] ? "Hinge" : "Rigid");
		printf("%s\n", this->link_way[1] ? "Hinge" : "Rigid");
		printf("E=%.2f, I=%.2f, A=%.2f, L=%.2f\n\n", this->E, this->I, this->A,this->L);
	}
}

Element::~Element()
{

}