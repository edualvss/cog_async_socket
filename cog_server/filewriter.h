#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>

class FileWriter {
private:
    std::string content;
    unsigned int fileSizeLimit;
    std::string baseFilename;

    void writeFile(std::string content,std::string filename) const;

public:
    FileWriter(std::string content,unsigned int fileSizeLimit, std::string filenamePrefix);
    void writeFiles() const;
};

#endif // FILEWRITER_H
