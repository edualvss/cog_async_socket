#include "setup.h"

#include <cstdlib>
#include <iostream>



////////// Environment info

enum ENV_VAR {
    FILESIZE_LIMIT = 0,
    FILENAME_PREFIX,
    SERVER_PORT,
    SERVER_TIMEOUT
};

const char* ENV_VAR_NAMES[4] = {
    "COG_FILESIZE_LIMIT",
    "COG_FILENAME_PREFIX",
    "COG_SERVER_PORT",
    "COG_SERVER_TIMEOUT"
};

const std::string ERROR_MESSAGES[4] = {
    "You must specify a `file size limit` in the environment variable ",
    "You must specify a `filename prefix` in the environment variable ",
    "[INFO] Specify a valid `server port` in the environment variable ",
    "[INFO] Specify a valid `server timeout` in the environment variable "
};

const char* SECTION_MARKER = "####################################";

////////// Server info

const int ServerSetup::DEFAULT_TIMEOUT = 0; // No timeout
const int ServerSetup::DEFAULT_PORT = 1024; // A higer port, no super user permission need

ServerSetup::ServerSetup()
    : serverPort(DEFAULT_PORT), serverTimeout(DEFAULT_TIMEOUT) {}

ServerSetup::ServerSetup(unsigned int serverPort, unsigned int serverTimeout)
    : serverPort(serverPort),serverTimeout(serverTimeout) {}

unsigned int ServerSetup::getServerPort() const {
    return this->serverPort;
}

unsigned int ServerSetup::getServerTimeout() const {
    return this->serverTimeout;
}

void ServerSetup::loadServerSetup() {

    // Port setting load
    int port = ServerSetup::DEFAULT_PORT;
    if(const char* varServerPort = std::getenv( ENV_VAR_NAMES[SERVER_PORT] )) {
        try {
            port = std::stoi(varServerPort);
        } catch(std::exception& ex) {
            std::string conversionErrorMessage = "Data conversion error in environment VAR `";
            conversionErrorMessage += std::string(ENV_VAR_NAMES[SERVER_PORT]);
            conversionErrorMessage += "`: ";
            conversionErrorMessage += ex.what();
            conversionErrorMessage += std::string(" of `") + std::string(varServerPort)
                                      + std::string("`.");
            throw std::invalid_argument( conversionErrorMessage );
        }
    } else {
        throw std::invalid_argument( ERROR_MESSAGES[SERVER_PORT] + ENV_VAR_NAMES[SERVER_PORT] );
    }
    if(port <= 0) {
        throw std::invalid_argument(
            std::string("The env VAR value of `")
            + ENV_VAR_NAMES[SERVER_PORT]
            +"` must be greater than ZERO!");
    }
    if(port < 1024) {
        std::cout << "[WARN] The chosen port `" << port << "` is in the space of the reserved ones. "
             << "You will need super user access to run the server." << std::endl;
    }
    this->serverPort = port;


    // Timeout setting load
    int timeout = ServerSetup::DEFAULT_TIMEOUT;
    if(const char* varTimeout = std::getenv( ENV_VAR_NAMES[SERVER_TIMEOUT] )) {
        try {
            timeout = std::stoi(varTimeout);
        } catch(std::exception& ex) {
            std::string conversionErrorMessage = "Data conversion error in environment VAR `";
            conversionErrorMessage += std::string(ENV_VAR_NAMES[SERVER_TIMEOUT]);
            conversionErrorMessage += "`: ";
            conversionErrorMessage += ex.what();
            conversionErrorMessage += std::string(" of `") + std::string(varTimeout)
                                      + std::string("`.");
            throw std::invalid_argument( conversionErrorMessage );
        }
    } else {
        throw std::invalid_argument( ERROR_MESSAGES[SERVER_TIMEOUT] + ENV_VAR_NAMES[SERVER_TIMEOUT] );
    }
    if(timeout < 0) {
        throw std::invalid_argument(
            std::string("The env VAR value of `")
            + ENV_VAR_NAMES[SERVER_TIMEOUT]
            +"` must be greater than or equal to ZERO!");
    }
    this->serverTimeout = timeout;

}


////////// App info

ApplicationSetup::ApplicationSetup(std::string filenamePrefix, unsigned int fileSizeLimit)
    : filenamePrefix(filenamePrefix), fileSizeLimit(fileSizeLimit) {

    std::cout << SECTION_MARKER << std::endl;
    std::cout << "Creating an Application setup with:" << std::endl
              << "* Filename prefix: " << filenamePrefix << std::endl
              << "* File size limit: " << fileSizeLimit << " bytes" << std::endl;
    std::cout << SECTION_MARKER << std::endl;
}


std::string ApplicationSetup::getFilenamePrefix() const {
    return this->filenamePrefix;
}

unsigned int ApplicationSetup::getFileSizeLimit() const {
    return this->fileSizeLimit;
}


ApplicationSetup::pointer ApplicationSetup::createApplicationSetup() {

    std::string filenamePrefix = "";
    int fileSizeLimit = 0;


    // Get filename from ENV vars
    if( const char* varFilenamePrefix = std::getenv(ENV_VAR_NAMES[FILENAME_PREFIX]) ) {
        filenamePrefix = varFilenamePrefix;
    } else {
        throw std::invalid_argument( ERROR_MESSAGES[FILENAME_PREFIX] + ENV_VAR_NAMES[FILENAME_PREFIX] );
    }


    // Get file size limit from ENV vars
    if( const char* varFileSizeLimit = std::getenv(ENV_VAR_NAMES[FILESIZE_LIMIT]) ) {
        try {
            fileSizeLimit = std::stoi(varFileSizeLimit);
        } catch(std::exception& ex) {
            std::string conversionErrorMessage = "Data conversion error in environment VAR `";
            conversionErrorMessage += std::string(ENV_VAR_NAMES[FILESIZE_LIMIT]);
            conversionErrorMessage += "`: ";
            conversionErrorMessage += ex.what();
            conversionErrorMessage += std::string(" of `") + std::string(varFileSizeLimit)
                                      + std::string("`.");
            throw std::invalid_argument( conversionErrorMessage );
        }
    } else {
        throw std::invalid_argument( ERROR_MESSAGES[FILESIZE_LIMIT] + ENV_VAR_NAMES[FILESIZE_LIMIT] );
    }
    if(fileSizeLimit <= 0) {
        throw std::invalid_argument(
            std::string("The env VAR value of `")
                + ENV_VAR_NAMES[FILESIZE_LIMIT]
                +"` must be greater than ZERO!");
    }

    return pointer(new ApplicationSetup(filenamePrefix,fileSizeLimit));

}
