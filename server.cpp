#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <stdexcept>

#include "header.h"
#include "functions.cpp"

using namespace std;

const int PORT = 7435;


class SocketRAII {
public:
    SocketRAII(int fd) : fd_(fd) {}
    ~SocketRAII() {
        if (close(fd_) < 0) {
            cerr << "Ошибка при закрытии сокета" << endl;
        }
    }
    int get() const { return fd_; }
private:
    int fd_;
};

void handle_client(shared_ptr<SocketRAII> client_socket) {
    cout << "Клиент подключен." << endl;

    char buffer[1024] = {0};
    int bytesReceived;

    try {
        while ((bytesReceived = read(client_socket->get(), buffer, sizeof(buffer) - 1)) > 0) {
            string input(buffer); // Создаем строку из полученных данных
            cout << "Получен запрос: " << input << endl;

            // Передаем данные в функцию обработки
            string communication = communicate_with_client(input, client_socket->get());
            
            // Отправляем ответ клиенту
            send(client_socket->get(), communication.c_str(), communication.size(), 0);
            cout << "Ответ отправлен клиенту." << endl;

            // Очищаем буфер
            memset(buffer, 0, sizeof(buffer));
        }

        if (bytesReceived < 0) {
            throw runtime_error("Ошибка при чтении данных от клиента");
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

    cout << "Клиент отключен." << endl;
}

void start_server() {
    SocketRAII server_socket(socket(AF_INET, SOCK_STREAM, 0));
    if (server_socket.get() < 0) {
        throw runtime_error("Ошибка создания сокета");
    }

    int opt = 1;
    if (setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        throw runtime_error("Ошибка установки параметров сокета");
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htons(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_socket.get(), (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw runtime_error("Ошибка привязки сокета");
    }

    if (listen(server_socket.get(), 3) < 0) {
        throw runtime_error("Ошибка при прослушивании");
    }

    cout << "Сервер запущен и ждет соединения" << endl;

    // Добавить первого администратора
    string start_message = before_login(); 
    cout << start_message;
    while (true) {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int new_socket = accept(server_socket.get(), (struct sockaddr*)&client_address, &addrlen);
        if (new_socket < 0) {
            cerr << "Ошибка принятия соединения" << endl;
            continue;
        }

        auto client_socket = make_shared<SocketRAII>(new_socket);
        thread client_thread(handle_client, client_socket);
        client_thread.detach();  // Отсоединяем поток, чтобы он работал независимо
    }
}

int main() {
    try {
        start_server();
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return -1;
    }

    return 0;
}
