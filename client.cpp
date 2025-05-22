#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class Client {
private:
    int sock;
    sockaddr_in server_address;

    int createSocket() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            std::cerr << "Socket creation failed\n";
            return -1;
        }
        return sock;
    }

    bool configureServerAddress(const std::string& ip, int port) {
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0) {
            std::cerr << "Invalid address / Address not supported\n";
            return false;
        }
        return true;
    }

    bool connectToServer() {
        if (connect(sock, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
            std::cerr << "Connection Failed\n";
            return false;
        }
        return true;
    }

    void sendMessageAndReceiveReply() {
        const char* msg = "Hello from client!";
        send(sock, msg, strlen(msg), 0);
        std::cout << "Message sent to server.\n";

        char buffer[1024] = {0};
        int bytes_received = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received reply: " << buffer << std::endl;
        }
    }

public:
    void start(const std::string& ip, int port) {
        if (createSocket() == -1) return;

        if (!configureServerAddress(ip, port)) return;

        if (!connectToServer()) return;

        std::cout << "Connected to server!\n";

        sendMessageAndReceiveReply();

        close(sock);
    }
};


int main() {
    Client myClient;
    myClient.start("127.0.0.1", 8080);
    return 0;
}
