#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

using namespace boost::asio;
using ip::tcp;

class ConnectionHandler : public boost::enable_shared_from_this<ConnectionHandler> {
private:
    tcp::socket sock;
    unsigned int timeout;

    std::string filenamePrefix;
    unsigned int maxFileSize;
//    char* data[max_length];
    std::string data;
//    std::vector<char> data;
//    boost::asio::streambuf data;

    std::string replyServerMessage;

    boost::asio::steady_timer timer_;

    ConnectionHandler(boost::asio::io_service& io_service,unsigned int timeout,
                      std::string filenamePrefix, unsigned int maxFileSize);

public:
    typedef boost::shared_ptr<ConnectionHandler> pointer;

    // creating the pointer
    static pointer createConnectionHandler(boost::asio::io_service& io_service,
                                           unsigned int timeout,
                                           std::string filenamePrefix,
                                           unsigned int maxFileSize);

    //socket creation
    tcp::socket& getSocket();

    void start();
    void cancel();

    void handleRead(const boost::system::error_code& err, size_t bytes_transferred);

    void asyncReply();
    void handleWrite(const boost::system::error_code& err, size_t bytes_transferred);

};

#endif // CONNECTIONHANDLER_H
