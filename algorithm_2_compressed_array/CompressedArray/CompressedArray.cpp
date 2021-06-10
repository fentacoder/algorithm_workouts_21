#include <iostream>
#include "CompressedArray.h"

int main()
{
    const int arrSize = 10;

    int intArr[arrSize] = {0,1,2,2,2,3,3,4,3,4};
    char charArr[arrSize] = {'b','a','b','b','b','b','c','a','f','t'};

    nano::CompressedArray compressedArr = nano::CompressedArray();

    std::cout << "\ncompressing integer array...\n";
    std::cout << "\n original array size: " << arrSize << "\n";
    compressedArr.compress<int>(intArr, arrSize);
    std::cout << "\n new array size: " << compressedArr.value.size() << "\n";

    std::cout << "\ncompressing character array...\n";
    std::cout << "\n original array size: " << arrSize << "\n";
    compressedArr.compress<char>(charArr, arrSize);
    std::cout << "\n new array size: " << compressedArr.value.size() << "\n";
}

