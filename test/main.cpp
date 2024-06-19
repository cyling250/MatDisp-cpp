#include<iostream>
#include"Structure.h"
#include<vector>

using namespace std;

int main() {
//    Structure s;
//    Node n1(0, 0);
//    Node n2(2, 0);
//    Node n3(3, 0);
//    Node n4(5, 0);
//    s.link(&n1, &n2);
//    s.link(&n2, &n3, false, true);
//    s.link(&n3, &n4, true);
//    for (int i = 0; i < 3; i++) {
//        elements[i].print();
//    }
//    elements[0].add_restrain(vector < int > {-1, -1, -1, -1, -1, 0});
//    elements[1].add_restrain(vector < int > {-1, -1, 0, -1, -1, 0});
//    elements[2].add_restrain(vector < int > {-1, -1, 0, -1, -1, -1});
//    elements[1].set_load(vector < double > {0, 2, 1});
//    elements[0].set_load(vector < double > {2, 1});
//    s.get_freedom();
//    s.get_K();
//    s.get_load();
//    s.add_restrain();
//    print(s.K);
//    print(s.load);
    Element e1({1, 2}, {2, 4});
    cout << e1 << endl;
    return 0;
}