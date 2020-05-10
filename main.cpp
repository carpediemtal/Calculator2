#include <iostream>
#include <regex>
#include <map>
#include <fstream>
#include "calculator.h"
using namespace std;


int main(int argc, char *argv[]) {
    Parse p;
    //如果没有参数
    if (argc != 1) {
        for (int i = 1; i < argc; i++) {
            ifstream infile;
            infile.open(argv[i]);
            cout << argv[i] << endl;
            string str;
            while (getline(infile, str)) {
                double ans = p.getAns(str);
                cout << ans << endl;
            }
        }
    } else {//如果有传入的参数
        while (true) {
            string str;
            cin >> str;
            if (str == "exit") {
                break;
            }
            double ans = p.getAns(str);
            cout << ans << endl;
        }
    }
    return 0;
}
