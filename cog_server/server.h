#ifndef SERVER_H
#define SERVER_H

#include "connectionhandler.h"

class ServerSetup;
class ApplicationSetup;

class Server {
private:
    tcp::acceptor _acceptor;

    ServerSetup* serverSetup;
    ApplicationSetup* applicationSetup;

    void startAccept();
public:
    //constructor for accepting connection from client
    Server(boost::asio::io_service& io_service, ServerSetup& ss, ApplicationSetup* as);

    void handleAccept(ConnectionHandler::pointer connection,
                       const boost::system::error_code& err);
};

#endif // SERVER_H
