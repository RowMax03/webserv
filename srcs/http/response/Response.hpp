#include <algorithm>
#include <dirent.h>
#include <string>
#include <sstream>
//#include "ResponseBody.hpp"
//#include "ResponseHead.hpp"
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../../error_handler/errorHandler.hpp"
class Response {

private:
    HttpParser _parser;
    const Config::Server* _config;
    ResponseHead responseHead;
    ResponseBody responseBody;

public:
    Response(const HttpParser& parser, const Config::Server &config) : _parser(parser), _config(&config), responseHead(parser, config), responseBody(parser, config) {}

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

    std::string generateDirectoryListing(const std::string& path) {
        DIR* dir;
        struct dirent* ent;
        std::ostringstream oss;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                oss << ent->d_name << "\n";
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
        errorHandler.checkMethod();
        errorHandler.checkPath();
        if (responseHead.location.autoindex ) {
            std::string directoryListing = generateDirectoryListing(responseHead.fullPathToFile);
            responseBody.setBody(directoryListing);
            responseHead.setContentLength(std::to_string(directoryListing.size()));
            responseHead.setStatusCode("201");
            responseHead.setStatusMessage("Created");
        } else if (responseBody.getBody().empty()){
            responseBody.init(responseHead.fullPathToFile);
        }
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