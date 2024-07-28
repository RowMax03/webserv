/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:27:52 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/28 20:57:55 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UploadHandler.hpp"
#include <iostream>

UploadHandler::UploadHandler(const std::string &location, const std::string &ct_header, const std::string &body) : _location(location), _body(body)
{
	_boundary = ct_header.substr(ct_header.find("boundary=") + 9);
	parseBody();
}

UploadHandler::~UploadHandler()
{
}

void UploadHandler::parseBody()
{
	std::string boundary = "--" + _boundary;
	std::string end_boundary = "--" + _boundary + "--";
	size_t start = _body.find(boundary);
	size_t end = _body.find(end_boundary);
	// std::cout << "boundary: " << boundary << std::endl;
	// std::cout << "end_boundary: " << end_boundary << std::endl;
	
	while (start != std::string::npos && end != std::string::npos)
	{
		std::string part = _body.substr(start + boundary.length(), end - start - boundary.length());
		size_t filename_start = part.find("filename=\"") + 10;
		size_t filename_end = part.find("\"", filename_start);
		std::string filename = part.substr(filename_start, filename_end - filename_start);
		size_t content_start = part.find("\r\n\r\n") + 4;
		std::string content = part.substr(content_start);
		saveFile(filename, content);
		start = _body.find(boundary, end);
		end = _body.find(end_boundary, start);
	}
}

void UploadHandler::saveFile(const std::string &filename, const std::string &content)
{
	std::cout << "Saving file: " << filename << " to: " << _location << std::endl;
	std::ofstream file(_location + "/" + filename);
	if (!file.is_open())
		throw std::runtime_error("500");
	file << content;
	file.close();
}


