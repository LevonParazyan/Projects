#include "Dragon.h"
#include <fstream>
#include <iostream>
#include <string>

bool Dragon::read_file()
{
    file_in.open("Dragon.txt");
    while (!file_in.eof()) {
        std::string str;
        std::getline(file_in, str);
        check(str);
    }
    if (condition_flag) {
        std::cout << "Compile error: ";
        exit(1);
    }
    return file_in.is_open();
}

int Dragon::read_lines(std::string str)
{
    if (str == "") { return 0; }
        std::string line = str;
        std::string word;
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] != ' ')
            {
                word.push_back(line[i]);
            }
            else {
                if (word != "") {
                    words.push_back(word);
                }
                word.clear();
            }
        }
       
}

bool Dragon::check(std::string str)
{
    if (str == "")
    {
        return 1;
    }
    str.push_back(' ');
    words.clear();
    read_lines(str);
    if (condition_flag || loop_flag) {
        if (check_words(words[0], Instructions)) {
            std::cout << "Compile error: ";
            exit(1);

        }
        if (words[0] == "}") {
            condition_flag = 0;
            return 1;
        }
        else if (condition_flag == -1) return 1;
    }
    if (check_words(words[0], Type_name)) {
        convert_Type_name(words);
    }
    else if (words[0] == "print") {
        print(words);
    }
    else if (check_words(words[0], Variables)) {
        Arithmetic_unit(words);
    }
    else if (words[0] == "cond") {
        condition_flag = condition(words);
    }
    else if (words[0] == "loop") {
        loop(words);
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }
}

bool Dragon::Arithmetic_unit(std::vector<std::string> rv)
{
    if (rv.size() == 5 || rv.size() == 3) {
        if (rv[1] != "=") {
            std::cout << "Compile error: ";
            exit(1);
        }
        if (check_isdigit(rv[2])) {
            tmp1 = stof(rv[2]);
            int index = check_variable(rv[0]);
            if (var[index]->type_name == "nat") {
                *(int*)var[index]->value = tmp1;
            }
            else {
                *(float*)var[index]->value = tmp1;
            }
        }
        else if (check_words(rv[2], Variables)) {
            int index = check_variable(rv[2]);
            if (var[index]->type_name == "nat") {
                tmp1 = *(int*)var[index]->value;
            }
            else {
                tmp1 = *(float*)var[index]->value;
            }
        }
        else {
            std::cout << "Compile error: ";
            exit(1);
        }
        if (rv.size() == 5) {
            if (check_isdigit(rv[4])) {
                tmp2 = stof(rv[4]);
            }
            else if (check_words(rv[4], Variables)) {
                int index = check_variable(rv[4]);
                if (var[index]->type_name == "nat") {
                    tmp2 = *(int*)var[index]->value;
                }
                else {
                    tmp2 = *(float*)var[index]->value;
                }
            }
            else {
                std::cout << "Compile error: ";
                exit(1);
            }
            int index = check_variable(rv[0]);
            if (rv[3] == "+") {
                result = tmp1 + tmp2;
            }
            else if (rv[3] == "-") {
                result = tmp1 - tmp2;
            }
            else if (rv[3] == "*") {
                result = tmp1 * tmp2;
            }
            else if (rv[3] == "/") {
                if (tmp2 == 0) {
                    std::cout << "ZERO DIVISION ERROR: ";
                    exit(1);
                }
                result = tmp1 / tmp2;
            }
            else {
                std::cout << "Compile error: ";
                exit(1);
            }
            if (var[index]->type_name == "nat") {
                *(int*)var[index]->value = result;
            }
            else {
                *(float*)var[index]->value = result;
            }
        }
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }
}

bool Dragon::print(std::vector<std::string> rv)
{
    
    for (int i = 1; i < rv.size(); ++i) {
        int flag = 0;
        for (int j = 0; j < Variables.size(); ++j) {
            if (rv[i] == Variables[j]) {
                if (var[j]->type_name == "nat") {
                    std::cout << *(int*)var[j]->value;
                }
                else if(var[j]->type_name == "real"){
                    std::cout << *(float*)var[j]->value;
                }
                ++flag;
                break;
            }
        }
        if (flag == 0) {
            std::cout << rv[i];
        }
        std::cout << " ";
    }
    std::cout << '\n';
    return false;
}

bool Dragon::convert_Type_name(std::vector<std::string> rv)
{
    if (rv.size() != 2 && rv.size() != 4) { 
        std::cout << "Compile error: ";
        exit(1); 
    }
    if (!check_syntax_variables(rv[1])) {
        std::cout << "Compile error: ";
        exit(1);
    }
    else {
        Variables.push_back(rv[1]);
    }
    if (rv.size() == 4) {
        if (rv[2] != "=") {
            std::cout << "Compile error: ";
            exit(1);
        }
        if (!check_isdigit(rv[3])) {
            std::cout << "You must write digit ";
            exit(1);
        }
    }
    Variable* p = new Variable;
    p->type_name= rv[0];
    p->variable_name = rv[1];
    if (p->type_name == "nat") {
        p->value = new int;
        if (rv.size() == 4) {
            *(int*)p->value = std::stoi(rv[3]);
        }
        else
        {
            *(int*)p->value = 0;
        }
    }
    else if (p->type_name == "real") {
        p->value = new float;
        if (rv.size() == 4) {
            *(float*)p->value = std::stof(rv[3]);
        }
        else 
        {
            *(int*)p->value = 0;
        }
    }
    var.push_back(p);
    return true;
}

int Dragon::condition(std::vector<std::string> rv)
{
    float tmp1 = 0;
    float tmp2 = 0;
    if (rv.size() != 5) {
        std::cout << "Compile error: ";
        return false;
    }
    if (check_isdigit(rv[1])) {
        tmp1 = stof(rv[1]);
    }
    else if (check_words(rv[1], Variables)) {
        int index = check_variable(rv[1]);
        if (var[index]->type_name == "nat") {
            tmp1 = *(int*)var[index]->value;
        }
        else {
            tmp1 = *(float*)var[index]->value;
        }
    }
    else {
        std::cout << "Compile error: ";
        return false;
    }
    if (check_isdigit(rv[3])) {
        tmp2 = stof(rv[3]);
    }
    else if (check_words(rv[3], Variables)) {
        int index = check_variable(rv[3]);
        if (var[index]->type_name == "nat") {
            tmp2 = *(int*)var[index]->value;
        }
        else {
            tmp2 = *(float*)var[index]->value;
        }
    }
    else {
        std::cout << "Compile error: ";
        return false;
    }
    if (rv[4] != "{") {
        std::cout << "Compile error: ";
        return false;
    }
    if (rv[2] == ">") {
        if (tmp1 > tmp2) return 1;
        else return -1;
    }
    else if (rv[2] == "<") {
        if (tmp1 < tmp2) return 1;
        else return -1;
    }
    else if (rv[2] == "==") {
        if (tmp1 == tmp2) return 1;
        else return -1;
    }
    else if (rv[2] == "!=") {
        if (tmp1 != tmp2) return 1;
        else return -1;
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }

}

void Dragon::loop(std::vector<std::string> rv)
{
   
    float tmp1 = 0;
    float tmp2 = 0;
    if (rv.size() != 5) {
        std::cout << "Compile error: ";
        exit(1);
    }
    if (check_words(rv[1], Variables)) {
        int index = check_variable(rv[1]);
            tmp1 = index;
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }
    if (check_words(rv[3], Variables)) {
        int index = check_variable(rv[3]);
            tmp2 = index;
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }
    if (rv[4] != "{") {
        std::cout << "Compile error: ";
        exit(1);
    }
    while (!file_in.eof()) {
        std::string str;
        std::getline(file_in, str);
        loop_flag = 1;
        if (str == "}") {
            ++loop_flag;
            break;
        }
        else {
            stack.push_back(str);
        }
    }
    if (loop_flag == 1) {
        std::cout << "Compile error: ";
        exit(1);
    }
    if (rv[2] == ">") {
        loop_big(tmp1, tmp2);
    }
    else if (rv[2] == "<") {
        loop_small(tmp1, tmp2);
    }
    else if (rv[2] == "==") {
        loop_equal(tmp1, tmp2);
    }
    else if (rv[2] == "!=") {
        loop_not_equal(tmp1, tmp2);
    }
    else {
        std::cout << "Compile error: ";
        exit(1);
    }
    loop_flag = 0;
    exit(1);
}

void Dragon::loop_big(float tmp1, float tmp2) {
    if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "nat")
        while (*(int*)var[tmp1]->value > *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "real")
        while (*(int*)var[tmp1]->value > *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "nat")
        while (*(float*)var[tmp1]->value > *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "real")
        while (*(float*)var[tmp1]->value > *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
}

void Dragon::loop_small(float tmp1, float tmp2) {
    if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "nat")
        while (*(int*)var[tmp1]->value < *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "real")
        while (*(int*)var[tmp1]->value < *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "nat")
        while (*(float*)var[tmp1]->value < *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "real")
        while (*(float*)var[tmp1]->value < *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
}

void Dragon::loop_equal(float tmp1, float tmp2) {
    if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "nat")
        while (*(int*)var[tmp1]->value == *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "real")
        while (*(int*)var[tmp1]->value == *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "nat")
        while (*(float*)var[tmp1]->value == *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "real")
        while (*(float*)var[tmp1]->value == *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
}

void Dragon::loop_not_equal(float tmp1, float tmp2) {
    if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "nat")
        while (*(int*)var[tmp1]->value != *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "nat" && var[tmp2]->type_name == "real")
        while (*(int*)var[tmp1]->value != *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "nat")
        while (*(float*)var[tmp1]->value != *(int*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
    else if (var[tmp1]->type_name == "real" && var[tmp2]->type_name == "real")
        while (*(float*)var[tmp1]->value != *(float*)var[tmp2]->value)
        {
            for (int i = 0; i < stack.size(); ++i) {
                check(stack[i]);
            }
        }
}

bool Dragon::check_syntax_variables(std::string str)
{
    if ((str[0] >= 65 && str[0] <= 90) || (str[0] >= 97 && str[0] <= 122) || (str[0] == '_')) {}
    else return false;
    for (int i = 1; i < str.size(); ++i) {
        if ((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || (str[i] >= 48 && str[i] <= 57) || (str[i] == '_')) {}
        else {
            return false;
        }
    }
    return true;
}

int Dragon::check_variable(std::string str)
{
    for (int j = 0; j < Variables.size(); ++j) {
        if (str == Variables[j]) {
            return j;
        }
    }
    return -1;
}

bool Dragon::check_isdigit(std::string str) {
    
    int count = 0;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '.') {
            ++count;
            if (count == 1) {
                continue;
            }
            else return false;
        }
        if ((str[i] >= 48 && str[i] <= 57)) {}
        else {
            return false;
        }
    }
    return true;
}

bool Dragon::check_words(std::string str, std::vector<std::string> rhs)
{
    for (int i = 0; i < rhs.size(); ++i) {
        if (str == rhs[i]) { return true; }
    }
    return false;
    
}

Dragon::~Dragon()
{
    for (int i = 0; i < var.size(); ++i) {
        delete var[i];
    }
}
