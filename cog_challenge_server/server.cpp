#include "server.h"
#include "setup.h"

#include <iostream>

using std::cout;
using std::endl;

Server::Server(boost::asio::io_service& io_service,
               ServerSetup& ss, ApplicationSetup *as)
    : _acceptor(io_service, tcp::endpoint(tcp::v4(), ss.getServerPort()) ),
    serverSetup(&ss), applicationSetup(as)
{
    startAccept();
}

void Server::startAccept() {

    // socket
    ConnectionHandler::pointer connection = ConnectionHandler::createConnectionHandler(
        (boost::asio::io_context&) _acceptor.get_executor().context(),
        this->serverSetup->getServerTimeout(),this->applicationSetup->getFilenamePrefix(),
        this->applicationSetup->getFileSizeLimit()
    );

    // asynchronous accept operation and wait for a new connection.
    _acceptor.async_accept(connection->getSocket(),
                           boost::bind(&Server::handleAccept, this, connection,
                                       boost::asio::placeholders::error));
}

void Server::handleAccept(ConnectionHandler::pointer connection,
                           const boost::system::error_code& err) {

    if (!err) {
        connection->start();
    }
    startAccept();
}
