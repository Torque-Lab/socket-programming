#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

class Server {
private:
    int server_fd;
    sockaddr_in address;

    int createSocket() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            cerr << "Socket creation failed\n";
            return -1;
        }
        return server_fd;
    }

    int bindSocket() {
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
        address.sin_port = htons(8080);

        return bind(server_fd, (sockaddr*)&address, sizeof(address));
    }

    int listenForClient() {
        return listen(server_fd, 5);
    }

    void acceptClient() {
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_len);

        if (client_fd == -1) {
            cerr << "Accept failed\n";
            return;
        }

        cout << "Client connected! FD = " << client_fd << endl;

        char buffer[1024] = {0};
        int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            cout << "Received message: " << buffer << endl;
        }

        const char* reply = "Hello from server!";
        send(client_fd, reply, strlen(reply), 0);
        cout << "Reply sent to client.\n";

        close(client_fd);
    }

public:
    void start() {
        if (createSocket() == -1) return;

        if (bindSocket() == -1) {
            cerr << "Bind failed\n";
            return;
        }

        if (listenForClient() == -1) {
            cerr << "Listen failed\n";
            return;
        }

        cout << "Server is listening on port 8080...\n";

        acceptClient();

        close(server_fd);
    }
};


int main() {
    Server myServer;
    myServer.start();

    return 0;
}
