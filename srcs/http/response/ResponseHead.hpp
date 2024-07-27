/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHead.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:40 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/27 22:49:32 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#pragma once
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "../../conf_parser/ServerConfig.hpp"
#include "../parser/HttpParser.hpp"
#include "fstream"

class ResponseHead {
private:
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
	std::string _cookie;

public:
std::string ServerName;
    Config::Location location;
    std::string fullPathToFile;
    std::string location_path;
    long unsigned int numClients;

    ResponseHead();


    ~ResponseHead();

    std::string serialize(HttpParser &parser);
	void setDefault(Config::Location location, HttpParser &parser, std::string ServerName, int numClients);

    //utils

    float calculateServerLoad(int activeConnections);

    std::string calculateRetryAfter(int numClients);

    std::string formatLastModifiedTime(const std::string &filePath);

    std::string getCurrentDate();

    std::string intToString(int value);


    void filecheck(std::string fullPath,
                             std::string path);

    void checkLocation(Config::Location location, HttpParser &_parser);

    void checkRedirect();

    std::string join(const std::vector <std::string> &vec, const char *delim);

    //getter / setter

    std::string getHeader() const;

    void setHeader(const std::string &header);

    std::string getStatusCode() const;

    void setStatusCode(const std::string &statusCode);

    std::string getStatusMessage() const;

    void setStatusMessage(const std::string &statusMessage);

    std::string getConnectionType() const;

    void setConnectionType(const std::string &connectionType);

    std::string getContentType() const;

    void setContentType(const std::string &contentType);

    std::string getContentLength() const;

    void setContentLength(const std::string &contentLength);

    std::string getAllow() const;

    void setAllow(const std::string &allow);

    std::string getContentLanguage() const;

    void setContentLanguage(const std::string &contentLanguage);

    std::string getContentLocation() const;

    void setContentLocation(const std::string &contentLocation);

    std::string getLastModified() const;

    void setLastModified(const std::string &lastModified);

    std::string getLocation() const;

    void setLocation(const std::string &location);

    std::string getRetryAfter() const;

    void setRetryAfter(const std::string &retryAfter);

    std::string getTransferEncoding() const;

    void setTransferEncoding(const std::string &transferEncoding);

    std::string getWwwAuthenticate() const;

    void setWwwAuthenticate(const std::string &wwwAuthenticate);

	void setCookie(const std::string &cookie);
	std::string getCookie() const;

};
