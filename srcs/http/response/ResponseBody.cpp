#include "ResponseBody.hpp"

ResponseBody::ResponseBody() {
    setBody("");
}

ResponseBody::ResponseBody(const ResponseBody &other) : body(other.body) {}

ResponseBody &ResponseBody::operator=(const ResponseBody &other) {
    if (this != &other) {
        body = other.body;
    }
    return *this;
}

ResponseBody::~ResponseBody() {}

void ResponseBody::init(std::string path) {
    FileHandler fileHandler;
    setBody(fileHandler.readFile(path));
}

std::string ResponseBody::serialize() {
    if (!body.empty())
        return body;
    return "";
}

const std::string &ResponseBody::getBody() const {
    return body;
}

void ResponseBody::setBody(const std::string &body) {
    this->body = body;
}
