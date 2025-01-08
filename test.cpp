#include "TList.hpp"
#include <iostream>

int main() {
    TList<int, 20> a{10};
    a.back();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000 - 1);
    for (int i = 0; i < 100; i++) {
        int j = dis(gen);
        a.append(j);
        if (a.size() == 10) {
            printf("--------------------------------------\n");
            std::string a_content = "[";
            for (auto v : a) {
                a_content += std::to_string(v) + " ";
            }
            a_content += "]";
            printf("a = %s\n", a_content.data());
            int pop_index = j % 10;
            auto d = a.pop_back(pop_index).value_or(999);
            a_content = "[";
            for (auto v : a) {
                a_content += std::to_string(v) + " ";
            }
            a_content += "]";
            printf("pop_back(%d) = %d\n", pop_index, d);
            printf("a = %s\n", a_content.data());
        }
    }
}
