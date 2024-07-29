/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 19:57:41 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 17:38:04 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

FileHandler::FileHandler() {}

FileHandler::~FileHandler() {}

FileHandler::FileHandler(const FileHandler &src) {
	*this = src;
}

FileHandler &FileHandler::operator=(const FileHandler &src) {
	(void)src;
	return *this;
}

std::string FileHandler::readFile(const std::string &path) {
	// std::cout << "Reading file: " << path << std::endl;
	if (opendir(path.c_str()))
		throw std::runtime_error("404");
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (file)
	{
		std::ostringstream contents;
		contents << file.rdbuf();
		file.close();
		return contents.str();
	}
	throw std::runtime_error("404");
}
