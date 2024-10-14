#ifndef SETUP_H
#define SETUP_H

#include <memory>
#include <string>

////////// Server info

class ServerSetup {
private:
    unsigned int serverPort;
    unsigned int serverTimeout;

    static const int DEFAULT_PORT;
    static const int DEFAULT_TIMEOUT;

public:
    ServerSetup();
    ServerSetup(unsigned int serverPort, unsigned int serverTimeout);

    void loadServerSetup();

    unsigned int getServerPort() const;
    unsigned int getServerTimeout() const;

};


////////// App info

class ApplicationSetup : std::enable_shared_from_this<ApplicationSetup>{
private:
    std::string filenamePrefix;
    unsigned int fileSizeLimit;

    ApplicationSetup(std::string filenamePrefix, unsigned int fileSizeLimit);
public:
    typedef std::shared_ptr<ApplicationSetup> pointer;
    static pointer createApplicationSetup();

    std::string getFilenamePrefix() const;
    unsigned int getFileSizeLimit() const;
};


#endif // SETUP_H
