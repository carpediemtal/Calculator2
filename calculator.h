//
// Created by 67460 on 2020/5/10.
//
#include <regex>

#ifndef CALCULATOR_H
#define CALCULATOR_H

class Parse {
private:
    std::map<std::string, double> m;
    std::regex regex1;
    std::regex regex2;
    std::regex regex3;
    std::regex regex4;
    std::regex regex5;
    std::regex regex6;
    std::regex regex7;
public:
    Parse();

    double getAns(std::string str);

    double stringToDouble(std::string str);

    bool endsWith(std::string s, std::string sub);
};

#endif //CALCULATOR_H
