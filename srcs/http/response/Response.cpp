/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:13 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/27 20:30:53 by nscheefe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

	//try catch if error than error handler
	// header shit
		//construct empty error handler with empty head and body
		// cathc errorhandler handle error code function
	//parse request
		//parse request line
		//parse headers
	//getlocation / matchlocation
    //session handling csrf token throws with bad request if false
	//session handling auth throws with 401

	//tr catch
	//update request parser


	//body shit
	//parse body


	//switch on request method
		//hande difrent cases CGI , GET, POST, DELETE
			//set response head and body
			//set mime type

	//error checking ?? throw with response code when error checking methods return true
	//end of funciton next step is to serialize response and pollout


Response::Response(const Config::Server &config, SessionHandler &sessionHandler, long unsigned int clients)
        :_config(&config), sessionHandler(&sessionHandler), clients(clients) {
			ErrorHandler errorHandler(responseHead, responseBody);
}

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

void Response::recive(const std::string &request) {
	try {
		_parser.parse(request);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e);
	}
}

void Response::handleHead(){
	try {
		_location = _parser->getlocation();
		responseHead.setDefault(_location, _parser, _config->getServerName(), clients);
		if (location.auth == true)
		{

			if(!sessionHandler.checkSession(_parser.getHeaders()["Cookie"])) //@if header access not set
			{
				responseHead.setStatusCode("401");
				responseHead.setWwwAuthenticate("Basic realm='accress Controll sessin handling from webserv', charset='UTF-8'");
				throw std::runtime_error("401");
			} //@else if header access set
			//else throw 401 with www authenticat
			//generateSession
		}
		if (errorHandler.isBadRequest() && errorHandler.checkMethod())
			throw std::runtime_error("403");
		if (!errorHandler.checkPath())
			errorHandler.checkMethod();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e);
	}
}



void Response::handleBody(){
	try {
		if(_parser.getMethod() == "POST")
		{
			handlePost();
		} else if (_parser.getMethod() == "GET") {
			handleGet();
		} else if (_parser.getMethod() == "DELETE") {
			handleDelete();
		} else if (_parser.isCgi) {
			handleCgi();
		}else {
			throw std::runtime_error("405");
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e);
	}
}

//##################################################################################################################################

void Response::handlePost(){
	if (_parser.getHeaders()["Content-Type"].find("multipart/form-data") != std::string::npos) {
		std::cout << "POST with file upload to: " << responseHead.location.uploadDir << std::endl;
		UploadHandler uploadHandler(_locations.uploadDir , _parser.getHeaders()["Content-Type"], _parser.getBody());
	}
}

void Response::handleGet(){
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

void Response::handleDelete(){
	FileHandler fileHandler(responseHead.fullPathToFile);
	fileHandler.deleteFile();
	responseHead.setStatusCode("204");
	responseHead.setStatusMessage("No Content");
}


void Response::handleCgi(){
	CGI cgi(_parser, responseHead.fullPathToFile);
	responseBody.setBody(cgi.run());
}


std::string Response::serialize() {
	std::string head;
	std::string body;
	if(_parser.getMethod() != "POST" || _parser.getMethod() != "DELETE")
	{
		body = responseBody.serialize();
	}
	if (!_parser.isCgi) {
		std::stringstream ss;
		ss << body.size();
		std::string length = ss.str();
		responseHead.setContentLength(length);
		head = responseHead.serialize();
	}
	std::string response = head + body;
	return response;
}


//##################################################################################################################################

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
		throw std::runtime_error("404");
        oss << "Could not open directory";
    }
    std::string result = oss.str();
    return result;
}


void Response::setMimeType(const std::string& filePath) {
	std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

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
    mimeTypes["json"] = "application/json";
    mimeTypes["xml"] = "application/xml";
    mimeTypes["csv"] = "text/csv";
    mimeTypes["mp3"] = "audio/mpeg";
    mimeTypes["wav"] = "audio/wav";
    mimeTypes["mp4"] = "video/mp4";
    mimeTypes["avi"] = "video/x-msvideo";
    mimeTypes["mov"] = "video/quicktime";
    mimeTypes["doc"] = "application/msword";
    mimeTypes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mimeTypes["ppt"] = "application/vnd.ms-powerpoint";
    mimeTypes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mimeTypes["xls"] = "application/vnd.ms-excel";
    mimeTypes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mimeTypes["ico"] = "image/x-icon";
    mimeTypes["webp"] = "image/webp";
    mimeTypes["otf"] = "font/otf";
    mimeTypes["ttf"] = "font/ttf";
    mimeTypes["woff"] = "font/woff";
    mimeTypes["woff2"] = "font/woff2";
    mimeTypes["eot"] = "application/vnd.ms-fontobject";
    mimeTypes["sfnt"] = "font/sfnt";


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

