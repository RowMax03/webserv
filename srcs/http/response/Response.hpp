#include <string>
#include "ResponseBody.hpp"
#include "ResponseHead.hpp"

class Response {

private:
    HttpParser _parser;
    Config _config;
    ResponseHead responseHead;
    ResponseBody responseBody;

public:
    Response(const HttpParser& parser, const Config& config) : _parser(parser), _config(config), responseHead(parser, config), responseBody(parser, config) {}

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

    void init() {
        responseHead.init();
        responseBody.init();
    }

    std::string serialize() {
        std::string body = responseBody.serialize();
        responseHead.setContentLength(std::to_string(body.size()));
        std::string head = responseHead.serialize();
        std::string response = head + body;
        return response;
    }
};