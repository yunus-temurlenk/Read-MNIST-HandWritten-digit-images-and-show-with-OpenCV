#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>


// Function to read IDX3-UBYTE files
std::vector<std::vector<unsigned char>> readIDX3UByteFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the IDX3-UBYTE file." << std::endl;
        return {};
    }

    // Read the IDX3-UBYTE file header
    char magicNumber[4];
    char numImagesBytes[4];
    char numRowsBytes[4];
    char numColsBytes[4];

    file.read(magicNumber, 4);
    file.read(numImagesBytes, 4);
    file.read(numRowsBytes, 4);
    file.read(numColsBytes, 4);
    std::cout<<static_cast<int>(numImagesBytes[0])<<"  "<<static_cast<int>(numImagesBytes[1])<<"  "<<
        (int)static_cast<unsigned char>(numImagesBytes[2])<<"  "<<static_cast<int>(numImagesBytes[3])<<"  "<<std::endl;

    // Convert the header information from big-endian to native endianness
    int numImages = (static_cast<unsigned char>(numImagesBytes[0]) << 24) | (static_cast<unsigned char>(numImagesBytes[1]) << 16) | (static_cast<unsigned char>(numImagesBytes[2]) << 8) | static_cast<unsigned char>(numImagesBytes[3]);
    int numRows = (static_cast<unsigned char>(numRowsBytes[0]) << 24) | (static_cast<unsigned char>(numRowsBytes[1]) << 16) | (static_cast<unsigned char>(numRowsBytes[2]) << 8) | static_cast<unsigned char>(numRowsBytes[3]);
    int numCols = (static_cast<unsigned char>(numColsBytes[0]) << 24) | (static_cast<unsigned char>(numColsBytes[1]) << 16) | (static_cast<unsigned char>(numColsBytes[2]) << 8) | static_cast<unsigned char>(numColsBytes[3]);

    // Initialize a vector to store the images
    std::vector<std::vector<unsigned char>> images;

    for (int i = 0; i < numImages; i++) {
        // Read each image as a vector of bytes
        std::vector<unsigned char> image(numRows * numCols);
        file.read((char*)(image.data()), numRows * numCols);

        images.push_back(image);
    }

    file.close();

    return images;
}

// Function to read IDX3-UBYTE files
std::vector<std::vector<unsigned char>> readLabelFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open the IDX3-UBYTE file." << std::endl;
        return {};
    }

    // Read the IDX3-UBYTE file header
    char magicNumber[4];
    char numImagesBytes[4];

    file.read(magicNumber, 4);
    file.read(numImagesBytes, 4);

    // Convert the header information from big-endian to native endianness
    int numImages = (static_cast<unsigned char>(numImagesBytes[0]) << 24) | (static_cast<unsigned char>(numImagesBytes[1]) << 16) | (static_cast<unsigned char>(numImagesBytes[2]) << 8) | static_cast<unsigned char>(numImagesBytes[3]);

    // Initialize a vector to store the images
    std::vector<std::vector<unsigned char>> images;

    for (int i = 0; i < numImages; i++) {
        // Read each image as a vector of bytes
        std::vector<unsigned char> image(1);
        file.read((char*)(image.data()), 1);

        images.push_back(image);
    }

    file.close();

    return images;
}

int main() {

    std::string filename = "/home/cvlab/Downloads/train-images-idx3-ubyte/train-images.idx3-ubyte";
    std::string label_filename = "/home/cvlab/Downloads/train-labels-idx1-ubyte/train-labels.idx1-ubyte";

    std::vector<std::vector<unsigned char>> imagesFile = readIDX3UByteFile(filename);
    std::vector<std::vector<unsigned char>> labelsFile = readLabelFile(label_filename);
    std::vector<cv::Mat> imagesData;  // Store your images
    std::vector<int> labelsData;      // Corresponding labels

    for(int imgCnt=0; imgCnt<(int)imagesFile.size(); imgCnt++)
    {
        int rowCounter = 0;
        int colCounter = 0;

        cv::Mat tempImg = cv::Mat::zeros(cv::Size(28,28),CV_8UC1);
        for (int i = 0; i < (int)imagesFile[imgCnt].size(); i++) {

            tempImg.at<uchar>(cv::Point(colCounter++,rowCounter)) = (int)imagesFile[imgCnt][i];
            if ((i) % 28 == 0) {
                rowCounter++;
                colCounter= 0;
                if(i == 756)
                    break;
            }
        }
        std::cout<<(int)labelsFile[imgCnt][0]<<std::endl;

        imagesData.push_back(tempImg);
        labelsData.push_back((int)labelsFile[imgCnt][0]);

// to visualize each image ,n dataset  to check only
//        cv::imshow("TempImg",tempImg);
//        cv::waitKey(0);
    }


    return 0;
}
