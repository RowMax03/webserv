#pragma once
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
    std::string Base64Login;
	std::string username;
    std::string password;
	static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

    // Constructor for easy initialization
    Login(const std::string& user, const std::string& pass) : username(user), password(pass) {
		Base64Login = encodeLogin("myUsername", "myPassword");
		}
std::string base64_encode(const std::string &in) {
    std::string out;

    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string encodeLogin(const std::string& username, const std::string& password) {
    return base64_encode(username + ":" + password);
}
    bool operator==(const Login& other) const {
        return username == other.username && password == other.password;
    }
};


class SessionHandler {
private:
    std::vector<Login> _logins;
    std::map<std::string, std::string> _sessionStorage;


public:
    // Default constructor
  SessionHandler() {
        _logins.push_back(Login("user1", "password1"));
        _logins.push_back(Login("user2", "password2"));
}

// Corrected copy constructor
SessionHandler(const SessionHandler& other)
    : _sessionStorage(other._sessionStorage) {
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
        }
        return *this;
    }




void generateSession(const std::string& incomingCreds) {
    if (validateCredentials(incomingCreds)) {
        std::string session_id = generateUniqueSessionId();
		_responseHead->setCookie("session_id=" + session_id + "; Secure; HttpOnly; SameSite=Strict; Max-Age=86400");
        _sessionStorage[session_id] = username; // Corrected to store string
    }else
	{
		throw std::runtime_error("401");
	}
}
    bool validateCredentials(const std::string& incomingCreds) {
        for (const auto& login : _logins) {
            if (login.encodedCredentials == incomingCreds) {
                return true; // Credentials are valid
            }
        }
        return false; // Credentials are invalid
    }

 bool checkSession(std::string cookie) {
	std::string session_id = cookie.substr(cookie.find('=') + 1);
	if (_sessionStorage.find(session_id) != _sessionStorage.end()) {
		return true;
	} else {
		return false;
	}
	return false;
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
