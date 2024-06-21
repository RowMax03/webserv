/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 19:54:18 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/21 20:30:26 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

class FileHandler
{
	public:
		FileHandler();
		~FileHandler();
		FileHandler(const FileHandler &src);
		FileHandler &operator=(const FileHandler &src);
		static std::string readFile(const std::string &path);
	private:

};
