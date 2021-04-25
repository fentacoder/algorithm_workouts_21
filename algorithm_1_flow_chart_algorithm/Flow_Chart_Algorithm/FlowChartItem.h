#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#define TITLE_CAP 15
#define TABULAR_VIEW_MARKER "_tblvw"
#define SEPARATION 4
#define SCREEN_WIDTH 120
#define LENGTH 15
#define OFFSET 2

class FlowChartItem {

public:
	int startingPos = (SCREEN_WIDTH / 2) - 1, centerPos, length;
	std::string title = "", titlePreview = "", parent;
	std::vector<FlowChartItem> children;

	FlowChartItem() {
		centerPos = startingPos;
	}

	FlowChartItem(int startingPosition, int initLength, std::string initTitle, std::string initParent, std::vector<FlowChartItem> initChildren = {}) {
		startingPos = startingPosition;
		length = initLength;
		centerPos = (initLength / 2) + startingPosition;
		parent = initParent;

		if (initTitle.size() > TITLE_CAP) {
			titlePreview = initTitle.substr(0, TITLE_CAP - 3) + "...";
			title = initTitle;
		}
		else {
			title = initTitle;
		}
	}

	~FlowChartItem() {
		startingPos = 0;
		centerPos = 0;
		title = "";
		titlePreview = "";
		parent = "";
		children.clear();
	}

	bool operator==(FlowChartItem& f) {
		if (title == f.title) {
			return true;
		}
		else {
			return false;
		}
	}

	bool addItem(std::string titleToAdd, std::string parentTitle, int parentCenterPos) {
		if (title == parentTitle) {
			//add child to flow chart items vector
			children.push_back(FlowChartItem(determineStartingPos(parentCenterPos), length, titleToAdd, title));
			return true;
		}
		else {
			for (auto& child : children) {
				bool completionFlag = child.addItem(titleToAdd, parentTitle, child.centerPos);
				if (completionFlag) return true;
			}
			return false;
		}
	}

	FlowChartItem selectItem(std::string titleToSelect) {
		if (title == titleToSelect) {
			//select child in flow chart items vector
			return *this;
		}
		else {
			for (auto& child : children) {
				FlowChartItem childToSelect = child.selectItem(titleToSelect);
				if (childToSelect.title.size() > 0) return childToSelect;
			}
			return FlowChartItem();
		}
	}

	bool updateItem(std::string titleToUpdate, std::string titleToUpdateWith) {
		if (title == titleToUpdate) {
			//update child in flow chart items vector
			title = titleToUpdateWith;
			titlePreview = titleToUpdateWith.substr(0, TITLE_CAP - 3) + "...";
			return true;
		}
		else {
			for (auto& child : children) {
				bool completionFlag = child.updateItem(titleToUpdate, titleToUpdateWith);
				if (completionFlag) return true;
			}
			return false;
		}
	}

	std::string removeItem(std::string titleToRemove) {
		if (title == titleToRemove) {
			//remove child flow chart item(s)
			children.clear();
			return parent;
		}
		else {
			for (auto& child : children) {
				int postRemovalSiblingCount = children.size() - 1;
				std::string completionFlag = child.removeItem(titleToRemove);
				if (completionFlag.size() > 0) {
					//remove the child element
					if (title == completionFlag) {
						for (int i = 0; i < children.size(); i++) {
							if (children.at(i).title == child.title) {
								children.erase(children.begin() + i);
								break;
							}
						}

						if (title == completionFlag && postRemovalSiblingCount == 3) {
							for (auto& childItem : children) {
								replace(childItem.title, TABULAR_VIEW_MARKER, "");
								replace(childItem.titlePreview, TABULAR_VIEW_MARKER, "");
							}
						}
						return completionFlag;
					}
				}
			}
		}
	}

	void show(int screenWidth, int parentCenterPos, int siblingCount, int siblingPos, bool firstItem = false, bool tableView = false,
		bool sameLine = false, bool lastChild = false, bool firstChild = true) {
		//create bracket
		if (!firstItem && !sameLine && !tableView) bracketDisplay(parentCenterPos, siblingCount, parent.size());

		//show the current item
		int startingDisplayPos = 0;
		if (firstItem) {
			startingDisplayPos = (screenWidth / 2) - SEPARATION;
			std::cout << stringExtender(" ", startingDisplayPos - 1) << (title.size() > TITLE_CAP ? titlePreview : title) << "\n";

			showCascading(screenWidth);
		}
		else {
			//set the starting position based on which sibling it is
			if (siblingCount < 4) {
				int parentLength = parent.size();
				switch (siblingPos) {
				case 1:
					if (siblingCount == 1) {
						startingDisplayPos = parentCenterPos - parentLength;
					}
					else {
						startingDisplayPos = (parentCenterPos - parentLength - (parentLength / 2)) - SEPARATION;
					}
					break;
				case 2:
					startingDisplayPos = (parentCenterPos - (parentLength / 2));
					break;
				case 3:
					startingDisplayPos = (parentCenterPos + parentLength + (parentLength / 2)) + SEPARATION;
					break;
				default:
					break;
				}

				if (siblingCount > 1) {
					if (lastChild) {
						std::cout << stringExtender(" ", SEPARATION) << title << "\n";
					}
					else if (firstChild) {
						std::cout << stringExtender(" ", startingDisplayPos + OFFSET - 1) << title;
					}
					else {
						std::cout << stringExtender(" ", SEPARATION) << title;
					}
				}
				else {
					std::cout << stringExtender(" ", startingDisplayPos + OFFSET - 1) << title << "\n";
				}

				//now show the children items if not marked with the TABULAR_VIEW_MARKER
				showCascading(screenWidth);
			}
			else {
				//show tabular view of children
				startingDisplayPos = (parentCenterPos - (TITLE_CAP / 2));
				std::cout << stringExtender(" ", startingDisplayPos - 1) << (title.size() > TITLE_CAP ? titlePreview : title) << "\n"
					<< stringExtender(" ", startingDisplayPos - 1) << stringExtender("-", TITLE_CAP) << "\n";
			}
		}
	}

	void showCascading(int screenWidth) {
		if (children.size() > 0) {
			if (children.size() > 3) {
				for (int i = 0; i < children.size(); i++) {

					if (i == children.size() - 1) {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, true, true, true, false);
					}
					else if (i == 0) {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, true, false, false, true);
					}
					else {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, true, true, false, false);
					}
				}

			}
			else {
				for (int i = 0; i < children.size(); i++) {

					if (i == children.size() - 1) {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, false, true, true, false);
					}
					else if (i == 0) {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, false, false, false, true);
					}
					else {
						children.at(i).show(screenWidth, centerPos, children.size(), i + 1, false, false, true, false, false);
					}
				}
			}
		}
	}

	bool checkIfUniqueTitle(std::string titleToTest) {
		//compare item title
		if (title == titleToTest) return false;

		if (children.size() > 0) {
			for (auto& child : children) {
				bool completionFlag = child.checkIfUniqueTitle(titleToTest);
				if (completionFlag) return true;
			}
		}
		return true;
	}

private:
	int determineStartingPos(int parentCenterPos) {
		if (children.size() > 0) {
			int childrenSize = children.size();

			//no more than 3 children branching
			if (childrenSize > 2) {
				//show a tabular view of these elements
				int tableStartingPos = 0;
				for (int i = 0; i < children.size(); i++) {
					if (i == 0) {
						tableStartingPos = children.at(i).startingPos;
					}
					else {
						children.at(i).startingPos = tableStartingPos;
					}

					//adds marker to items that are in the table view
					children.at(i).title += TABULAR_VIEW_MARKER;
					children.at(i).titlePreview += TABULAR_VIEW_MARKER;
				}

				return tableStartingPos;
			}
			else {
				return children.at(children.size() - 1).startingPos + SEPARATION;
			}
		}
		else {
			return (parentCenterPos - (LENGTH / 2));
		}
	}

	std::string stringExtender(const std::string element, int amount) {
		//adds the element by the number of times required
		std::string outputStr = element;

		for (int i = 0; i < amount; i++) {
			outputStr += element;
		}

		return outputStr;
	}

	void bracketDisplay(int parentCenterPos, int siblingCount, int parentLength) {
		//bracket creation
		std::cout << "\n" << stringExtender(" ", parentCenterPos - (parentLength / 2) - OFFSET);

		if (siblingCount > 1) {
			for (int i = 0; i < siblingCount - 1; i++) {
				if (i == siblingCount - 2) {
					std::cout << stringExtender("-", SEPARATION) << "\n";
				}
				else {
					std::cout << stringExtender("-", SEPARATION);
				}
			}
		}
		else {
			std::cout << "\n";
		}

		//branch creation
		std::cout << stringExtender(" ", parentCenterPos - SEPARATION - OFFSET);
		if (siblingCount > 1) {
			for (int i = 0; i < siblingCount; i++) {
				if (i == siblingCount - 1) {
					std::cout << stringExtender(" ", SEPARATION) << "|\n";
				}
				else {
					std::cout << stringExtender(" ", SEPARATION) << "|";
				}
			}
		}
		else {
			std::cout << "|\n";
		}
	}

	void replace(std::string& stringToUpdate, const std::string stringToReplace, const std::string stringToReplaceWith) {
		int startPos = stringToUpdate.find(stringToReplace);

		stringToUpdate.replace(startPos, stringToReplace.length(), stringToReplaceWith);
	}
};