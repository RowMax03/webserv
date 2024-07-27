/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHead.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:36 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/23 20:21:39 by nscheefe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ResponseHead.hpp"

ResponseHead::ResponseHead(const HttpParser &_parser, const Config::Server &conf, std::string location_path,
                           int numClients) : _parser(_parser), _config(&conf), location_path(location_path),
                                             numClients(numClients) {
    setStatusCode("200");
    setStatusMessage("OK");

    std::map <std::string, std::string> headers = _parser.getHeaders();
    checkLocation();
    setConnectionType("keep-alive");
    setContentType(headers["Accept"].substr(0, headers["Accept"].find(",")));
    setContentLength("0");
    setAllow(join(location.methods, ", "));
    setContentLanguage("");
    setContentLocation((_parser.getPath() == location_path ? location.index : _parser.getPath()));
    setLastModified(formatLastModifiedTime(fullPathToFile));
    setRetryAfter(calculateRetryAfter());
    setTransferEncoding("");
    //@todo ( whenn www-authenticate returns base64 'user:password')
    if(location.auth === true ) { //@todo add controlling for Sessio Cookie thats already exists
        setStatusCode("401")
        setWwwAuthenticate("Basic realm='accress Controll sessin handling from webserv', charset='UTF-8'");
    }
    checkRedirect();

}

ResponseHead::ResponseHead(const ResponseHead &other) : _parser(other._parser), _config(other._config),
                                                        _header(other._header) {}

ResponseHead &ResponseHead::operator=(const ResponseHead &other) {
    if (this != &other) {
        _parser = other._parser;
        _config = other._config;
        _header = other._header;
    }
    return *this;
}

ResponseHead::~ResponseHead() {}

std::string ResponseHead::serialize() {
    std::ostringstream oss;
    if (!_parser.getVersion().empty() && !getStatusCode().empty() && !getStatusMessage().empty())
        oss << _parser.getVersion() << " " << getStatusCode() << " " << getStatusMessage() << "\r\n";
    if (!_config->server_name.empty())
        oss << "Server: " << _config->server_name << "\r\n";
    if (!getCurrentDate().empty())
        oss << "Date: " << getCurrentDate() << "\r\n";
    if (!getConnectionType().empty())
        oss << "Connection: " << getConnectionType() << "\r\n";
    if (!getContentType().empty())
        oss << "Content-Type: " << getContentType() << "\r\n";
    if (!getContentLength().empty())
        oss << "Content-Length: " << getContentLength() << "\r\n";
    if (!getAllow().empty())
        oss << "Allow: " << getAllow() << "\r\n";
    if (!getContentLanguage().empty())
        oss << "Content-Language: " << getContentLanguage() << "\r\n";
    if (!getContentLocation().empty())
        oss << "Content-Location: " << getContentLocation() << "\r\n";
    if (!getLastModified().empty())
        oss << "Last-Modified: " << getLastModified() << "\r\n";
    if (!getLocation().empty())
        oss << "Location: " << getLocation() << "\r\n";
    if (!getRetryAfter().empty())
        oss << "Retry-After: " << getRetryAfter() << "\r\n";
    if (!getTransferEncoding().empty())
        oss << "Transfer-Encoding: " << getTransferEncoding() << "\r\n";
    if (!getWwwAuthenticate().empty()) {
        oss << "WWW-Authenticate: " << getWwwAuthenticate() << "\r\n";
    }
    if (!getCookie().empty()){
		oss << "Set-Cookie: " << getCookie() << "\r\n";
	}
    oss << "\r\n";

    return oss.str();
}


//utils

float ResponseHead::calculateServerLoad() {
    int activeConnections = numClients;
    int maxConnections = 100;
    float load = static_cast<float>(activeConnections) / maxConnections;
    return load;
}

std::string ResponseHead::calculateRetryAfter() {
    float load = calculateServerLoad();
    int delay;

    if (load > 0.75) {
        delay = 120;
    } else if (load > 0.5) {
        delay = 60;
    } else {
        delay = 30;
    }
    std::ostringstream oss;
    oss << delay;
    return oss.str();
}

std::string ResponseHead::formatLastModifiedTime(const std::string &filePath) {
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0) {
        return "";
    }

    char dateStr[80];
    std::tm *ptm = std::localtime(&fileInfo.st_mtime);
    std::strftime(dateStr, sizeof(dateStr), "%a, %d %b %Y %H:%M:%S GMT", ptm);

    return std::string(dateStr);
}

std::string ResponseHead::getCurrentDate() {
    std::time_t now = std::time(NULL);
    std::tm *timeinfo = std::gmtime(&now);

    char buffer[1000];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

    return std::string(buffer);
}

std::string ResponseHead::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}


void ResponseHead::filecheck(std::string fullPath, std::map<std::string, Config::Location>::const_iterator it,
                             std::string path) {
    std::ifstream file(fullPath.c_str());
    if (file.good()) {
        location = it->second;
        fullPathToFile = fullPath;
        setLocation(path);
        file.close();
    } else
        return;
}

void ResponseHead::checkLocation() {
    std::map<std::string, Config::Location>::const_iterator it = _config->locations.find(location_path);
    if (it != _config->locations.end()) {
        std::string modPath = _parser.getPath();
        if (modPath.find(location_path) == 0)
            modPath.erase(0, location_path.length());
        std::string fullPath = it->second.root + (_parser.getPath() == location_path ? it->second.index : modPath);
        std::cout << "fullpath :" << fullPath << std::endl;
        filecheck(fullPath, it, _parser.getPath());
    }
}

void ResponseHead::checkRedirect() {
    if (!location.redirect_url.empty()) {
        setStatusCode(intToString(location.redirect_status));
        setLocation(location.redirect_url);
    }
}

std::string ResponseHead::join(const std::vector <std::string> &vec, const char *delim) {
    std::ostringstream os;
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (it != vec.begin())
            os << delim;
        os << *it;
    }
    return os.str();
}



//getter / setter

std::string ResponseHead::getHeader() const { return _header; }

void ResponseHead::setHeader(const std::string &header) { _header = header; }

std::string ResponseHead::getStatusCode() const { return _statusCode; }

void ResponseHead::setStatusCode(const std::string &statusCode) { _statusCode = statusCode; }

std::string ResponseHead::getStatusMessage() const { return _statusMessage; }

void ResponseHead::setStatusMessage(const std::string &statusMessage) { _statusMessage = statusMessage; }

std::string ResponseHead::getConnectionType() const { return _connectionType; }

void ResponseHead::setConnectionType(const std::string &connectionType) { _connectionType = connectionType; }

std::string ResponseHead::getContentType() const { return _contentType; }

void ResponseHead::setContentType(const std::string &contentType) { _contentType = contentType; }

std::string ResponseHead::getContentLength() const { return _contentLength; }

void ResponseHead::setContentLength(const std::string &contentLength) { _contentLength = contentLength; }

std::string ResponseHead::getAllow() const { return _allow; }

void ResponseHead::setAllow(const std::string &allow) { _allow = allow; }

std::string ResponseHead::getContentLanguage() const { return _contentLanguage; }

void ResponseHead::setContentLanguage(const std::string &contentLanguage) { _contentLanguage = contentLanguage; }

std::string ResponseHead::getContentLocation() const { return _contentLocation; }

void ResponseHead::setContentLocation(const std::string &contentLocation) { _contentLocation = contentLocation; }

std::string ResponseHead::getLastModified() const { return _lastModified; }

void ResponseHead::setLastModified(const std::string &lastModified) { _lastModified = lastModified; }

std::string ResponseHead::getLocation() const { return _location; }

void ResponseHead::setLocation(const std::string &location) { _location = location; }

std::string ResponseHead::getRetryAfter() const { return _retryAfter; }

void ResponseHead::setRetryAfter(const std::string &retryAfter) { _retryAfter = retryAfter; }

std::string ResponseHead::getTransferEncoding() const { return _transferEncoding; }

void ResponseHead::setTransferEncoding(const std::string &transferEncoding) { _transferEncoding = transferEncoding; }

std::string ResponseHead::getWwwAuthenticate() const { return _wwwAuthenticate; }

void ResponseHead::setWwwAuthenticate(const std::string &wwwAuthenticate) { _wwwAuthenticate = wwwAuthenticate; }

void ResponseHead::setCookie(const std::string &cookie) {
	_cookie = cookie;
}

std::string ResponseHead::getCookie() {
	return _cookie;
}

