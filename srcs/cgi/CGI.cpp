/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreidenb <mreidenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:00:40 by mreidenb          #+#    #+#             */
/*   Updated: 2024/06/17 18:55:42 by mreidenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

/**
 * @brief Construct a new CGI::CGI object
 *
 * @param request The HttpParser object that contains the request
 * @param config The Config object that containts the cgi config
 */
CGI::CGI(const HttpParser &request, const std::string &documentRoot) : _documentRoot(documentRoot), _request(request) {
	toCharArr(request.toCgiEnv());
}

/**
 * @brief Destroy the CGI::CGI object
 *
 */
CGI::~CGI() {
	deleteEnv();
}

/**
 * @brief Convert the envp vector to a char**
 *
 * @param envp The vector of strings that contains the envp
 */
void CGI::toCharArr(const std::vector<std::string> &envp) {
	_env = new char*[envp.size() + 1];
	for (size_t i = 0; i < envp.size(); i++) {
		_env[i] = new char[envp[i].size() + 1];
		std::strcpy(_env[i], envp[i].c_str());
	}
	_env[envp.size()] = NULL;
}

/**
 * @brief Delete (free) the env char**
 *
 */
void CGI::deleteEnv() {
	for (size_t i = 0; _env[i]; i++) {
		delete[] _env[i];
	}
	delete[] _env;
}

/**
 * @brief Run the CGI script
 *
 * @return std::string The response from the CGI script
 */
std::string CGI::run() {
	//document root, get from config later
	//std::string documentRoot = "/Users/max/Projekte/webserv";
	// check if we can open the file
	checkRigths(_documentRoot + _request.getPath());
	int inputPipe[2];
	int outputPipe[2];

	if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
		throw std::runtime_error("pipe failed");
	}

	pid_t pid = fork();
	if (pid == -1)
		throw std::runtime_error("fork failed");
	if (pid == 0)
		handleChildProcess(inputPipe, outputPipe, _documentRoot);
	else
		return handleParentProcess(inputPipe, outputPipe, pid);
	return "";
}

/**
 * @brief Check if the file exists and is executable
 *
 * @param path The path to the file
 */
void CGI::checkRigths(const std::string &path) {
	if (access(path.c_str(), F_OK) == -1) {
		throw std::runtime_error("file not found");
	}
	if (access(path.c_str(), X_OK) == -1) {
		throw std::runtime_error("file not executable");
	}
}

/**
 * @brief Handle the child process
 *
 * @param inputPipe The input pipe
 * @param outputPipe The output pipe
 * @param documentRoot The document root from the config
 */
void CGI::handleChildProcess(int inputPipe[2], int outputPipe[2], const std::string& documentRoot) {
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

/**
 * @brief Handle the parent process
 *
 * @param inputPipe The input pipe
 * @param outputPipe The output pipe
 * @param pid The pid of the child process
 * @return std::string The response from the CGI script
 */
std::string CGI::handleParentProcess(int inputPipe[2], int outputPipe[2], pid_t pid) {
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
		{ // execve failed in child process so we kill it and throw an exception
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
