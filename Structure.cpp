#include "Structure.h"
#include"Matrix.h"
#include"common.h"
Structure::Structure()
{
	this->size_of_K = 0;
	//其余的参数不进行初始化，在nodes为空时，不访问其余的参数
}

void Structure::link(Node& node1,Node& node2,Element& element,bool link_way1,bool link_way2,double E,double I,double A) 
{
	//Element参数是必须的，要让Structure生成杆件，就需要有个引用参数来接受生成的参数
	if (!(node1.flag && node2.flag)) { printf("Node is not initialized."); return; }
	//向结构注册结点
	if (!index_node(&node1)) {
		this->nodes.push_back((DDWORD)&node1); 
		Node::number++;//结点总数自增
		node1.NO = Node::number;//赋予结点1编号
	}
	if (!index_node(&node2)) {
		this->nodes.push_back((DDWORD)&node2);
		Node::number++;//结点总数自增
		node2.NO = Node::number;//赋予结点2编号
	}
	//向结构注册杆件
	this->elements.push_back((DDWORD)&element);
	Element::number++;
	element.NO = Element::number;//赋予杆件编号

	//向杆件注册结点
	element.nodes.push_back((DDWORD)&node1);
	element.nodes.push_back((DDWORD)&node2);
	element.link_way[0] = link_way1;
	element.link_way[1] = link_way2;
	//向结点注册杆件
	node1.elements.push_back((DDWORD)&element);
	node2.elements.push_back((DDWORD)&element);
	node1.link_way.push_back(link_way1);
	node2.link_way.push_back(link_way2);
	//杆件生成完毕

	//计算杆件基本参数
	double length = pow(pow(node1.loca[0] - node2.loca[0], 2) + pow(node1.loca[1] - node2.loca[1], 2), 0.5);
	element.L = length;
	element.E = E; element.I = I; element.A = A;

	//生成局部坐标单元刚度矩阵
	vector<vector<double>> temp = {
		{E * A / length, 0, 0, -E * A / length, 0, 0},
		{0, 12 * E * I / pow(length, 3), 6 * E * I / pow(length, 2), 0, -12 * E * I / pow(length, 3), 6 * E * I / pow(length, 2)},
		{ 0, 6 * E * I / pow(length, 2), 4 * E * I / length, 0, -6 * E * I / pow(length, 2), 2 * E * I / length},
		{ -E * A / length, 0, 0, E * A / length, 0, 0},
		{0, -12 * E * I / pow(length, 3), -6 * E * I / pow(length, 2), 0, 12 * E * I / pow(length, 3), -6 * E * I / pow(length, 2)},
		{0, 6 * E * I / pow(length, 2), 2 * E * I / length, 0, -6 * E * I / pow(length, 2), 4 * E * I / length} 
	};
	if (link_way1 && link_way2) {
		//说明是轴力杆，直接约束
		temp = {
		{E * A / length, 0, 0, -E * A / length, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{ -E * A / length, 0, 0, E * A / length, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}
		};
	}
	//生成定位向量
	double cosa = (node2.loca[0] - node1.loca[0]) / length;
	double sina = (node2.loca[1] - node1.loca[1]) / length;
	element.T = {
		{cosa, sina, 0, 0, 0, 0},
		{-sina, cosa, 0, 0, 0, 0},
		{0, 0, 1, 0, 0, 0},
		{0, 0, 0, cosa, sina, 0},
		{0, 0, 0, -sina, cosa, 0},
		{0, 0, 0, 0, 0, 1}
	};
	vector<vector<double>> Tt = transpose(element.T);
	element.k_e = Tt * temp * element.T;//计算单元刚度矩阵
	//此时，element中只有restrain, load, delta, force没有被幅值
}

int Structure::index_node(Node* node)
{
	//索引已经保存的结点，如果找到，直接给出可索引的序号，否则返回-1
	for (int i = 0; i < this->nodes.size(); i++) {
		if ((DDWORD)node == this->nodes[i])return i;
	}
	return false;
}

int Structure::get_freedom()
{
	int freedom_num = 0;//初始化自由度编号
	for (int i = 0; i < this->nodes.size(); i++) {

		int link_temp = 0;
		Node* temp_node = (Node*)this->nodes[i];

		if (temp_node->is_splice()) {
			//如果i是全铰接点，遍历该结点的所有杆件
			for (int j = 0; j < temp_node->elements.size(); j++) {
				Element* temp_element = (Element*)temp_node->elements[j];
				if (temp_element->nodes[0] == (DDWORD)temp_node) {
					//如果是第一个结点
					temp_element->freedom[0] = freedom_num;
					temp_element->freedom[1] = freedom_num + 1;
					temp_element->freedom[2] = freedom_num + 2 + link_temp;
				}
				else {
					//如果是第二个结点
					temp_element->freedom[3] = freedom_num;
					temp_element->freedom[4] = freedom_num + 1;
					temp_element->freedom[5] = freedom_num + 2 + link_temp;
				}
				link_temp++;
			}
		}
		else {
			//如果i不是全铰接点
			bool add = true;
			for (int j = 0; j < temp_node->elements.size(); j++) {
				Element* temp_element = (Element*)temp_node->elements[j];
				if (temp_element->nodes[0] == (DDWORD)temp_node) {
					//如果这是该单元的第一个结点
					if (temp_node->link_way[j]) {
						//如果该结点与该单元链接方式为铰接
						temp_element->freedom[0] = freedom_num;
						temp_element->freedom[1] = freedom_num + 1;
						temp_element->freedom[2] = freedom_num + 2 + link_temp;
						link_temp++;
					}
					else {
						//如果该节点与该单元链接方式为刚接
						temp_element->freedom[0] = freedom_num;
						temp_element->freedom[1] = freedom_num + 1;
						temp_element->freedom[2] = freedom_num + 2;
						if (add) {
							//如果是第一个结点,刚节点也需要给linktemp+1
							link_temp++;
							add = false;
						}
					}
				}
				else {
					//这是该单元的第二个结点
					if (temp_node->link_way[j]) {
						//如果该结点与该单元链接方式为铰接
						temp_element->freedom[3] = freedom_num;
						temp_element->freedom[4] = freedom_num + 1;
						temp_element->freedom[5] = freedom_num + 2 + link_temp;
						link_temp++;
					}
					else {
						//如果该节点与该单元链接方式为刚接
						temp_element->freedom[3] = freedom_num;
						temp_element->freedom[4] = freedom_num + 1;
						temp_element->freedom[5] = freedom_num + 2;
						if (add) {
							//如果是第一个结点,刚节点也需要给linktemp+1
							link_temp++;
							add = false;
						}
					}
				}
			}
		}

		for (int k = freedom_num; k < freedom_num + 3 + link_temp; k++) {
			temp_node->freedom.push_back(k);
		}
		freedom_num += 2 + link_temp;
	}
	this->size_of_K = freedom_num;
	return freedom_num;
}

void Structure::get_K()
{
	//初始化
	this->K = vector<vector<double>>(this->size_of_K, vector<double>(this->size_of_K));
	for (int k = 0; k < this->elements.size(); k++) {
		vector<int> Lambda = ((Element*)this->elements[k])->freedom;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				this->K[Lambda[i]][Lambda[j]] += ((Element*)this->elements[k])->k_e[i][j];
			}
		}
	}
}

void Structure::get_load()
{
	this->load = vector<double>(this->size_of_K);
	//先处理结点荷载
	for (int i = 0; i < this->nodes.size(); i++) {
		Node* temp_node = (Node*)this->nodes[i];
		vector<int> Lambda = temp_node->freedom;
		for (int j = 0; j < 3; j++) {
			this->load[Lambda[j]] += temp_node->load[j];//结点不需要反号
		}
	}
	//再处理等效荷载
	for (int i = 0; i < this->elements.size(); i++) {
		Element* temp_element = (Element*)this->elements[i];
		vector<int>Lambda = temp_element->freedom;
		vector<vector<double>> T = temp_element->T;
		temp_element->load = -(transpose(T) * temp_element->load);
		for (int j = 0; j < 6; j++) {
			this->load[Lambda[j]] += temp_element->load[j];
		}
	}
}

void Structure::add_restrain()
{
	//添加约束仅仅只能对杆件添加约束，这里的约束直接进行划行划列
	vector<int>delete_temp;//需要删除的自由度
	this->in_result = vector<int>(this->size_of_K);
	for (int i = 0; i < this->elements.size(); i++) {
		Element* temp_element = (Element*)this->elements[i];
		for (int j = 0; j < 6; j++) {
			if (temp_element->restrain[j] == -1) {
				//说明该自由度被约束住了
				this->in_result[temp_element->freedom[j]] = -1;
				if (!is_exist(delete_temp, temp_element->freedom[j]))
					delete_temp.push_back(temp_element->freedom[j]);
			}
		}
	}

	int real_size = this->size_of_K - delete_temp.size();
	vector<vector<double>> temp_k = vector<vector<double>>(real_size, vector<double>(real_size));
	vector<double> temp_load = vector<double>(real_size);
	int count_i = 0, count_j = 0;
	for (int i = 0; i < this->size_of_K; i++) {
		if (is_exist(delete_temp, i))continue;
		for (int j = 0; j < this->size_of_K; j++) {
			if (is_exist(delete_temp, j))continue;
			temp_k[count_i][count_j] = this->K[i][j];//只有当这行这列在delete_temp中均未出现时，才能写入
			count_j++;
		}
		temp_load[count_i] = this->load[i];
		count_i++;
		count_j = 0;
	}
	this->load = temp_load;
	this->K = temp_k;
}

Structure::~Structure()
{
}
