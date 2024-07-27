/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:13 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/27 22:24:25 by nscheefe         ###   ########.fr       */
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
        : _config(&config),
        sessionHandler(&sessionHandler),
        errorHandler(responseHead, responseBody, *_config),
        clients(clients),
		Parser(config) {
}
Response::Response(const Response &other) : _config(other._config),
                                            sessionHandler(other.sessionHandler),
											errorHandler(other.errorHandler),
                                            clients(other.clients),
											Parser(other.Parser) {}

 Response &Response::operator=(const Response &other) {
     if (this != &other) {
        Parser = other.Parser;
        _config = other._config;
        responseHead = other.responseHead;
        responseBody = other.responseBody;
     }
     return *this;
}

Response::~Response() {}

void Response::recive(const std::string &request) {
	try {
		Parser.updateRawRequest(request);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e.what());
	}
}

void Response::handleHead(){
	try {
		_location = Parser.getLocation();
		responseHead.setDefault(_location, Parser, _config->server_name, clients);
		if (_location.auth == true)
		{

			if(!sessionHandler->checkSession(Parser.getHeaders()["Cookie"])) //@if header access not set
			{
				responseHead.setStatusCode("401");
				responseHead.setWwwAuthenticate("Basic realm='accress Controll sessin handling from webserv', charset='UTF-8'");
				throw std::runtime_error("401");
			} //@else if header access set
			//else throw 401 with www authenticat
			//generateSession
		}

	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e.what());
	}
}



void Response::handleBody(){
	try {
		if(Parser.getMethod() == "POST")
		{
			handlePost();
		} else if (Parser.getMethod() == "GET") {
			handleGet();
		} else if (Parser.getMethod() == "DELETE") {
			handleDelete();
		} else if (Parser.isCgi) {
			handleCgi();
		}else {
			throw std::runtime_error("405");
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e.what());
	}
}

//##################################################################################################################################

void Response::handlePost(){
	if (Parser.getHeaders()["Content-Type"].find("multipart/form-data") != std::string::npos) {
		std::cout << "POST with file upload to: " << responseHead.location.uploadDir << std::endl;
		UploadHandler uploadHandler(_location.uploadDir , Parser.getHeaders()["Content-Type"], Parser.getBody());
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
//	FileHandler fileHandler();
//	fileHandler.deleteFile(responseHead.fullPathToFile);
//	responseHead.setStatusCode("204");
//	responseHead.setStatusMessage("No Content");
}


void Response::handleCgi(){
	CGI cgi(Parser, responseHead.fullPathToFile);
	responseBody.setBody(cgi.run());
}


std::string Response::serialize() {
	std::string head;
	std::string body;
	if(Parser.getMethod() != "POST" || Parser.getMethod() != "DELETE")
	{
		body = responseBody.serialize();
	}
	if (!Parser.isCgi) {
		std::stringstream ss;
		ss << body.size();
		std::string length = ss.str();
		responseHead.setContentLength(length);
		head = responseHead.serialize(Parser);
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
            oss << "<a href='" << removeTrailingSlash(Parser.getPath()) << "/" << ent->d_name << "'>"
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

