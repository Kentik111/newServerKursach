#ifndef HEADER_H
#define HEADER_H

#include <pqxx/pqxx>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <stdexcept>
#include <sstream>

#include <openssl/evp.h>
#include <iomanip>

using namespace std;
using namespace pqxx;

string communicate_with_client(string& message, int client_socket_fd);

string add_shop(connection &C, const string &shop_name, const string &address_name);
string add_product_to_shop(connection &C, const string &shop_name, const string &address_name, const string &product_name, double price, int quantity);
string find_product(connection &C, const string &product_name);
string find_shop_addresses(connection &C, const string &shop_name);
string delete_product_from_shop(connection &C, const string &shop_name, const string &product_name, const string &adress_name);
string delete_shop(connection &C, const string &shop_name, const string &adress_name);

bool isValidNumber(const string& str);
bool isValidDouble(const string& str);
int stringToInt(const string& str);
double stringToDouble(const string& str);

string sha256(const string &password);
string add_first_admin(connection &C);

#endif