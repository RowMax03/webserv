/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:35:13 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/29 22:08:57 by mreidenb         ###   ########.fr       */
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

Response::~Response() {
	// std::cout << "Response Destructor" << std::endl;
}

void Response::recive(const std::string &request) {
	try {
		Parser.updateRawRequest(request);
	}
	catch (const std::exception &e) {
		std::cerr << "Error in recive: " << e.what() << std::endl;
		errorHandler.handleErrorCode(e.what());
	}
}

void Response::handleHead(){
	responseBody.setBody("");
	try {
		_location = Parser.getLocation();
		responseHead.setDefault(_location, Parser, _config->server_name, clients);
		if (_location.auth == true)
		{

			if(!sessionHandler->checkSession(Parser.getHeaders()["Cookie"]))
			{
				throw std::runtime_error("401");
			}
		}

	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		errorHandler.handleErrorCode(e.what());
	}
}



void Response::handleBody(){
	try {
		std::cout << "Handle Body, Method: " << Parser.getMethod() << std::endl;
		if (Parser.isCgi) {
			handleCgi();
		} else if(Parser.getMethod() == "POST") {
			handlePost();
		} else if (Parser.getMethod() == "GET") {
			handleGet();
		} else if (Parser.getMethod() == "DELETE") {
			handleDelete();
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
	}else if (Parser.getHeaders()["Content-Type"].find("application/x-www-form-urlencoded") != std::string::npos) {
    		std::string sessionCookie = Parser.getHeaders()["Cookie"];
			bool isSessionValid = sessionHandler->checkSession(sessionCookie);
            std::map<std::string, std::string> body = sessionHandler->parseBody(Parser.getBody());
			if(body["form"] == "login")
			{
    			if (!isSessionValid) {
            	std::string username = body["name"];
            	std::string password = body["password"];
				std::cout << "Username: " << username << std::endl;
				std::cout << "Password: " << password << std::endl;
            	Login login(username, password);
            	if (sessionHandler->validateCredentials(login.Base64Login)) {
                	responseHead.setCookie(sessionHandler->generateSession(login.Base64Login));
					responseHead.setStatusCode("201");
					responseHead.setStatusMessage("Created");
				} else {
            	    throw std::runtime_error("401");
            	}
				}
			}else if(body["form"] == "logout") {
				std::cout << "Logout" << std::endl;
				if(isSessionValid)
				{
									std::cout << "Logout2" << std::endl;

					responseHead.setCookie("session_id=; Secure; HttpOnly; SameSite=Strict; Max-Age=0");
					sessionHandler->deleteSession(sessionCookie);
				}else{
					throw std::runtime_error("401");
				}
			}
	}
}

void Response::handleGet(){
	DIR *dir = opendir(responseHead.fullPathToFile.c_str());
	if (responseHead.location.autoindex && dir != NULL && responseBody.getBody().empty()) {
		std::string directoryListing = generateDirectoryListing(responseHead.fullPathToFile, dir);
		responseBody.setBody(directoryListing);
		responseHead.setContentLength(intToString(directoryListing.size()));

		closedir(dir);
	} else if (responseBody.getBody().empty()) {
		setMimeType(responseHead.fullPathToFile);
		responseBody.init(responseHead.fullPathToFile);
	}
}

void Response::handleDelete(){
	FileHandler fileHandler;
	fileHandler.deleteFile(responseHead.fullPathToFile);
	responseHead.setStatusCode("204");
	responseHead.setStatusMessage("No Content");
}


void Response::handleCgi(){
	CGI cgi(Parser, responseHead.fullPathToFile);
	responseBody.setBody(cgi.run());
}


std::string Response::serialize() {
	std::string head;
	std::string body;
	if (responseHead.getStatusCode() == "")
	{
		handleHead();
	}

	if(Parser.getMethod() == "GET" || Parser.getMethod() == "DELETE" || Parser.getMethod() == "POST")
	{
		handleBody();
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
		oss << "<script>"
			<< "function sendDeleteRequest(filePath) {"
			<< "  fetch(filePath, { method: 'DELETE' })"
			<< "    .then(response => {"
			<< "      if (response.ok) {"
			<< "        window.location.reload();"
			<< "      } else {"
			<< "        alert('Error: ' + response.statusText);"
			<< "      }"
			<< "    });"
			<< "}"
			<< "</script>";
		while ((ent = readdir(dir)) != NULL) {
			// Skip the current and parent directory entries
			std::string modPath = path;
			if (modPath.find(_location.root) == 0)
            	modPath = _location.path + modPath.erase(0, _location.root.length());
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
				oss << "<a href='" << modPath + '/' + ent->d_name << "'>" << ent->d_name << "</a><br>";
				continue;
			}
			// Generate the link for the directory entry
			oss << "<a href='" << removeTrailingSlash(modPath) << "/" << ent->d_name << "'>"
				<< ent->d_name << "</a>";
			if (std::find(_location.methods.begin(), _location.methods.end(), "DELETE") == _location.methods.end())
			{
				oss << "<br>";
				continue;
			}
			// Add a button for the directory entry that calls sendDeleteRequest when clicked
			oss << "<button onclick=\"sendDeleteRequest('" << removeTrailingSlash(modPath) << "/" << ent->d_name << "')\">Delete</button>"
				<< "<br>";
		}
		closedir(dir);
	} else {
		// Directory could not be opened; throw an exception
		throw std::runtime_error("404: Directory not found");
	}
	return oss.str();
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
	else if (extension != "")
    	responseHead.setContentType("application/octet-stream");// Default MIME type for unknown/other files
	else
		responseHead.setContentType("text/plain");
}

std::string Response::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

