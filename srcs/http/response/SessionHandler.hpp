#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib> // For srand, rand
#include <ctime> // For time

#include "ResponseHead.hpp"
#include "../parser/HttpParser.hpp"

struct Login {
    std::string username;
    std::string password;

    // Constructor for easy initialization
    Login(const std::string& user, const std::string& pass) : username(user), password(pass) {}

    // Equality operator to compare two Login objects
    bool operator==(const Login& other) const {
        return username == other.username && password == other.password;
    }
};


class SessionHandler {
private:
    std::vector<Login> _logins; // Assuming Login is a defined class
    ResponseHead *_responseHead; // Assuming ResponseHead is a defined class
    HttpParser _parser; // Assuming HttpParser is a defined class
    std::map<std::string, std::string> _sessionStorage; // Session storage

public:
    // Default constructor
  SessionHandler(ResponseHead &responseHead, HttpParser parser)
    : _responseHead(&responseHead), _parser(parser) {
        _logins.push_back(Login("user1", "password1"));
        _logins.push_back(Login("user2", "password2"));
		    std::cout << "SessionHandler constructor" << std::endl;
    std::string cookie = _parser.getHeaders()["Cookie"]; // Ensure getHeader is correctly named
    if(cookie != "") {
        checkSession(); // Ensure this method is declared and defined
    } else {
        std::map<std::string, std::string> body = parseBody(_parser.getBody());
std::cout << "body: " << body["name"] << std::endl;
        generateSession(body["name"], body["password"]); // Ensure this method is declared and defined
    }
}

// Corrected copy constructor
SessionHandler(const SessionHandler& other)
    : _responseHead(other._responseHead), _parser(other._parser), _sessionStorage(other._sessionStorage) {
    // Copy constructor logic here
}

// Corrected definition of parseBody without extra qualification
std::map<std::string, std::string> parseBody(const std::string& body) {
    std::map<std::string, std::string> parsedBody;
    std::istringstream bodyStream(body);
    std::string pair;

    while (std::getline(bodyStream, pair, '&')) {
        std::size_t delimiterPos = pair.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = pair.substr(0, delimiterPos);
            std::string value = pair.substr(delimiterPos + 1);
            parsedBody[key] = value;

            // Debugging print statements
            std::cout << "Key: " << key << ", Value: " << value << std::endl;
        }
    }

    return parsedBody;
}
    // Copy assignment operator
    SessionHandler& operator=(const SessionHandler& other) {
        if (this != &other) {
            _sessionStorage = other._sessionStorage;
            _responseHead = other._responseHead;
            _parser = other._parser;
        }
        return *this;
    }

void generateSession(const std::string& username, const std::string& password) {
    Login login(username, password);
    if (loginExists(login)) {
        std::string session_id = generateUniqueSessionId();
		_responseHead->setCookie("session_id=" + session_id);
        _sessionStorage[session_id] = username; // Corrected to store string
    }
}
bool loginExists(const Login& login) {
    for (std::vector<Login>::const_iterator it = _logins.begin(); it != _logins.end(); ++it) {
        if (*it == login) {
            return true;
        }
    }
    return false;
}
 void checkSession() {
 }

    // Destructor
~SessionHandler() {
    // Explicit destructor logic here (if necessary)
}
    std::string generateUniqueSessionId() {
        std::stringstream ss;
        ss << time(nullptr) << rand(); // Simple unique ID generation
        return ss.str();
    }
    // Additional member functions here
};
