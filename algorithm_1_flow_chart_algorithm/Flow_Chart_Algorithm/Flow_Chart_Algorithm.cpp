#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "FlowChartItem.h"

FlowChartItem initialItem = FlowChartItem();

std::string stringExtender(const std::string element, int amount) {
	//adds the element by the number of times required
	std::string outputStr = element;

	for (int i = 0; i < amount; i++) {
		outputStr += element;
	}

	return outputStr;
}

void display() {
	if (initialItem.title.size() > 0) {
		std::cout << stringExtender("-", SCREEN_WIDTH) << "\n";
		std::cout << stringExtender(" ", (SCREEN_WIDTH / 2) - 1) + "|" + stringExtender(" ", (SCREEN_WIDTH / 2) - 1) + "\n";
		initialItem.show(SCREEN_WIDTH, (LENGTH / 2) + initialItem.startingPos, 1, 1, true, true, true, true, true);
		std::cout << stringExtender("-", SCREEN_WIDTH) << "\n";
	}
	else {
		std::cout << "\nNothing to display...\n";
	}
}

std::string listChildren(std::vector<FlowChartItem> childrenList) {
	std::string outputString = "[";

	for (int i = 0; i < childrenList.size(); i++) {
		if (i == childrenList.size() - 1) {
			outputString = outputString + childrenList.at(i).title.substr(0, 7) + "...]";
		}
		else {
			outputString = outputString + childrenList.at(i).title.substr(0, 7) + "...,";
		}

	}

	return outputString;
}

void displaySpecific(FlowChartItem specificItem) {
	std::cout << "\n" << stringExtender("-", SCREEN_WIDTH) << "\n";

	std::cout << "Current Item Title: \t" << specificItem.title << "\n";
	std::cout << "Parent: \t" << specificItem.parent << "\n";
	std::cout << "Starting Position: \t" << specificItem.startingPos << "\n";
	std::cout << "Center Position: \t" << specificItem.centerPos << "\n";

	//display all of the child elements below
	std::cout << stringExtender(" ", SCREEN_WIDTH / 2 - 4) << "Children\n\n";

	std::string childrenList;
	if (specificItem.children.size() > 0) {
		for (auto& child : specificItem.children) {
			childrenList = childrenList + "\n" + child.title + "\t" + "Children: " + listChildren(specificItem.children) + "\n" +
				stringExtender("-", SCREEN_WIDTH) + "\n";
		}

		std::cout << childrenList;
	}

}

int main()
{
	char input = 't';
	bool updateCompletion;
	std::string userItemTitle,userParentTitle,titleToUpdateWith;
	FlowChartItem selectedItem;
	while (input != 'q') {
		//display options for the user
		std::cout << "Add an item to your flow chart (a), select an item (s), update an item (u), remove an item (r), display your flow chart (d), or quit the program (q):\n\n";
	
		std::cin >> input;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		switch (std::tolower(input)) {
		case 'a':
			std::cout << "Enter the title of the item you want to add:\n";
			std::getline(std::cin, userItemTitle);

			if (!initialItem.checkIfUniqueTitle(userItemTitle)) {
				std::cout << "title is not unique, item could not be added...\n\n";
				break;
			}

			std::cout << "Enter the parent title of the item you want to add, if this is the first item enter 'f':\n";
			std::getline(std::cin, userParentTitle);
			if (userParentTitle == "f") {
				initialItem.title = userItemTitle;
				if (userItemTitle.size() > TITLE_CAP) {
					initialItem.titlePreview = userItemTitle.substr(0, TITLE_CAP - 3) + "...";
					initialItem.centerPos = initialItem.startingPos + (initialItem.titlePreview.size() / 2);
				}
				else {
					initialItem.centerPos = initialItem.startingPos + (initialItem.title.size() / 2);
				}

			}
			else {
				bool completionFlag = initialItem.addItem(userItemTitle, userParentTitle, initialItem.centerPos);
				if (!completionFlag) std::cout << "item could not be added because parent title " << userParentTitle << " does not exist...\n\n";
			}

			break;

		case 's':
			std::cout << "Enter the title of the item you want to select:\n";
			std::getline(std::cin, userItemTitle);

			selectedItem = initialItem.selectItem(userItemTitle);
			if (selectedItem.title.size() > 0) {
				displaySpecific(selectedItem);
			}
			else {
				std::cout << "Item could not be found...\n\n";
			}
			break;

		case 'u':
			std::cout << "Enter the title of the item you want to update:\n";
			std::getline(std::cin, userItemTitle);

			std::cout << "Enter the title you want to update this item to:\n";
			std::getline(std::cin, titleToUpdateWith);

			if (!initialItem.checkIfUniqueTitle(titleToUpdateWith)) {
				std::cout << "title is not unique, item could not be updated...\n\n";
				break;
			}
			
			updateCompletion = initialItem.updateItem(userItemTitle, titleToUpdateWith);
			if (!updateCompletion) std::cout << "Current title does not exist...\n\n";
			break;

		case 'r':
			std::cout << "Enter the title of the item you want to remove:\n";
			std::getline(std::cin, userItemTitle);

			if (userItemTitle == initialItem.title) {
				initialItem.title = "";
				initialItem.titlePreview = "";
				initialItem.children.clear();
			}
			else {
				initialItem.removeItem(userItemTitle);
			}
			break;

		case 'd':
			display();
			break;

		case 'q':
			input = 'q';
			break;
		default:
			input = 'q';
			break;
		}
	}
}
