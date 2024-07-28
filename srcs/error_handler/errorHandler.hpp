/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:36:02 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/28 21:37:14 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../http/response/ResponseHead.hpp"
#include "../http/response/ResponseBody.hpp"
#include "../http/parser/HttpParser.hpp"
class ErrorHandler {
private:
    ResponseHead& _responseHead;
    ResponseBody& _responseBody;
    const Config::Server* _config;

public:
    ErrorHandler(ResponseHead &responseHead, ResponseBody &responseBody, const Config::Server &config)
            :_responseHead(responseHead), _responseBody(responseBody), _config(&config) {}

    ErrorHandler(const ErrorHandler& other)
            : _responseHead(other._responseHead), _responseBody(other._responseBody), _config(other._config)  {}

    // ErrorHandler& operator=(const ErrorHandler& other) {
    //     // if (this != &other) {
    //     //     _parser = other._parser;
    //     //     _responseHead = other._responseHead;
    //     //     _responseBody = other._responseBody;
    //     //     _config = other._config;
    //     // }
	// 	*this = other;
    //     // return *this;
    // }

    ~ErrorHandler() {}


    void handleErrorCode(const std::string& statusCode) {
        if (statusCode == "400") {
            _responseHead.setStatusCode("400");
            _responseHead.setStatusMessage("Bad Request");
            setErrorPage();
        } else if (statusCode == "401") {
            _responseHead.setStatusCode("401");
            _responseHead.setStatusMessage("Unauthorized");
            setErrorPage();
        } else if (statusCode == "403") {
            _responseHead.setStatusCode("403");
            _responseHead.setStatusMessage("Forbidden");
            setErrorPage();
        } else if (statusCode == "404") {
            _responseHead.setStatusCode("404");
            _responseHead.setStatusMessage("Not Found");
            setErrorPage();
        } else if (statusCode == "405") {
            _responseHead.setStatusCode("405");
            _responseHead.setStatusMessage("Method Not Allowed");
            setErrorPage();
        } else if (statusCode == "500") {
            _responseHead.setStatusCode("500");
            _responseHead.setStatusMessage("Internal Server Error");
            setErrorPage();
        } else {
            _responseHead.setStatusCode("418");
            _responseHead.setStatusMessage("im a tea pot");
			setErrorPage();

        }
    }


    bool checkPath(){
        if(_responseHead.fullPathToFile.empty())
        {
            handleErrorCode("404");
            return true;

        }
        return false;
    }

    void setErrorPage(){
        std::map<int, Config::ErrorPage>::const_iterator errorPage = _config->error_pages.find(atoi(_responseHead.getStatusCode().c_str()));
        if (errorPage != _config->error_pages.end())
        {

            std::ifstream file(errorPage->second.url.c_str());
            if (file.good()) {
                _responseBody.init(errorPage->second.url);
                file.close();
                return;
            }
        }
        std::ostringstream oss;
        oss << "Error: " << _responseHead.getStatusCode() << " " << _responseHead.getStatusMessage();
        _responseBody.setBody(oss.str());
    }

};

