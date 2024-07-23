
#pragma once
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
    const Config::Server *_config;
    ResponseHead responseHead;
    ResponseBody responseBody;
public:
    Response(const HttpParser &parser, const Config::Server &config, std::string path, long unsigned int clients);

    Response(const Response &other);

    Response &operator=(const Response &other);

    ~Response();

    std::string removeTrailingSlash(const std::string &input);

    std::string generateDirectoryListing(const std::string &path, DIR *dir);

    void init();

    std::string intToString(int value);

    std::string serialize();
};
