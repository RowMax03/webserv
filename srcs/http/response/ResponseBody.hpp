#pragma once
#include <string>
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../files/FileHandler.hpp"

class ResponseBody {
private:
    std::string body;

public:
    ResponseBody();

    ResponseBody(const ResponseBody &other);

    ResponseBody &operator=(const ResponseBody &other);

    ~ResponseBody();

    void init(std::string path);

    std::string serialize();

    const std::string &getBody() const;

    void setBody(const std::string &body);
};
