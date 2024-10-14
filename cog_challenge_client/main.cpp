#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main() {
    boost::asio::io_service io_service;
    // socket creation
    tcp::socket socket(io_service);

    // connection
    int port = 0;
    if( const char* varPort = std::getenv("COG_SERVER_PORT") ) {
        try {
            port = std::stoi( varPort );
        } catch(...) {
            cout << "Check the server port value in the environment variable `COG_SERVER_PORT`" << endl;
            cout << "The value `" << varPort << "` is not allowed!" << endl;
            return -1;
        }
    } else {
        cout << "Set the server port in the environment variable `COG_SERVER_PORT`" << endl;
        return -2;
    }
    socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), port ));

    // request/message from client
//    const string msg = "Eduardo"; // Tamanho: 7
    const string msg = "Hello World!\nFrom Client!\n"; // Tamanho: 26

    // Tamanho 1025
//    const string msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque interdum urna eget viverra mattis. Cras at malesuada lectus. Phasellus ultricies nisi sed egestas placerat. Sed odio metus, semper facilisis rhoncus at, semper in arcu. Nulla quis est pulvinar, aliquet odio sed, tristique ligula. Integer gravida consectetur urna, nec semper quam ullamcorper et. Sed rutrum, turpis nec interdum tristique, arcu risus convallis ligula, vel congue tortor arcu id turpis. Maecenas mi urna, porta at erat nec, volutpat imperdiet ligula. Proin fermentum vel massa ut consectetur. " \
//                       "Morbi a malesuada diam. Fusce vulputate aliquet ligula, nec semper turpis. Pellentesque tempus lorem sed mauris placerat dignissim. Nulla facilisi. Vestibulum euismod ullamcorper turpis quis aliquet. Donec aliquet, neque a commodo semper, massa libero porttitor dolor, in elementum nibh augue sit amet est. Quisque tempor, quam in suscipit iaculis, sapien ex ultricies ipsum, id consequat purus dui porta risus. Vestibulum ante ipsum primis in eu.";

    boost::system::error_code error;
    boost::asio::write( socket, boost::asio::buffer(msg), error );
    if( !error ) {
        cout << "Client sending a message with the size of: " << msg.length() << " bytes!" << endl;
    } else {
        cout << "send failed: " << error.message() << endl;
    }

    socket.shutdown(socket_base::shutdown_send); // FINAL DO ENVIO! Sem essa linha dá para testar o timeout, pois o envio não acaba.

    // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if( error && error != boost::asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
    } else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << "Response from the server: " << data << endl;
    }
    return 0;
}
