#include<iostream>
#include"Structure.h"
#include"Mat.h"
#include<cmath>

using namespace std;

int main() {
    Structure s;
    Node n1(0, 0), n2(12, 0), n3(0, 6), n4(12, 6);
    Element* e1 = s.link(&n1, &n2, false, false, 6.94 * pow(10, -3) * 12 * 12, 1.0 / 12, 0.63);
    Element* e2 = s.link(&n1, &n3, false, false, 6.94 * pow(10, -3) * 24 * 6, 1.0 / 24, 0.5);
    Element* e3 = s.link(&n2, &n4, false, false, 6.94 * pow(10, -3) * 24 * 6, 1.0 / 24, 0.5);
    e2->addRestrain({false, false, false, true, true, true});
    e3->addRestrain({false, false, false, true, true, true});
    e2->addLoad({2, 1});

    s.getFreedom();
    s.loadProcess();
    s.getEntireK();
    s.freedomProcess();
    cout << "K:\n" << s.K << endl;
    cout << "clacK:\n" << s.calcK << endl;
    cout << "load:\n" << s.load << endl;
    cout << "calcLoad:\n" << s.calcLoad << endl;
    return 0;
}