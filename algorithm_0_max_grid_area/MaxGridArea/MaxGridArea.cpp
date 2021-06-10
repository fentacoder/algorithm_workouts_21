#include <iostream>
#include <vector>


int N = 10, M = 10;
std::vector<std::vector<int>> userMatrix { 
    {0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,1,1,1,0},
    {1,0,0,0,0,1,1,1,1,0},
    {0,0,0,0,0,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,0},
    {0,0,1,1,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,0,0},
    {0,0,0,0,1,1,1,1,0,0}
};

int countVertical(std::vector<std::vector<int>> matrix, int rectHeight, int rowPos, int colPos) {
    int count = 0;
    for (int i = rowPos; i < rectHeight; i++) {
        if (matrix[i][colPos] == 1) count++;
    }
    return count;
}

int countHorizontal(std::vector<int> row, int rectWidth, int colPos) {
    int count = 0;
    for (int i = colPos; i < rectWidth; i++) {
        if (row[i] == 1) count++;
    }
    return count;
}

int innerSearchVertical(std::vector<std::vector<int>> matrix, int rowPos, int colPos, int rectWidth, int rectHeight) {
    int returnHeight = rectHeight;
    //check for where to start scanning
    int startingPoint = colPos;
    if (M - colPos > rectWidth) startingPoint = colPos - (M - colPos);

    //scan through for the minimum width
    for (int i = colPos; i < rectWidth + colPos; i++) {
        //count continuous 1s from this rowPos
        int vertiRowPos = (N - rowPos > N - rectHeight ? (rowPos - ((N - rowPos) - (N - rectHeight))) : rowPos);
        if (vertiRowPos < 0) vertiRowPos = 0;

        int width = countVertical(matrix, rectHeight, vertiRowPos, i);
        if (width < rectWidth) returnHeight = width;
    }

    return returnHeight;
}

int innerSearchHorizontal(std::vector<std::vector<int>> matrix, int rowPos, int colPos, int rectHeight, int rectWidth) {
    int returnWidth = rectWidth;
    //check for where to start scanning
    int startingPoint = rowPos;
    if (N - rowPos > rectHeight) startingPoint = rowPos - (N - rowPos);

    //scan through for the minimum width
    for (int i = rowPos; i < rectHeight + rowPos; i++) {
        //count continuous 1s from this colPos
        int horizColPos = (M - colPos > M - rectWidth ? (colPos - ((M - colPos) - (M - rectWidth))) : colPos);
        if (horizColPos < 0) horizColPos = 0;

        int width = countHorizontal(matrix[i], rectWidth, horizColPos);
        if (width < rectWidth) returnWidth = width;
    }

    return returnWidth;
}

int bidirectionalVertical(std::vector<std::vector<int>> matrix, int rowPos, int colPos) {
    //go down
    int downLength = (N - rowPos < (int)(N / 2)) ? N - rowPos : rowPos;
    int downCount = 0;

    for (int i = rowPos; i < downLength + 1; i++) {
        if (matrix[i][colPos] == 1) {
            downCount++;
        }
        else {
            break;
        }
    }

    //go up
    int upLength = (N - rowPos >= (int)(N / 2)) ? N - rowPos : rowPos;
    int upCount = 0;

    for (int j = rowPos + 1; j < upLength; j++) {
        if (matrix[j][colPos] == 1) {
            upCount++;
        }
        else {
            break;
        }
    }

    return (downCount + upCount);
}

int bidirectionalHorizontal(std::vector<std::vector<int>> matrix, int rowPos, int colPos) {
    //go left
    int leftLength = (M - colPos < (int)(M / 2)) ? M - colPos : colPos;
    int leftCount = 0;

    for (int i = colPos; i < leftLength + 1; i++) {
        if (matrix[rowPos][i] == 1) {
            leftCount++;
        }
        else {
            break;
        }
    }

    //go right
    int rightLength = (M - colPos >= (int)(M / 2)) ? M - colPos : colPos;
    int rightCount = 0;

    for (int j = colPos + 1; j < rightLength; j++) {
        if (matrix[rowPos][j] == 1) {
            rightCount++;
        }
        else {
            break;
        }
    }

    return (leftCount + rightCount);
}

int search(std::vector<std::vector<int>> matrix, int rowPos, int colPos, int currentMaxArea) {
    int rectArea = 0;
    //rectWidth and rectHeight are the max dimensions of the rectangle from the current 1
    int rectWidth = bidirectionalHorizontal(matrix, rowPos, colPos);
    int rectHeight = bidirectionalVertical(matrix, rowPos, colPos);

    //these next two lines check for the inner ones to get the true area of the rectangle going both axis two determine the largest one

    // if newRectWidth is actually smaller, then it will be updated
    int newRectWidth = innerSearchHorizontal(matrix, rowPos, colPos, rectHeight, rectWidth);

    rectArea = newRectWidth * rectHeight;

    // if rectHeight is actually smaller, then it will be updated
    int newRectHeight = innerSearchVertical(matrix, rowPos, colPos, rectWidth, rectHeight);

    if (newRectWidth < rectWidth) rectWidth = newRectWidth;
    if (newRectHeight < rectHeight) rectHeight = newRectHeight;

    if (rectWidth * rectHeight < rectArea) rectArea = rectWidth * rectHeight;

    return (currentMaxArea > rectArea) ? currentMaxArea : rectArea;
}

void maxArea(std::vector<std::vector<int>> matrix, int matrixWidth, int matrixHeight) {
    int area = 0;
    for (int i = 0; i < matrixHeight; i++) {
        for (int j = 0; j < matrixWidth; j++) {
            if (matrix[i][j] == 1) {
                area = search(matrix, i, j, area);
                
            }
        }
    }

    std::cout << "The max area for this grid is: " << area << "\n";
}

void displayMatrix(std::vector<std::vector<int>> matrix) {
    std::cout << "\n";

    for (int j = 0; j < N; j++) {

        std::cout << "\n|";
        for (int k = 0; k < M; k++) {
            if (k < M - 1) { 
                std::cout << matrix[j][k] << ",";
            }
            else {
                std::cout << matrix[j][k];
            }
        }
        std::cout << "|";
    }
    std::cout << "\n";
}

int main()
{
    displayMatrix(userMatrix);

    maxArea(userMatrix, M, N);
}