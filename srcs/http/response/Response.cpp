/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:13 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/15 17:58:36 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"


Response::Response(const HttpParser &parser, const Config::Server &config, std::string path, long unsigned int clients)
        : _parser(parser), _config(&config), responseHead(parser, config, path, clients),
          responseBody(parser, config) {}

Response::Response(const Response &other) : _parser(other._parser), _config(other._config),
                                            responseHead(other.responseHead),
                                            responseBody(other.responseBody) {}

Response &Response::operator=(const Response &other) {
    if (this != &other) {
        _parser = other._parser;
        _config = other._config;
        responseHead = other.responseHead;
        responseBody = other.responseBody;
    }
    return *this;
}

Response::~Response() {}

std::string Response::removeTrailingSlash(const std::string &input) {
    if (!input.empty() && input[input.length() - 1] == '/') {
        return input.substr(0, input.length() - 1);
    }
    return input;
}

std::string Response::generateDirectoryListing(const std::string &path, DIR *dir) {
    struct dirent *ent;
    std::ostringstream oss;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            oss << "<a href='" << removeTrailingSlash(_parser.getPath()) << "/" << ent->d_name << "'>"
                << ent->d_name << "</a>" << "<br>";
        }
        closedir(dir);
    } else {
        oss << "Could not open directory";
    }
    std::string result = oss.str();
    return result;
}

void Response::init() {
    responseHead.init();
    ErrorHandler errorHandler(_parser, responseHead, responseBody, *_config);
	if (_parser.getMethod() == "POST" && _parser.getHeaders()["Content-Type"].find("multipart/form-data") != std::string::npos) {
		try {
		std::cout << "POST with file upload to: " << responseHead.location.uploadDir << std::endl;
		UploadHandler uploadHandler(responseHead.location.uploadDir , _parser.getHeaders()["Content-Type"], _parser.getBody());
		}
		catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			errorHandler.handleErrorCode("500");
		}
	}
    if (errorHandler.isBadRequest() && errorHandler.checkMethod())
        errorHandler.handleErrorCode("403");
    if (!errorHandler.checkPath())
        errorHandler.checkMethod();
    if (_parser.isCgi) {
        // CGI
        try {
            CGI cgi(_parser, responseHead.location.root);
            responseBody.setBody(cgi.run());
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::string error = e.what();
            // Error handling, replace with error handler later
            if (error == "file not found")
                errorHandler.handleErrorCode("404");
            else if (error == "file not executable")
                errorHandler.handleErrorCode("403");
            else
                errorHandler.handleErrorCode("500");
        }
    } else {
        DIR *dir = opendir(responseHead.fullPathToFile.c_str());
        if (responseHead.location.autoindex && dir != NULL) {
            std::string directoryListing = generateDirectoryListing(responseHead.fullPathToFile, dir);
            responseBody.setBody(directoryListing);
            responseHead.setContentLength(intToString(directoryListing.size()));
            responseHead.setStatusCode("201");
            responseHead.setStatusMessage("Created");
            closedir(dir);
        } else if (responseBody.getBody().empty()) {
			setMimeType(responseHead.fullPathToFile);
            responseBody.init(responseHead.fullPathToFile);
        }
    }
}

void Response::setMimeType(const std::string& filePath) {
	std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	// Example MIME types, extend this map according to your needs
    std::map<std::string, std::string> mimeTypes;
    mimeTypes["html"] = "text/html";
    mimeTypes["txt"] = "text/plain";
    mimeTypes["js"] = "application/javascript";
	mimeTypes["svg"] = "image/svg+xml";
    mimeTypes["css"] = "text/css";
    mimeTypes["jpg"] = "image/jpeg";
    mimeTypes["png"] = "image/png";
    mimeTypes["gif"] = "image/gif";
    mimeTypes["pdf"] = "application/pdf";

    std::map<std::string, std::string>::iterator it = mimeTypes.find(extension);
    if (it != mimeTypes.end()) {
        responseHead.setContentType(it->second);
    }
	else
    	responseHead.setContentType("application/octet-stream");// Default MIME type for unknown/other files
}

std::string Response::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string Response::serialize() {
    std::string body = responseBody.serialize();
    std::stringstream ss;
    ss << body.size();
    std::string length = ss.str();
    responseHead.setContentLength(length);
    std::string head = responseHead.serialize();
    std::string response = head + body;
    return response;
}
