//
// Created by 67460 on 2020/5/10.
//
#include <map>
#include "calculator.h"
#include <regex>


Parse::Parse() {
    //把π添加到map里
    m.insert(std::pair<std::string, double>("pi", 3.1415926));
    //二元运算
    regex1 = std::regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+))");
    //三元加减运算
    regex2 = std::regex(
            R"((\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+))");
    //三元混合运算（乘除在前）
    regex3 = std::regex(
            R"((\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+))");
    //三元混合运算（乘除在后）
    regex4 = std::regex(
            R"((\d+|\d+\.\d+|[A-Za-z]+)([+\-])(\d+|\d+\.\d+|[A-Za-z]+)([+/*-])(\d+|\d+\.\d+|[A-Za-z]+))");
    //三元乘除运算
    regex5 = std::regex(R"((\d+|\d+\.\d+|[A-Za-z]+)([*/])(\d+|\d+\.\d+|[A-Za-z]+)([*/])(\d+|\d+\.\d+|[A-Za-z]+))");
    //给某个字母赋值，比如 A=3*4+2
    regex6 = std::regex("([A-Za-z]+)=(.+)");
    //只有一个数字或者只有一个字母
    regex7 = std::regex(R"(\d+|\d+\.\d+|[A-Za-z]+)");
}

double Parse::getAns(std::string str) {
    std::smatch result;
    if (endsWith(str, ";")) {
        str = str.substr(0, str.length() - 1);
    }

    //二元运算
    if (regex_match(str, result, regex1)) {
        double num1 = m.count(result[1]) == 1 ? m.at(result[1]) : stringToDouble(result[1]);
        double num2 = m.count(result[3]) == 1 ? m.at(result[3]) : stringToDouble(result[3]);
        std::string myOperator = result[2];
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
        m.insert(std::pair<std::string, double>(result[1], ans));
        return ans;
    }


    //只有一个数字或者只有一个字母
    if (regex_match(str, result, regex7)) {
        return m.count(result[0]) ? m.at(result[0]) : stringToDouble(result[0]);
    }

    //throw new UnsupportedOperationException("unknown sentence");
    throw "unknown sentence";
}

double Parse::stringToDouble(std::string str) {
    std::istringstream iss(str);
    double x;
    iss >> x;
    return x;
}

bool Parse::endsWith(std::string s, std::string sub) {
    return s.rfind(sub) == (s.length() - sub.length()) ? 1 : 0;
}
