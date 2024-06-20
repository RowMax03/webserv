#include <ctime>
#include <sstream>

class ResponseHead {
private:
    HttpParser _parser;
    config _config;
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
    ResponseHead(const Http_parser& _parser, const _config& conf) : _parser(_parser), _config(conf) {
        init();
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
        setConnectionType("keep-alive");
        setContentType("text/html");
        setContentLength("0");
        setAllow("");
        setContentLanguage("");
        setContentLocation("");
        setLastModified("");
        setLocation("");
        setRetryAfter("");
        setTransferEncoding("");
        setWwwAuthenticate("");
    }

    std::string serialize() {
        std::ostringstream oss;

        if (!getParser().getVersion().empty() && !getStatusCode().empty() && !getStatusMessage().empty())
            oss << getParser().getVersion() << " " << getStatusCode() << " " << getStatusMessage() << "\r\n";
        if (!getConfig().getServerName().empty())
            oss << "Server: " << getConfig().getServerName() << "\r\n";
        if (!getCurrentDate().empty())
            oss << "Date: " << getCurrentDate() << "\r\n";
        if (!getConnectionType().empty())
            oss << "Connection: " << getConnectionType() << "\r\n";
        if (getParser().getMethod() == "POST" || getParser().getMethod() == "PUT") {
            if (!getContentType().empty())
                oss << "Content-Type: " << getContentType() << "\r\n";
            if (!getContentLength().empty())
                oss << "Content-Length: " << getContentLength() << "\r\n";
        }
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

    std::string getCurrentDate() {
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::gmtime(&now);

        char buffer[1000];
        std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

        return std::string(buffer);
    }

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