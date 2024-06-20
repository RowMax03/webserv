#include <string>

class ResponseBody {
private:
    HttpParser _parser;
    Config _config;
    std::string body;

public:
    ResponseBody(const HttpParser& parser, const Config& config) : _parser(parser), _config(config) {}

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

    void init() {
        setBody("");
    }

    std::string serialize() {
        return body;
    }

    const std::string& getBody() const {
        return body;
    }

    void setBody(const std::string& body) {
        this->body = body;
    }
};