#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"

class ResponseHead {
private:
    HttpParser _parser;
    const Config::Server* _config;
    std::string _header;
    std::string _statusCode;
    std::string _statusMessage;
    std::string _connectionType;
    std::string _contentType;
    std::string _contentLength;
    std::string _allow;
    std::string _contentLanguage;
    std::string _contentLocation;
    std::string _lastModified;
    std::string _location;
    std::string _retryAfter;
    std::string _transferEncoding;
    std::string _wwwAuthenticate;

public:
    Config::Location location;
    std::string fullPathToFile;
    std::string location_path;
    long unsigned int numCLients;

    ResponseHead(const HttpParser& _parser, const Config::Server &conf, std::string location_path, int numClients) : _parser(_parser), _config(&conf), location_path(location_path), numCLients(numClients) {
        setStatusCode("");
        setStatusMessage("");
        setAllow("");
        setConnectionType("");
        setContentLanguage("");
        setContentLength("");
        setContentLocation("");
        setContentType("");
        setLastModified("");
        setRetryAfter("");
        setTransferEncoding("");
        setWwwAuthenticate("");

    }

    ResponseHead(const ResponseHead& other) : _parser(other._parser), _config(other._config), _header(other._header) {}

    ResponseHead& operator=(const ResponseHead& other) {
        if (this != &other) {
            _parser = other._parser;
            _config = other._config;
            _header = other._header;
        }
        return *this;
    }

    ~ResponseHead() {}

    void init() {
        setStatusCode("200");
        setStatusMessage("OK");

        std::map <std::string, std::string> headers = _parser.getHeaders();
        checkLocation();
        checkRedirect();
        setConnectionType("keep-alive");
        setContentType(headers["Accept"].substr(0, headers["Accept"].find(",")));
        setContentLength("0");
        setAllow(join(location.methods, ", "));

        setContentLanguage("");
        setContentLocation((_parser.getPath() == location_path ?  location.index  : _parser.getPath()));
        setLastModified(formatLastModifiedTime(fullPathToFile));
        setRetryAfter(calculateRetryAfter());
        setTransferEncoding("");
        setWwwAuthenticate("");
    }

    std::string serialize() {
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
        if (!getWwwAuthenticate().empty())
            oss << "WWW-Authenticate: " << getWwwAuthenticate() << "\r\n";
        oss << "\r\n";

        return oss.str();
    }


    //utils

    float calculateServerLoad() {
        int activeConnections = numCLients;
        int maxConnections = 100;
        float load = static_cast<float>(activeConnections) / maxConnections;
        return load;
    }

    std::string calculateRetryAfter() {
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

    std::string formatLastModifiedTime(const std::string& filePath) {
        struct stat fileInfo;
        if (stat(filePath.c_str(), &fileInfo) != 0) {
            return "";
        }

        char dateStr[80];
        std::tm* ptm = std::localtime(&fileInfo.st_mtime);
        std::strftime(dateStr, sizeof(dateStr), "%a, %d %b %Y %H:%M:%S GMT", ptm);

        return std::string(dateStr);
    }

    std::string getCurrentDate() {
        std::time_t now = std::time(NULL);
        std::tm* timeinfo = std::gmtime(&now);

        char buffer[1000];
        std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

        return std::string(buffer);
    }

    void filecheck(std::string fullPath, std::map<std::string, Config::Location>::const_iterator it, std::string path){
        std::ifstream file(fullPath.c_str());
        if (file.good()) {
            location = it->second;
            fullPathToFile = fullPath;
            setLocation(path);
            file.close();
        } else
            return;
    }

    void checkLocation() {
        std::map<std::string, Config::Location>::const_iterator it = _config->locations.find(location_path);
        if (it != _config->locations.end()) {
            std::string fullPath = it->second.root + (_parser.getPath() == location_path ?   it->second.index  : _parser.getPath());
            std::cout << "fullpath :"<<fullPath << std::endl;
            filecheck(fullPath, it, _parser.getPath());
        }
    }

    void checkRedirect(){
        if (!location.redirect_url.empty())
        {
            setStatusCode(std::to_string(location.redirect_status));
            setLocation(location.redirect_url);
        }
    }

    std::string join(const std::vector<std::string>& vec, const char* delim)
    {
        std::ostringstream os;
        for(std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            if(it != vec.begin())
                os << delim;
            os << *it;
        }
        return os.str();
    }



    //getter / setter

    std::string getHeader() const { return _header; }
    void setHeader(const std::string& header) { _header = header; }

    std::string getStatusCode() const { return _statusCode; }
    void setStatusCode(const std::string& statusCode) { _statusCode = statusCode; }

    std::string getStatusMessage() const { return _statusMessage; }
    void setStatusMessage(const std::string& statusMessage) { _statusMessage = statusMessage; }

    std::string getConnectionType() const { return _connectionType; }
    void setConnectionType(const std::string& connectionType) { _connectionType = connectionType; }

    std::string getContentType() const { return _contentType; }
    void setContentType(const std::string& contentType) { _contentType = contentType; }

    std::string getContentLength() const { return _contentLength; }
    void setContentLength(const std::string& contentLength) { _contentLength = contentLength; }

    std::string getAllow() const { return _allow; }
    void setAllow(const std::string& allow) { _allow = allow; }

    std::string getContentLanguage() const { return _contentLanguage; }
    void setContentLanguage(const std::string& contentLanguage) { _contentLanguage = contentLanguage; }

    std::string getContentLocation() const { return _contentLocation; }
    void setContentLocation(const std::string& contentLocation) { _contentLocation = contentLocation; }

    std::string getLastModified() const { return _lastModified; }
    void setLastModified(const std::string& lastModified) { _lastModified = lastModified; }

    std::string getLocation() const { return _location; }
    void setLocation(const std::string& location) { _location = location; }

    std::string getRetryAfter() const { return _retryAfter; }
    void setRetryAfter(const std::string& retryAfter) { _retryAfter = retryAfter; }

    std::string getTransferEncoding() const { return _transferEncoding; }
    void setTransferEncoding(const std::string& transferEncoding) { _transferEncoding = transferEncoding; }

    std::string getWwwAuthenticate() const { return _wwwAuthenticate; }
    void setWwwAuthenticate(const std::string& wwwAuthenticate) { _wwwAuthenticate = wwwAuthenticate; }
};