/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 19:54:18 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/29 19:41:52 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>

class FileHandler
{
	public:
		FileHandler();
		~FileHandler();
		FileHandler(const FileHandler &src);
		FileHandler &operator=(const FileHandler &src);
		static std::string readFile(const std::string &path);
		static void deleteFile(const std::string &path);
	private:

};
