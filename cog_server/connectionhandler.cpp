#include "connectionhandler.h"

#include "filewriter.h"

#include <iostream>
using std::string;
using std::cout;
using std::endl;

extern const char* SECTION_MARKER;


ConnectionHandler::ConnectionHandler(boost::asio::io_service& io_service,
                                     unsigned int timeout,
                                     std::string filenamePrefix,
                                     unsigned int maxFileSize)
    : sock(io_service),timeout(timeout), filenamePrefix(filenamePrefix),
    maxFileSize(maxFileSize), data(""), replyServerMessage(""),
    timer_(sock.get_executor(),boost::asio::chrono::milliseconds(timeout)) {}

ConnectionHandler::pointer ConnectionHandler::createConnectionHandler(
    boost::asio::io_service& io_service,unsigned int timeout,
    std::string filenamePrefix, unsigned int maxFileSize) {

    return pointer(new ConnectionHandler(io_service,timeout,filenamePrefix,maxFileSize));
}


//socket creation
tcp::socket& ConnectionHandler::getSocket() {
    return sock;
}


void ConnectionHandler::start() {


    cout << ">>>>>>>> Start connection from: " << this->sock.remote_endpoint() << endl;

    if(timeout > 0) {
        timer_.expires_from_now(boost::asio::chrono::milliseconds(timeout));
        timer_.async_wait(std::bind(&ConnectionHandler::cancel,this));
    }

    boost::asio::async_read(sock,
                            boost::asio::dynamic_buffer(data),
                            //data,
//                            boost::asio::transfer_at_least(1), // async_read_until
                            boost::bind(&ConnectionHandler::handleRead,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred)
                            );

/*    // Fixed buffer size
    sock.async_read_some(
        boost::asio::buffer(data, this->maxFileSize+1),
        boost::bind(&ConnectionHandler::handleRead,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)
    );
*/

}

void ConnectionHandler::cancel() {
    if(this->sock.is_open()) {
        this->sock.cancel();
    }
}

void ConnectionHandler::handleRead(const boost::system::error_code& err,
                                    size_t bytes_transferred) {

    if (!err) {
        cout << SECTION_MARKER << endl;
        cout << "Data reception"<< endl;
        cout << SECTION_MARKER << endl;

        // Only with delimited buffer size
        cout << "Received Bytes: " << bytes_transferred << endl;
        cout << "Received Message - START <<< " << endl;
        cout << data << endl;
        cout << "Received Message - END >>>" << endl;

        // File not being saved here! It can be improved.
    } else {
        if (err == boost::asio::error::eof) { // Not a error
            // Surrounding approach to receive all the content from the client
            // with a End-of-Stream/File signal: "shutdown_sending".

            this->timer_.cancel();

            cout << SECTION_MARKER << endl;
            cout << "Data reception"<< endl;
            cout << SECTION_MARKER << endl;

            cout << "Received Bytes: " << bytes_transferred << endl;
            cout << "Message Received!" << endl;

            if(bytes_transferred == 0) {
                this->replyServerMessage = "Empty content. File not saved.";
            } else {
                // The file writer also could be asynchronous
                FileWriter fw(data,maxFileSize,filenamePrefix);
                fw.writeFiles();
                this->replyServerMessage = "Content stored.";
            }

        } else {
            if(err == boost::asio::error::operation_aborted) {
                // Timeout
                this->replyServerMessage = "Connection timeout.";
                cout << ">>>>> Timeout" << endl;
            } else {
                // Any other error
                this->replyServerMessage = "Error on the server: " + err.message();
                std::cerr << "error: " << err.message() << std::endl;
            }
        }

        cout << "...Ending connection..." << endl;

        this->asyncReply();
        sock.close();
    }
}

void ConnectionHandler::asyncReply() {

    sock.async_write_some(
        boost::asio::buffer(replyServerMessage, replyServerMessage.length()),
        boost::bind(&ConnectionHandler::handleWrite,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void ConnectionHandler::handleWrite(const boost::system::error_code& err,
                                     size_t bytes_transferred) {
    cout << "[ConnectionHandler] Data sent."<< endl;
    if (!err) {
        cout << SECTION_MARKER << endl;
        cout << "Server sent reply!"<< endl;
        cout << SECTION_MARKER << endl;
    } else {
        std::cerr << "error on sent: " << err.message() << endl;
        sock.close();
    }
}

