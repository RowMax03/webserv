
#pragma once
#include <algorithm>
#include <dirent.h>
#include <string>
#include <sstream>
#include <map>
//#include "ResponseBody.hpp"
//#include "ResponseHead.hpp"
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../../error_handler/errorHandler.hpp"
#include "../../cgi/CGI.hpp"
#include "../../http/files/FileHandler.hpp"
#include "../../http/files/UploadHandler.hpp"
#include "./SessionHandler.hpp"
#include "../files/FileHandler.hpp"
class Response {

private:
    const Config::Server *_config;
	Config::Location _location;
    ResponseHead responseHead;
    ResponseBody responseBody;
	SessionHandler *sessionHandler;
	ErrorHandler errorHandler;
 long unsigned int clients;
 std::string body;
public:
    HttpParser Parser;
    Response(const Config::Server &config, SessionHandler &sessionHandler, long unsigned int clients);
    Response(const Response &other);
    Response &operator=(const Response &other);
    ~Response();

    void recive(const std::string &request);
    void handleHead();
    void handleBody();
    std::string serialize();
    void handlePost();
    void handleGet();
    void handleDelete();
    void handleCgi();

    std::string removeTrailingSlash(const std::string &input);
    std::string generateDirectoryListing(const std::string &path, DIR *dir);
    void setMimeType(const std::string& filePath);
    std::string intToString(int value);
};
