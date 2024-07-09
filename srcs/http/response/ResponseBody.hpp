#include <string>
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../files/FileHandler.hpp"

class ResponseBody {
private:
    HttpParser _parser;
    const Config::Server *_config;
    std::string body;

public:
    ResponseBody(const HttpParser &parser, const Config::Server &config);

    ResponseBody(const ResponseBody &other);

    ResponseBody &operator=(const ResponseBody &other);

    ~ResponseBody();

    void init(std::string path);

    std::string serialize();

    const std::string &getBody() const;

    void setBody(const std::string &body);
};