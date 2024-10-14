#include "filewriter.h"

#include <ctime>

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

FileWriter::FileWriter(std::string content, unsigned int fileSizeLimit,
                       std::string filenamePrefix)
    : content(content), fileSizeLimit(fileSizeLimit)
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);
    char formattedDatetime[25];
    strftime(formattedDatetime, 25, "%Y%m%d%H%M%S", &datetime);
    baseFilename = filenamePrefix + formattedDatetime;

    strftime(formattedDatetime, 25, "%Y/%m/%d - %H:%M:%S", &datetime);
    cout << ">>>>>>>>>> File processing timestamp <<<<<<<<<<" << endl;
    cout << "The content will be saved in the file: " << this->baseFilename << endl;
    cout << "Date and time: " << formattedDatetime << endl;
}

void FileWriter::writeFile(std::string content, std::string filename) const {
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::binary);
    file << content;
    file.close();
    cout << "File saved: " << filename << endl;
}

void FileWriter::writeFiles() const {

    int numFiles = ((this->content.size()-1) / this->fileSizeLimit) + 1;
    cout << "# of files to store: " << numFiles << endl;

    if(numFiles == 1) {
        // Store all the content in just one file
        this->writeFile(this->content,this->baseFilename);
    } else {
        // Split the content in multiple files
        cout << "Multiple files:" << endl;
        for(int fileCounter = 0; fileCounter < numFiles; fileCounter++) {
            std::string filename = this->baseFilename + "_" + std::to_string(fileCounter);
            this->writeFile(content.substr(fileCounter*fileSizeLimit, fileSizeLimit),filename);
        }
    }

}
