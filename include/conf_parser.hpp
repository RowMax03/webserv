/**
 * @file conf_parser.hpp
 * @brief This file contains the confParser class and related structures.
 */

#pragma once

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>

namespace confParser {

    /**
     * @struct ErrorPage
     * @brief A structure to hold error page information.
     */
    struct ErrorPage {
        int status; ///< The status code for the error page.
        std::string url; ///< The URL of the error page.
        ErrorPage* next; ///< Pointer to the next ErrorPage object.
    };

    /**
     * @struct LocationConfig
     * @brief A structure to hold location configuration information.
     */
    struct LocationConfig {
        std::string path; ///< The path of the location.
        std::string methods[3]; ///< The allowed methods (GET, POST, DELETE).
        int redirect_status; ///< The status code for redirection.
        std::string redirect_url; ///< The URL for redirection.
        std::string root; ///< The root directory for this location.
        bool autoindex; ///< Flag to indicate if autoindex is enabled.
        std::string index; ///< The index file for this location.
        LocationConfig* next; ///< Pointer to the next LocationConfig object.

        LocationConfig(); ///< Constructor for LocationConfig.
    };

    /**
     * @struct ServerConfig
     * @brief A structure to hold server configuration information.
     */
    struct ServerConfig {
        int listen; ///< The port number to listen on.
        std::string server_name; ///< The server name.
        int server_names_hash_bucket_size; ///< The size of the server names hash bucket.
        ErrorPage* error_pages; ///< Pointer to the list of error pages.
        std::string client_max_body_size; ///< The maximum client body size.
        LocationConfig* locations; ///< Pointer to the list of location configurations.

        ServerConfig(); ///< Constructor for ServerConfig.
    };

    /**
     * @class confParser
     * @brief A class to parse a configuration file.
     *
     * This class provides methods to parse a configuration file and store the configuration values in a ServerConfig object.
     */
    class confParser {
    public:
        /**
         * @brief Constructor that takes a configuration file
         * @param configFile The path to the configuration file
         */
        confParser(const std::string& configFile);

        /**
         * @brief Copy constructor
         * @param other The object to copy from
         */
        confParser(const confParser& other);

        /**
         * @brief Copy assignment operator
         * @param other The object to copy from
         * @return A reference to the current object
         */
        confParser& operator=(const confParser& other);

        /**
         * @brief Destructor
         */
        ~confParser();

        /**
         * @brief ServerConfig member variable
         */
        ServerConfig* serverConfig;

    private:
        /**
         * @brief Method to remove leading spaces from a string
         * @param str The string to process
         * @return The processed string
         */
        std::string removeLeadingSpaces(const std::string& str);

        /**
         * @brief Method to parse the configuration file
         * @param configFile The path to the configuration file
         */
        void parseConfigFile(const std::string& configFile);

        /**
         * @brief Method to read the configuration file
         * @param file The file to read from
         * @param serverConf Pointer to the server configuration vector
         * @param locationConfs Pointer to the location configuration vector
         */
        void readConfigFile(std::ifstream& file, std::vector<std::vector<std::string> >*& serverConf,
                            std::vector<std::vector<std::vector<std::string> > >*& locationConfs);

        /**
         * @brief Method to tokenize a string
         * @param str The string to tokenize
         * @param delim The delimiter to use for tokenization
         * @return A vector of tokens
         */
        std::vector<std::string> tokenize(const std::string& str, const std::string& delim);

        /**
         * @brief Method to set the server configuration values
         * @param serverConf A vector of server configuration values
         */
        void setServerConfigValue(std::vector<std::vector<std::string> >* serverConf);

        /**
         * @brief Method to set the location configuration values
         * @param locationConfs A vector of location configuration values
         */
        void setLocationConfigValue(std::vector<std::vector<std::vector<std::string> > >* locationConfs);
    };

} // confParser