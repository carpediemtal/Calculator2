#include <iostream>
#include <regex>
#include <map>
#include <fstream>

using namespace std;


class Parse {
private:
    map<string, double> m;
    regex regex1;
    regex regex2;
    regex regex3;
    regex regex4;
    regex regex5;
    regex regex6;
    regex regex7;
public:
    Parse() {
        //把π添加到map里
        m.insert(pair<string, double>("pi", 3.1415926));
        //二元运算
        regex1 = regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+))");
        //三元加减运算
        regex2 = regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+))");
        //三元混合运算（乘除在前）
        regex3 = regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+))");
        //三元混合运算（乘除在后）
        regex4 = regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+))");
        //三元乘除运算
        regex5 = regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([*/])(\d+|\d+\.\d+|[A-Za-z]+)([*/])(\d+|\d+\.\d+|[A-Za-z]+))");
        //给某个字母赋值，比如 A=3*4+2
        regex6 = regex("([A-Za-z]+)=(.+)");
        //只有一个数字或者只有一个字母
        regex7 = regex(R"(\d+|\d+\.\d+|[A-Za-z]+)");
    }

    double getAns(string str) {
        smatch result;
        if (endsWith(str, ";")) {
            str = str.substr(0, str.length() - 1);
        }

        //二元运算
        if (regex_match(str, result, regex1)) {
            double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
            double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
            string myOperator = result[2];
            if (myOperator == "+") {
                return num1 + num2;
            }
            if (myOperator == "-") {
                return num1 - num2;
            }
            if (myOperator == "*") {
                return num1 * num2;
            }
            if (myOperator == "/") {
                return num1 / num2;
            }
        }
        //三元加减运算
        if (regex_match(str, result, regex2)) {
            double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
            double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
            double num3 = m.count(result[5]) == 1 ? m.at(result[5]) : stringToDouble(result[5]);
            double ans;
            if (result[2] == "+") {
                ans = num1 + num2;
            } else {
                ans = num1 - num2;
            }
            if (result[4] == "+") {
                ans += num3;
            } else {
                ans -= num3;
            }
            return ans;
        }

        //三元混合运算（乘除在前）
        if (regex_match(str, result, regex3)) {
            double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
            double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
            double num3 = m.count(result[5]) == 1 ? m.at(result[5]) : stringToDouble(result[5]);
            double ans;
            if (result[2] == "*") {
                ans = num1 * num2;
            } else {
                ans = num1 / num2;
            }
            if (result[4] == "+") {
                ans += num3;
            } else {
                ans -= num3;
            }
            return ans;
        }

        //三元混合运算（乘除在后）
        if (regex_match(str, result, regex4)) {
            double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
            double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
            double num3 = m.count(result[5]) == 1 ? m.at(result[5]) : stringToDouble(result[5]);
            double ans;
            if (result[4] == "*") {
                ans = num2 * num3;
            } else {
                ans = num2 / num3;
            }
            if (result[2] == "+") {
                ans += num1;
            } else {
                ans -= num1;
            }
            return ans;
        }

        //三元乘除运算
        if (regex_match(str, result, regex5)) {
            double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
            double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
            double num3 = m.count(result[5]) == 1 ? m.at(result[5]) : stringToDouble(result[5]);
            double ans;
            if (result[2] == "*") {
                ans = num1 * num2;
            } else {
                ans = num1 / num2;
            }
            if (result[4] == "*") {
                ans *= num3;
            } else {
                ans /= num3;
            }
            return ans;
        }

        //给某个字母赋值，比如 A=3*4+2
        if (regex_match(str, result, regex6)) {
            double ans = getAns(result[2]);
            m.insert(pair<string, double>(result[1], ans));
            return ans;
        }


        //只有一个数字或者只有一个字母
        if (regex_match(str, result, regex7)) {
            return m.count(result[0]) ? m.at(result[0]) : stringToDouble(result[0]);
        }

        //throw new UnsupportedOperationException("unknown sentence");
        throw "unknown sentence";
    }

    double stringToDouble(string str) {
        istringstream iss(str);
        double x;
        iss >> x;
        return x;
    }

    bool endsWith(string s, string sub) {
        return s.rfind(sub) == (s.length() - sub.length()) ? 1 : 0;
    }
};


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
