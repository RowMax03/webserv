/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 22:23:38 by mreidenb          #+#    #+#             */
/*   Updated: 2024/07/13 14:46:43 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>

class UploadHandler
{
	private:
		const std::string &_location;
		const std::string &_body;
		std::string _boundary;
		void parseBody();
		void saveFile(const std::string &filename, const std::string &content);
	public:
		UploadHandler(const std::string &location, const std::string &ct_header, const std::string &body);
		~UploadHandler();
};
