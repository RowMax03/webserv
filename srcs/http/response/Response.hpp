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
    std::string path;
    long unsigned int numClients;
public:
    Response(const HttpParser& parser, const Config::Server &config, std::string path, long unsigned int clients) : _parser(parser), _config(&config), responseHead(parser, config), responseBody(parser, config), path(path), numClients(clients) {}

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
        responseHead.location_path = path;
        responseHead.numCLients = numClients;
        std::cout << "Location matched: " << responseHead.location_path << std::endl;

        responseHead.init();
        std::cout << "Location matched: " << responseHead.fullPathToFile << std::endl;
        ErrorHandler errorHandler(_parser, responseHead, responseBody, *_config);
        if (errorHandler.isBadRequest() && errorHandler.checkMethod())
            errorHandler.handleErrorCode("403");
        errorHandler.checkPath();
        errorHandler.checkMethod();
        DIR* dir = opendir(responseHead.fullPathToFile.c_str());
        if (responseHead.location.autoindex && dir != NULL){
            std::string directoryListing = generateDirectoryListing(responseHead.fullPathToFile, dir);
            responseBody.setBody(directoryListing);
            responseHead.setContentLength(std::to_string(directoryListing.size()));
            responseHead.setStatusCode("201");
            responseHead.setStatusMessage("Created");
            closedir(dir);
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