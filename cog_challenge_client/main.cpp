#include <iostream>
#include <boost/asio.hpp>

#include <fstream>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

string readinfo (std::istream& in) {

    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}


int main(int argc, char* argv[]) {

    string msg;

    /// 1. Conteúdo a ser enviado para o servidor
    /// 1. Content to be sent to the server

    // With a file to be read as command line argument
    if(argc > 1) {
        cout << "> File to read:  " << argv[1] << endl;
        std::ifstream fin(argv[1]);
        if (fin.is_open())
            msg = readinfo(fin);
        else {
            cerr << "# Error: file open failed." << endl;
            return 1;
        }
    } else {
    // Manual test cases
//        msg = "Eduardo"; // Size: 7 bytes/chars
//        msg = "Hello World!\nFrom Client!\n"; // Size: 26 bytes

        // Size 1025 bytes
        msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque interdum urna eget viverra mattis. Cras at malesuada lectus. Phasellus ultricies nisi sed egestas placerat. Sed odio metus, semper facilisis rhoncus at, semper in arcu. Nulla quis est pulvinar, aliquet odio sed, tristique ligula. Integer gravida consectetur urna, nec semper quam ullamcorper et. Sed rutrum, turpis nec interdum tristique, arcu risus convallis ligula, vel congue tortor arcu id turpis. Maecenas mi urna, porta at erat nec, volutpat imperdiet ligula. Proin fermentum vel massa ut consectetur. " \
                      "Morbi a malesuada diam. Fusce vulputate aliquet ligula, nec semper turpis. Pellentesque tempus lorem sed mauris placerat dignissim. Nulla facilisi. Vestibulum euismod ullamcorper turpis quis aliquet. Donec aliquet, neque a commodo semper, massa libero porttitor dolor, in elementum nibh augue sit amet est. Quisque tempor, quam in suscipit iaculis, sapien ex ultricies ipsum, id consequat purus dui porta risus. Vestibulum ante ipsum primis in eu.";
    }

    cout << "> Message to be sent:\n" << msg << endl;
    cout << "> Message size: " << msg.length() << " in bytes." << endl;


    /// 2. Obter a porta de comunicação do servidor via variável de ambiente
    /// 2. Getting the communcation port from the environment variable
    int port = 0;
    if( const char* varPort = std::getenv("COG_SERVER_PORT") ) {
        try {
            port = std::stoi( varPort );
            cout << "> Selected port: " << port << endl;
        } catch(...) {
            cerr << "# Check the server port value in the environment variable `COG_SERVER_PORT`" << endl;
            cerr << "# The value `" << varPort << "` is not allowed!" << endl;
            return -1;
        }
    } else {
        cerr << "# Set the server port in the environment variable `COG_SERVER_PORT`" << endl;
        return -2;
    }

    /// 3. Criação do socket para conexão com o servidor
    /// 3. Socket creation for connect with the server

    boost::asio::io_service io_service;
    tcp::socket socket(io_service);

    // Conexão / Connection
    try {
        socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), port ));
        cout << "> Connection established with the server!" << endl;
    } catch(boost::system::system_error& err) {
        cerr << "# It was not possible commnunicate with the server." << endl;
        cerr << "# The error is: " << err.what() << endl;
        return 2;
    }

    boost::system::error_code error;
    boost::asio::write( socket, boost::asio::buffer(msg), error );
    if( !error ) {
        cout << "> Client sending a message size of: " << msg.length() << " bytes!" << endl;
    } else {
        cerr << "# Send failed: " << error.message() << endl;
    }

    /// 4. Encerra a conexão para o servidor processar a mensagem
    /// 4. Closes the connection for the server process the message
    socket.shutdown(socket_base::shutdown_send); // FINAL DO ENVIO! Sem essa linha dá para testar o timeout, pois a conexão fica aberta.

    /// 5. Obter a resposta do servidor
    /// 5. Getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if( error && error != boost::asio::error::eof ) {
        cerr << "# Receive failed: " << error.message() << endl;
    } else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << ">>>> Response from the server: " << data << endl;
        cout << ">>>>>>>>> FINISHED <<<<<<<<<<" << endl;
    }
    return 0;
}
