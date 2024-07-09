#include <algorithm>
#include <dirent.h>
#include <string>
#include <sstream>
//#include "ResponseBody.hpp"
//#include "ResponseHead.hpp"
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../../error_handler/errorHandler.hpp"
#include "../../cgi/CGI.hpp"

class Response {

private:
    HttpParser _parser;
    const Config::Server* _config;
    ResponseHead responseHead;
    ResponseBody responseBody;
public:
    Response(const HttpParser& parser, const Config::Server &config, std::string path, long unsigned int clients) : _parser(parser), _config(&config), responseHead(parser, config, path, clients), responseBody(parser, config) {}

    Response(const Response& other) : _parser(other._parser), _config(other._config), responseHead(other.responseHead), responseBody(other.responseBody) {}

    Response& operator=(const Response& other) {
        if (this != &other) {
            _parser = other._parser;
            _config = other._config;
            responseHead = other.responseHead;
            responseBody = other.responseBody;
        }
        return *this;
    }

    ~Response() {}

    std::string removeTrailingSlash(const std::string& input) {
        if (!input.empty() && input[input.length() - 1] == '/') {
            return input.substr(0, input.length() - 1);
        }
        return input;
    }

    std::string generateDirectoryListing(const std::string& path, DIR* dir) {
        struct dirent* ent;
        std::ostringstream oss;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                oss << "<a href='" << removeTrailingSlash(_parser.getPath()) << "/" << ent->d_name <<"'>" <<ent->d_name<< "</a>" << "<br>";
            }
            closedir(dir);
        } else {
            oss << "Could not open directory";
        }
        std::string result = oss.str();
        return result;
    }

    void init() {
        responseHead.init();
        ErrorHandler errorHandler(_parser, responseHead, responseBody, *_config);
        if (errorHandler.isBadRequest() && errorHandler.checkMethod())
            errorHandler.handleErrorCode("403");
        if (!errorHandler.checkPath())
            errorHandler.checkMethod();
        if (_parser.isCgi) {
            // CGI
            try {
                CGI cgi(_parser, responseHead.location.root);
                responseBody.setBody(cgi.run());
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                std::string error = e.what();
                // Error handling, replace with error handler later
                if (error == "file not found")
                    errorHandler.handleErrorCode("404");
                else if (error == "file not executable")
                    errorHandler.handleErrorCode("403");
                else
                    errorHandler.handleErrorCode("500");
            }
        } else {
            DIR *dir = opendir(responseHead.fullPathToFile.c_str());
            if (responseHead.location.autoindex && dir != NULL) {
                std::string directoryListing = generateDirectoryListing(responseHead.fullPathToFile, dir);
                responseBody.setBody(directoryListing);
                responseHead.setContentLength(intToString(directoryListing.size()));
                responseHead.setStatusCode("201");
                responseHead.setStatusMessage("Created");
                closedir(dir);
            } else if (responseBody.getBody().empty()) {
                responseBody.init(responseHead.fullPathToFile);
            }
        }
    }

    std::string intToString(int value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    std::string serialize() {
        std::string body = responseBody.serialize();
        std::stringstream ss;
        ss << body.size();
        std::string length = ss.str();
        responseHead.setContentLength(length);
        std::string head = responseHead.serialize();
        std::string response = head + body;
        return response;
    }
};