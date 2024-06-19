#include"Structure.h"
#include"Mat.h"

bool isSplice(Node* node, const std::set<Element*>& elements) {
    for (const auto& element : elements) {
        if (element->nodes[0] != node & element->nodes[1] != node)
            throw std::runtime_error("element not includes node");
        int idx = element->nodes[0] == node ? 0 : 1;
        if (element->linkWay[idx]) return false;
    }
    return true;
}

Structure::Structure() {
    this->elements = std::set<Element>();
    this->sizeOfK = 0;
    // 其余的数组可以不初始化
}

Element* Structure::link(
        Node* node1,
        Node* node2,
        bool linkWay1,
        bool linkWay2,
        double E,
        double I,
        double A) {
    return (Element*) &*(elements.insert(Element(node1, node2, linkWay1, linkWay2, E, I, A)).first);
}

std::map<Node*, std::set<Element*>> Structure::getLinkDict() {
    std::map<Node*, std::set<Element*>> linkDict;
    for (Element element : this->elements) {
        // 如果没有找到
        if (linkDict.find(element.nodes[0]) == linkDict.end()) {
            linkDict[element.nodes[0]] = std::set<Element*>();
        }
        if (linkDict.find(element.nodes[1]) == linkDict.end()) {
            linkDict[element.nodes[1]] = std::set<Element*>();
        }
        linkDict[element.nodes[0]].insert(&element);
        linkDict[element.nodes[1]].insert(&element);
    }
    return linkDict;
}

int Structure::getFreedom() {
    int freedom = 0;
    std::map<Node*, std::set<Element*>> linkDict = getLinkDict();
    for (const auto& entity : linkDict) {
        Node* node = entity.first;
        std::set<Element*> tempElements = entity.second;
        int linkTemp = 0 - isSplice(node, tempElements);
        for (auto element : tempElements) {
            int i = element->nodes[1] == node;
            if (element->linkWay[i]) {
                linkTemp++;
                element->freedom[i * 3 + 0] = freedom;
                element->freedom[i * 3 + 1] = freedom + 1;
                element->freedom[i * 3 + 2] = freedom + 2 + linkTemp;
            } else {
                element->freedom[i * 3 + 0] = freedom;
                element->freedom[i * 3 + 1] = freedom + 1;
                element->freedom[i * 3 + 2] = freedom + 2;
            }
        }
        if (linkTemp > 0) node->freedom = concatenate(node->freedom, std::vector<int>(linkTemp));
        for (int k = freedom; k < freedom + 3 + linkTemp; k++) node->freedom[k - freedom] = k;
        freedom = freedom + 3 + linkTemp;
    }
    this->sizeOfK = freedom;
    return freedom;
}

const std::vector<std::vector<double>>* Structure::getEntireK(bool forceCalc) {
    if (!this->K.empty() && !forceCalc) {
        return &this->K;
    }
    this->K = std::vector<std::vector<double>>(this->sizeOfK, std::vector<double>(this->sizeOfK));
    for (auto element : this->elements) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                this->K[element.freedom[i]][element.freedom[j]] += element.globalKe[i][j]; // 对号入座
            }
        }
    }
    return &this->K;
}

void Structure::loadProcess() {
    this->load = std::vector<double>(this->sizeOfK);
    // 先处理节点荷载
    for (const auto& entity : getLinkDict()) {
        for (int i = 0; i < 3; i++) {
            Node* node = entity.first;
            this->load[node->freedom[i]] += node->load[i]; // 节点不需要反号
        }
    }
    // 再处理等效荷载
    for (auto element : this->elements) {
        element.load = transpose(element.T) * element.load;
        for (int i = 0; i < 6; i++) {
            this->load[element.freedom[i]] += element.load[i];
        }
    }
}

void Structure::freedomProcess() {
    std::vector<int> deleteTemp;
    this->inResult = std::vector<bool>(this->sizeOfK); // 初始化位移定位矩阵
    for (auto element : this->elements) {
        for (int i = 0; i < 6; i++) {
            if (element.restrain[i]) {
                // 该自由度被约束住了
                this->inResult[element.freedom[i]] = false;
                deleteTemp.push_back(element.freedom[i]);
            }
        }
        if (element.linkWay[0] && element.linkWay[1]) {
            // 说明这是一个轴力杆,对该轴力杆的弯矩进行自行约束
            this->inResult[element.freedom[2]] = false;
            this->inResult[element.freedom[5]] = false;
            deleteTemp.push_back(element.freedom[2]);
            deleteTemp.push_back(element.freedom[5]);
        }
    }
    if (!this->K.empty()) {
        // 约束划行
        this->calcK = deleteObj(this->K, deleteTemp);
        // 约束化列
        for (auto& i : this->calcK) {
            i = deleteObj(i, deleteTemp);
        }
    }
    if (!this->load.empty()) {
        this->calcLoad = deleteObj(this->load, deleteTemp);
    }
}

void Structure::resolve() {
    // TODO R G 求解 并得到杆端位移与内力
}


Structure::~Structure() = default;


