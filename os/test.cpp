#include <iostream>

int main(){
std::string big = "qwertqwe";
std::string small = "qw";
std::string for_return = "";

int pos = big.find(small);
while (pos != -1){
                        int index = big.find(small);
                        std::cout << "Found at index: " << pos << std::endl;
                        for_return += ":" + std::to_string(index);
                        big = big.erase(index, small.length());
                        std::cout << "string: " << big << std::endl;
                        pos = big.find(small);
                    }
std::cout << for_return;
}