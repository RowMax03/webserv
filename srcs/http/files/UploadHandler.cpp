/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:27:52 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 19:06:37 by mreidenb         ###   ########.fr       */
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
	std::string end_boundary = boundary + "--";
	size_t start = _body.find(boundary);
	size_t end;

	while (start != std::string::npos)
	{
		start += boundary.length(); // Move past the current boundary
		end = _body.find(boundary, start); // Find the next boundary

		if (end == std::string::npos) break; // If no more boundaries, exit loop

		std::string part = _body.substr(start, end - start);
		size_t filename_start = part.find("filename=\"") + 10;
		if (filename_start == std::string::npos) continue; // Skip if no filename found

		size_t filename_end = part.find("\"", filename_start);
		if (filename_end == std::string::npos) continue; // Skip if filename end not found

		std::string filename = part.substr(filename_start, filename_end - filename_start);
		size_t content_start = part.find("\r\n\r\n", filename_end) + 4;
		if (content_start == std::string::npos) continue; // Skip if content start not found

		std::string content = part.substr(content_start, part.length() - content_start - 2); // Adjust for trailing \r\n
		saveFile(filename, content);

		start = end; // Move start to the end of the current part for the next iteration
	}
}

void UploadHandler::saveFile(const std::string &filename, const std::string &content)
{
	try{
	std::cout << "Saving file: " << filename << " to: " << _location << std::endl;
	std::ofstream file(_location + "/" + filename);
	if (!file.is_open())
		throw std::runtime_error("500");
	file << content;
	file.close();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}


