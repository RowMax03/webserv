#include <string>
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "../files/FileHandler.hpp"
class ResponseBody {
private:
    HttpParser _parser;
    const Config::Server* _config;
    std::string body;

public:
    ResponseBody(const HttpParser& parser, const Config::Server &config) : _parser(parser), _config(&config) {
        setBody("");
    }

    ResponseBody(const ResponseBody& other) : _parser(other._parser), _config(other._config), body(other.body) {}

    ResponseBody& operator=(const ResponseBody& other) {
        if (this != &other) {
            _parser = other._parser;
            _config = other._config;
            body = other.body;
        }
        return *this;
    }

    ~ResponseBody() {}

    void init(std::string path) {
        FileHandler fileHandler;
        setBody(fileHandler.readFile(path));
    }

    std::string serialize() {
        if(!body.empty())
            return body;
        return "";
    }

    const std::string& getBody() const {
        return body;
    }

    void setBody(const std::string& body) {
        this->body = body;
    }
};