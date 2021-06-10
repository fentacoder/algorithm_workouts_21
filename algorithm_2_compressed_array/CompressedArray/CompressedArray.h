#pragma once
#include <string>
#include <vector>

namespace nano {
	class CompressedArray {
	public:
		//this is for which primitive type is being passed in
		char type;
		std::vector<std::string> value;
		CompressedArray() {
		}
		~CompressedArray() {
			value.clear();
		}

		template <typename T> void compress(T origArray[],int arrSize) {
			std::cout << "\n this type is not compatible...";
		}

		template <> void compress<int>(int origArray[], int arrSize) {
			if (arrSize > 1) {
				value.clear();
				type = 'i';
				int i = 0;
				int repeatCounter = 1;
				int currentChar = origArray[0];
				bool lastElem = false, lastIndexFirstTime = true;
				while (i < arrSize + 1) {
					if (!lastIndexFirstTime) repeatCounter--;
					if (origArray[i] == currentChar && i > 0 && !lastElem) {
						if (i == arrSize - 1) {
							repeatCounter++;
							lastElem = true;
						}
						else {
							i++;
							repeatCounter++;
						}
					}
					else {
						if (repeatCounter > 2) {
							std::string compressionItem = std::to_string(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem = std::to_string(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
						else if (repeatCounter > 1) {
							std::string compressionItem = std::to_string(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem = std::to_string(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
						else if (i == 0) {
							i++;
							std::string compressionItem = std::to_string(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 0;
						}
						else {
							std::string compressionItem = std::to_string(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem = std::to_string(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
					}
				}

				for (auto& intElem : value) {
					std::cout << "\n" << intElem << "\n";
				}
			}
			else {
				std::cout << "nothing to compress...\n";
			}
		}

		template <> void compress<char>(char origArray[], int arrSize) {
			if (arrSize > 1) {
				value.clear();
				type = 'c';
				int i = 0;
				int repeatCounter = 1;
				char currentChar = origArray[0];
				bool lastElem = false, lastIndexFirstTime = true;
				while (i < arrSize + 1) {
					if (!lastIndexFirstTime) repeatCounter--;
					if (origArray[i] == currentChar && i > 0 && !lastElem) {
						if (i == arrSize - 1) {
							repeatCounter++;
							lastElem = true;
						}
						else {
							i++;
							repeatCounter++;
						}
					}
					else {
						if (repeatCounter > 2) {
							std::string compressionItem;
							compressionItem.push_back(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem;
								compressionItem.push_back(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
						else if (repeatCounter > 1) {
							std::string compressionItem;
							compressionItem.push_back(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem;
								compressionItem.push_back(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
						else if (i == 0) {
							i++;
							std::string compressionItem;
							compressionItem.push_back(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 0;
						}
						else {
							std::string compressionItem;
							compressionItem.push_back(currentChar);
							value.push_back(compressionItem);
							currentChar = origArray[i];
							repeatCounter = 1;
							if (i < arrSize - 1) i++;

							if (!lastIndexFirstTime) {
								currentChar = origArray[i];
								std::string compressionItem;
								compressionItem.push_back(currentChar);
								value.push_back(compressionItem);
								break;
							}
							if (i == arrSize - 1 && lastIndexFirstTime) lastIndexFirstTime = false;
						}
					}
				}

				for (auto& intElem : value) {
					std::cout << "\n" << intElem << "\n";
				}
			}
			else {
				std::cout << "nothing to compress...\n";
			}
		}

		template <typename T> T* decompress() {
			int elemCount = 0;
			int currentPos = 0;
			for (int i = 0; i < value.size(); i++) {
				if (value[i].find("_") != std::string::npos) {
					if (value[i].find("_") != value[i].length() - 1) {
						int amount = std::stoi(value[i].substr(value[i].find("_") + 1, value[i].length()));
						elemCount += amount;
					}
					else {
						elemCount += 2;
					}
				}
				else {
					elemCount++;
				}
			}

			T* arrToReturn = new int[elemCount];

			for (int j = 0; j < value.size(); j++) {
				if (value[j].find("_") != std::string::npos) {
					if (value[j].find("_") != value[j].length() - 1) {
						int amount = std::stoi(value[j].substr(value[j].find("_") + 1, value[j].length()));

						switch (type) {
						case 'i':
							currentPos = arrayExtender<int>(&arrToReturn,amount,currentPos,std::stoi(value[j].substr(0, value[j].find("_"))));
						case 'c':
							currentPos = arrayExtender<char>(&arrToReturn, amount, currentPos, value[j].substr(0, value[j].find("_"))[0]);
							break;
						default:
							break;
						}
					}
					else {
						switch (type) {
						case 'i':
							arrToReturn[j] = std::stoi(value[j].substr(0, value[j].find("_")));
							arrToReturn[j + 1] = std::stoi(value[j].substr(0, value[j].find("_")));
							currentPos += 2;
							break;
						case 'c':
							arrToReturn[j] = value[j].substr(0,value[j].find("_"))[0];
							arrToReturn[j] = value[j + 1].substr(0, value[j].find("_"))[0];
							currentPos += 2;
							break;
						default:
							break;
						}
					}
				}
				else {
					switch (type) {
					case 'i':
						arrToReturn[j] = std::stoi(value[j]);
						currentPos++;
						break;
					case 'c':
						arrToReturn[j] = value[j][0];
						currentPos++;
						break;
					default:
						break;
					}
				}
			}
			return &arrToReturn;
		}

		template <typename T> int arrayExtender(T* decompressedArr, int amount, int currentPos, T value) {
		}

		template <> int arrayExtender<int>(int* decompressedArr, int amount, int currentPos, int value) {
			int currentPosToReturn = currentPos;
			for (int i = 0; i < amount; i++) {
				decompressedArr[currentPos + i + 1] = value;
				currentPosToReturn++;
			}

			currentPosToReturn++;
			return currentPosToReturn;
		}

		template <> int arrayExtender<char>(char* decompressedArr, int amount, int currentPos, char value) {
			int currentPosToReturn = currentPos;
			for (int i = 0; i < amount; i++) {
				decompressedArr[currentPos + i + 1] = value;
				currentPosToReturn++;
			}

			currentPosToReturn++;
			return currentPosToReturn;
		}
	};
}