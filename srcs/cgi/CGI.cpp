/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:00:40 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/05 18:25:30 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(const HttpParser &request) : _request(request) {
	toCharArr(request.toCgiEnv());
}

CGI::~CGI() {
	deleteEnv();
}

void CGI::toCharArr(const std::vector<std::string> &envp) {
	_env = new char*[envp.size() + 1];
	for (size_t i = 0; i < envp.size(); i++) {
		_env[i] = new char[envp[i].size() + 1];
		std::strcpy(_env[i], envp[i].c_str());
	}
	_env[envp.size()] = NULL;
}

void CGI::deleteEnv() {
	for (size_t i = 0; _env[i]; i++) {
		delete[] _env[i];
	}
	delete[] _env;
}

std::string CGI::run() {
	int inputPipe[2];
	int outputPipe[2];

	//document root, get from config later
	std::string documentRoot = "/var/www/html";

	if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
		throw std::runtime_error("pipe failed");
	}

	pid_t pid = fork();
	if (pid == -1) {
		throw std::runtime_error("fork failed");
	}
	if (pid == 0) {
		close(inputPipe[1]); // close write end of input pipe
		close(outputPipe[0]); // close read end of output pipe
		dup2(inputPipe[0], STDIN_FILENO);
		dup2(outputPipe[1], STDOUT_FILENO);
		std::string path = documentRoot + _request.getPath();
		char* argv[] = { const_cast<char*>(path.c_str()), NULL };
		execve(argv[0], argv, _env); // execve never returns if successful
		std::string err = "execve failed";
		write(outputPipe[1], err.c_str(), err.size());
	}
	else {
		std::string cgi_response;
		close(inputPipe[0]); // close read end of input pipe
		close(outputPipe[1]); // close write end of output pipe
		write(inputPipe[1], _request.getBody().c_str(), _request.getBody().size());
		close(inputPipe[1]);
		char buffer[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0)
		{
			if (bytesRead == 14 && strncmp(buffer, "execve failed\n", 14) == 0)
			{
				close(outputPipe[0]);
				kill(pid, SIGKILL);
				throw std::runtime_error("execve failed");
			}
			cgi_response.append(buffer, bytesRead);
		}
		close(outputPipe[0]);
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			return cgi_response;
		else
			throw std::runtime_error("CGI script failed");
	}
}
