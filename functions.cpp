#include "header.h"

string sha256(const string &password) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password.c_str(), password.size());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    stringstream ss;
    for(unsigned int i = 0; i < hash_len; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Функция добавления первого администратора
string add_first_admin(connection &C) {
    string answer;
    work W(C);
    
    // Проверяем, существует ли уже пользователь Admin1
    string check_sql = "SELECT COUNT(*) FROM users WHERE user_login = 'Admin1';";
    result R = W.exec(check_sql);
    int user_exists = R[0][0].as<int>();
    
    if (user_exists > 0) {
        answer = "Пользователь Admin1 уже существует в системе.\n";
        W.commit();
        return answer;
    }
    
    // Данные администратора
    string login = "Admin1";
    string password = "123pass";
    string hashed_password = sha256(password);
    
    // Вставляем администратора в базу данных
    string insert_sql = "INSERT INTO users (user_login, user_role, user_password) VALUES (" + 
                        W.quote(login) + ", 'Администратор', " + W.quote(hashed_password) + ");";
    
    try {
        W.exec(insert_sql);
        W.commit();
        answer = "Первый администратор Admin1 успешно добавлен.\n";
    } catch (const exception &e) {
        W.abort();
        answer = "Ошибка добавления администратора: " + string(e.what()) + "\n";
    }
    
    return answer;
}

string add_user(connection &C, const string &login, const string &password){
    string answer;
    work W(C);
    
    string check_sql = "SELECT COUNT(*) FROM users WHERE user_login = " + W.quote(login) + ";";
    result R = W.exec(check_sql);
    int user_exists = R[0][0].as<int>();
    
    if (user_exists > 0) {
        answer = "Пользователь уже существует в системе.\n";
        W.commit();
        return answer;
    }
    
    string hashed_password = sha256(password);
    
    string insert_sql = "INSERT INTO users (user_login, user_role, user_password) VALUES (" + 
                        W.quote(login) + ", 'Пользователь', " + W.quote(hashed_password) + ");";
    
    try {
        W.exec(insert_sql);
        W.commit();
        answer = "Пользователь успешно добавлен.\n";
    } catch (const exception &e) {
        W.abort();
        answer = "Ошибка добавления пользователя: " + string(e.what()) + "\n";
    }
    
    return answer;
}

// Функция проверки пользователя
string check_user(connection &C, const string &login, const string &password) {
    string answer;
    work W(C);
    
    try {
        // Проверяем существование пользователя
        string check_sql = "SELECT user_password, user_role FROM users WHERE user_login = " + W.quote(login) + ";";
        result R = W.exec(check_sql);
        
        if (R.empty()) {
            answer = "Пользователь с таким логином не найден.\n";
            W.commit();
            return answer;
        }
        
        // Получаем хеш пароля из БД
        string db_password_hash = R[0][0].as<string>();
        string user_role = R[0][1].as<string>();
        string input_password_hash = sha256(password);
        
        // Сравниваем хеши
        if (db_password_hash == input_password_hash) {
            if (user_role == "Администратор") {
                answer = "Вход админа\n";
            } else {
                answer = "Вход пользователя\n";
            }
        } else {
            answer = "Неверный пароль.\n";
        }
        
        W.commit();
    } catch (const exception &e) {
        W.abort();
        answer = "Ошибка при проверке пользователя: " + string(e.what()) + "\n";
    }
    
    return answer;
}

// Функция изменения роли пользователя
string change_user_role(connection &C, const string &login, const string &new_role) {
    string answer;
    work W(C);
    
    try {
        // Проверяем текущую роль пользователя
        string check_sql = "SELECT user_role FROM users WHERE user_login = " + W.quote(login) + ";";
        result R = W.exec(check_sql);
        
        if (R.empty()) {
            answer = "Пользователь с таким логином не найден.\n";
            W.commit();
            return answer;
        }
        
        string current_role = R[0][0].as<string>();
        
        if (current_role == new_role) {
            answer = "У пользователя уже установлена роль '" + new_role + "'.\n";
            W.commit();
            return answer;
        }
        
        // Обновляем роль
        string update_sql = "UPDATE users SET user_role = " + W.quote(new_role) + 
                           " WHERE user_login = " + W.quote(login) + ";";
        W.exec(update_sql);
        W.commit();
        
        answer = "Роль пользователя успешно изменена на '" + new_role + "'.\n";
    } catch (const exception &e) {
        W.abort();
        answer = "Ошибка при изменении роли пользователя: " + string(e.what()) + "\n";
    }
    
    return answer;
}

string add_shop(connection &C, const string &shop_name, const string &address_name) {
    string answer;
    work W(C);
    
    // Вставляем адрес, если он не существует
    string sql = "INSERT INTO adresses (adress_name) VALUES (" + W.quote(address_name) + ") ON CONFLICT (adress_name) DO NOTHING RETURNING adress_id;";
    result R = W.exec(sql);
    
    int address_id;
    if (R.size() > 0) {
        address_id = R[0][0].as<int>();
    } else {
        // Получаем существующий адрес
        sql = "SELECT adress_id FROM adresses WHERE adress_name = " + W.quote(address_name) + ";";
        R = W.exec(sql);
        address_id = R[0][0].as<int>();
    }

    // Вставляем магазин
    sql = "INSERT INTO shops (shop_name, adress_id) VALUES (" + W.quote(shop_name) + ", " + to_string(address_id) + ");";
    W.exec(sql);
    
    W.commit();
    answer = "Shop added successfully.\n";
    return answer;
}

string add_product_to_shop(connection &C, const string &shop_name, const string &address_name, const string &product_name, double price, int quantity) {
    string answer;
    work W(C);

    // Получаем adress_id
    string sql = "SELECT adress_id FROM adresses WHERE adress_name = " + W.quote(address_name) + ";";
    result R = W.exec(sql);
    if (R.empty()) {
        answer = "Address not found.\n";
        return answer;
    }
    int adress_id = R[0][0].as<int>();

    // Получаем shop_id, используя и название магазина, и adress_id
    sql = "SELECT shop_id FROM shops WHERE shop_name = " + W.quote(shop_name) + " AND adress_id = " + to_string(adress_id) + ";";
    R = W.exec(sql);
    if (R.empty()) {
        answer = "Shop not found at the specified address.\n";
        return answer;
    }
    int shop_id = R[0][0].as<int>();

    // Проверяем, существует ли продукт
    sql = "SELECT product_id FROM products WHERE product_name = " + W.quote(product_name) + ";";
    R = W.exec(sql);
    int product_id;
    if (R.empty()) {
        // Продукта нет, создаем новый
        sql = "INSERT INTO products (product_name) VALUES (" + W.quote(product_name) + ") RETURNING product_id;";
        R = W.exec(sql);
        product_id = R[0][0].as<int>();
    } else {
        // Продукт существует, используем существующий ID
        product_id = R[0][0].as<int>();
    }

    // Проверяем существование записи о цене
    sql = "SELECT price_id FROM prices WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
    R = W.exec(sql);

    if (R.empty()) {
        // Цены нет, добавляем новую
        sql = "INSERT INTO prices (shop_id, product_id, price) VALUES (" + to_string(shop_id) + ", " + to_string(product_id) + ", " + to_string(price) + ");";
        W.exec(sql);
    } else {
        // Цена есть, обновляем существующую (можно добавить логику для другой обработки ситуации)
         sql = "UPDATE prices SET price = " + to_string(price) + " WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
         W.exec(sql);
    }

    // Проверяем существование записи о количестве
    sql = "SELECT quantity_id FROM quantities WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
    R = W.exec(sql);
    if (R.empty()) {
         // Количества нет, добавляем новое
         sql = "INSERT INTO quantities (shop_id, product_id, quantity) VALUES (" + to_string(shop_id) + ", " + to_string(product_id) + ", " + to_string(quantity) + ");";
         W.exec(sql);
    } else {
        // Количество есть, обновляем существующую (можно добавить логику для другой обработки ситуации)
         sql = "UPDATE quantities SET quantity = " + to_string(quantity) + " WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
         W.exec(sql);
    }

    W.commit();
    answer = "Product added/updated successfully.\n";
    return answer;
}



string find_product(connection &C, const string &product_name) {
    stringstream resultStream;
    work W(C);

    string sql = "SELECT shops.shop_name, adresses.adress_name, prices.price, quantities.quantity "
                 "FROM shops "
                 "JOIN adresses ON shops.adress_id = adresses.adress_id "
                 "JOIN prices ON shops.shop_id = prices.shop_id "
                 "JOIN products ON prices.product_id = products.product_id "
                 "JOIN quantities ON shops.shop_id = quantities.shop_id AND prices.product_id = quantities.product_id "
                 "WHERE products.product_name = " + W.quote(product_name) + ";";

    result R = W.exec(sql);

    if (R.empty()) {
        resultStream << "Product not found.";
    } else {
        for (auto row : R) {
            resultStream << "Shop: " << row[0].as<string>() << ", Address: " << row[1].as<string>() << ", Price: " << row[2].as<double>() << ", Quantity: " << row[3].as<int>() << "\n";
        }
    }

    W.commit();
    return resultStream.str();
}

string find_shop_addresses(connection &C, const string &shop_name) {
    stringstream resultStream;
    work W(C);

    string sql = "SELECT adresses.adress_name "
                 "FROM shops "
                 "JOIN adresses ON shops.adress_id = adresses.adress_id "
                 "WHERE shops.shop_name = " + W.quote(shop_name) + ";";

    result R = W.exec(sql);

    if (R.empty()) {
        resultStream << "Shop not found.";
    } else {
        resultStream << "Addresses for shop '" << shop_name << "':\n";
        for (auto row : R) {
            resultStream << "- " << row[0].as<string>() << "\n";
        }
    }

    W.commit();
    return resultStream.str();
}

string delete_product_from_shop(connection &C, const string &shop_name, const string &product_name, const string &adress_name) {
    stringstream resultStream;
    work W(C);

    try {
        // Находим adress_id
        string find_adress_id_sql = "SELECT adress_id FROM adresses WHERE adress_name = " + W.quote(adress_name) + ";";
        result R_adress_id = W.exec(find_adress_id_sql);

        if (R_adress_id.empty()) {
            throw runtime_error("Address not found.");
        }

        int adress_id = R_adress_id[0][0].as<int>();

        // Находим shop_id
        string find_shop_id_sql = "SELECT shop_id FROM shops WHERE shop_name = " + W.quote(shop_name) + " AND adress_id = " + to_string(adress_id) + ";";
        result R_shop_id = W.exec(find_shop_id_sql);

        if (R_shop_id.empty()) {
            throw runtime_error("Shop not found at this address.");
        }

        int shop_id = R_shop_id[0][0].as<int>();


        // Находим product_id
        string find_product_id_sql = "SELECT product_id FROM products WHERE product_name = " + W.quote(product_name) + ";";
        result R_product_id = W.exec(find_product_id_sql);

        if (R_product_id.empty()) {
            throw runtime_error("Product not found.");
        }

        int product_id = R_product_id[0][0].as<int>();

        // Удаляем количество и цену для конкретного магазина и товара
        string delete_quantities_sql = "DELETE FROM quantities WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
        W.exec(delete_quantities_sql);

        string delete_prices_sql = "DELETE FROM prices WHERE shop_id = " + to_string(shop_id) + " AND product_id = " + to_string(product_id) + ";";
        W.exec(delete_prices_sql);

        W.commit();
        resultStream << "Product deleted from shop successfully.";


    } catch (const exception& e) {
        W.abort();
        resultStream << "Error deleting product from shop: " << e.what();
    }
    return resultStream.str();
}


string delete_shop(connection &C, const string &shop_name, const string &adress_name) {
    stringstream resultStream;
    work W(C);

    try {
        // Находим adress_id по adress_name
        string find_adress_id_sql = "SELECT adress_id FROM adresses WHERE adress_name = " + W.quote(adress_name) + ";";
        result R_adress_id = W.exec(find_adress_id_sql);

        if (R_adress_id.empty()) {
            throw runtime_error("Address not found.");
        }

        int adress_id = R_adress_id[0][0].as<int>();

        // Находим shop_id по shop_name и adress_id
        string find_shop_id_sql = "SELECT shop_id FROM shops WHERE shop_name = " + W.quote(shop_name) + " AND adress_id = " + to_string(adress_id) + ";";
        result R_shop_id = W.exec(find_shop_id_sql);
       
        if (R_shop_id.empty()) {
             throw runtime_error("Shop not found at this address.");
        }

        int shop_id = R_shop_id[0][0].as<int>();


        // Удаляем все связанные данные, используя shop_id
        string delete_quantities_sql = "DELETE FROM quantities WHERE shop_id = " + to_string(shop_id) + ";";
        W.exec(delete_quantities_sql);

        string delete_prices_sql = "DELETE FROM prices WHERE shop_id = " + to_string(shop_id) + ";";
        W.exec(delete_prices_sql);

        string delete_shop_sql = "DELETE FROM shops WHERE shop_id = " + to_string(shop_id) + ";";
        W.exec(delete_shop_sql);

        // Удаляем адрес, если он больше не используется
        string delete_adress_sql = "DELETE FROM adresses WHERE adress_id = " + to_string(adress_id) + " AND adress_id NOT IN (SELECT adress_id FROM shops);";
        W.exec(delete_adress_sql);
       

        W.commit();
        resultStream << "Shop deleted successfully.\n";

    } catch (const exception& e) {
        W.abort();
        resultStream << "Error deleting shop: " << e.what();
    }
    return resultStream.str();
}

//Функция для проверки корректности введенного числа
bool isValidNumber(const string& str) {
    stringstream ss(str);
    int num;
    return (ss >> num) && ss.eof();
}

bool isValidDouble(const string& str) {
    stringstream ss(str);
    double num;
    return (ss >> num) && ss.eof();
}

int stringToInt(const string& str) {
    try {
        return stoi(str);
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        throw;
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        throw;
    }
}

double stringToDouble(const string& str) {
    try {
        return stod(str);
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        throw;
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        throw;
    }
}

string before_login(){
    string result;
    try{
        connection C("dbname=shopsdb user=postgres password=root \
                            host=db port=5432");
        result = add_first_admin(C);
    }
    catch (const std::exception &e) {
            cerr << e.what() << std::endl;
            result = "Ошибка: проблема с подключением к базе данных\n";
        }
    return result;
}

string communicate_with_client(string& message, int client_socket_fd){
    string result;
    stringstream ss(message);
    string command;
    // Разделяем сообщение по разделителю '|'
    getline(ss, command, '|');
    try {
        connection C("dbname=shopsdb user=postgres password=root \
                            host=db port=5432");
        if (command == "register"){
            string username;
            string userpass;
            getline(ss, username, '|');
            getline(ss, userpass, '|');
            result = add_user(C, username, userpass);
        }
        else if (command == "check") {
            string username;
            string userpass;
            getline(ss, username, '|');
            getline(ss, userpass, '|');
            result = check_user(C, username, userpass);
        }
        else if (command == "change") {
            string username;
            string userrole;
            getline(ss, username, '|');
            getline(ss, userrole, '|');
            result = change_user_role(C, username, userrole);
        }
        else if (command == "1") {
            string shopName;
            string shopAddress;
            // Получаем название и адрес магазина
            getline(ss, shopName, '|');
            getline(ss, shopAddress, '|');
            result = add_shop(C, shopName, shopAddress);
        } else if (command == "2"){
            string shopName;
            string shopAddress;
            string productName;
            string cost;
            string count;

            getline(ss, shopName, '|');
            getline(ss, shopAddress, '|');
            getline(ss, productName, '|');
            getline(ss, cost, '|');
            getline(ss, count, '|');

            double price;
            int quantity;
            if (isValidDouble(cost)){
                price = stringToDouble(cost);
            }
            else{
                result = "Ошибка: Неккоректный ввод стоимости\n";
                return result;
            }
            if (isValidNumber(count)){
                quantity = stringToInt(count);
            }
            else{
                result = "Ошибка: Неккоректный ввод количества\n";
                return result;
            }
            result = add_product_to_shop(C, shopName, shopAddress, productName, price, quantity);
        } else if (command == "3"){
            string shopName;
            getline(ss, shopName, '|');
            result = find_shop_addresses(C, shopName);
        } else if (command == "4"){
            string productName;
            getline(ss, productName, '|');
            result = find_product(C, productName);
        } else if (command == "5"){
            string shopName;
            string shopAddress;
            // Получаем название и адрес магазина
            getline(ss, shopName, '|');
            getline(ss, shopAddress, '|');
            result = delete_shop(C, shopName, shopAddress);
        } else if (command == "6"){
            string shopName;
            string shopAddress;
            string productName;
            getline(ss, shopName, '|');
            getline(ss, shopAddress, '|');
            getline(ss, productName, '|');
            result = delete_product_from_shop(C, shopName, productName, shopAddress);
        } else {
            result = "Ошибка: неизвестная команда\n";
        }
    } catch (const std::exception &e) {
            cerr << e.what() << std::endl;
            result = "Ошибка: проблема с подключением к базе данных\n";
        }
    return result;
}
