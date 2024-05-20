# HTTP Server in C++98 (Single-Threaded with `poll()`)

This project is a non-blocking HTTP server written in C++98. It uses `poll()` for I/O multiplexing and operates in a single-threaded environment.

## Module Structure

### 1. Main Server Module

**Responsibilities:**
- Entry point of the server.
- Sets up socket, binds, listens, and accepts connections.
- Manages the main event loop using `poll()` for non-blocking I/O.
- Reads the configuration file.
- Ensures server listens on multiple ports as defined in the configuration file.

**Description:**
This module initializes the server by reading the configuration file to get settings such as ports. It sets up the server sockets, binds them to the specified ports, and sets them to listen for incoming connections. The main event loop uses `poll()` to monitor all server sockets and client connections for incoming data and outgoing responses.

**Key Functions:**
- `int main(int argc, char* argv[])`
- `void initializeServer(const std::string &configFilePath)`
- `void setupSockets()`
- `void mainEventLoop()`

**Allowed Functions:**
`socket`, `setsockopt`, `bind`, `listen`, `accept`, `poll`, `close`, `read`, `write`, `open`, `fcntl`

### 2. Configuration Module

**Responsibilities:**
- Parses the configuration file.
- Provides server settings such as ports, document root, and other configurations.

**Description:**
This module reads and parses the configuration file specified at startup. It extracts configuration settings like ports to listen on, the document root for serving files, and other relevant server settings. These settings are stored in a structured format for easy access by other modules.

**Key Functions:**
- `void loadConfiguration(const std::string &configFilePath)`
- `std::string getSetting(const std::string &key)`

**Data Structures:**
`struct ServerConfig`

**Allowed Functions:**
`open`, `read`, `close`, `strerror`

### 3. Connection Handler Module

**Responsibilities:**
- Manages individual client connections.
- Reads and parses HTTP requests.
- Generates and sends HTTP responses.
- Handles GET, POST, and DELETE methods.
- Serves static files and processes file uploads.
- Ensures non-blocking read and write operations using `poll()`.

**Description:**
This module handles the lifecycle of a client connection. It reads the HTTP request from the client, parses it to determine the request method and target resource, and generates an appropriate HTTP response. For GET requests, it serves static files. For POST requests, it handles file uploads. For DELETE requests, it deletes specified resources. It ensures all I/O operations are non-blocking and monitored using `poll()`.

**Key Functions:**
- `void handleClientConnection(int clientSocket)`
- `void processHTTPRequest(int clientSocket)`
- `void sendHTTPResponse(int clientSocket, const std::string &response)`

**Allowed Functions:**
`recv`, `send`, `read`, `write`, `poll`, `strerror`, `stat`, `open`, `close`, `dup`, `dup2`, `pipe`, `fork`, `execve`

### 4. HTTP Parser Module

**Responsibilities:**
- Parses HTTP requests.
- Extracts method, URI, headers, and other relevant information.

**Description:**
This module parses the raw HTTP request data received from the client. It extracts the HTTP method (GET, POST, DELETE), URI, headers, and other necessary information. The parsed data is structured into an `HTTPRequest` object for easy processing by the connection handler module.

**Key Functions:**
- `HTTPRequest parseHTTPRequest(const std::string &rawRequest)`

**Data Structures:**
`struct HTTPRequest`

**Allowed Functions:**
`strtok`, `strdup`, `strlen`, `strcpy`, `strcmp`

### 5. HTTP Response Module

**Responsibilities:**
- Generates HTTP responses.
- Formats headers and body for different types of responses.
- Provides default error pages.

**Description:**
This module generates HTTP responses based on the processed request data. It formats the response headers and body, ensuring proper HTTP status codes and content. It also provides default error pages for standard HTTP errors like 404 Not Found and 500 Internal Server Error.

**Key Functions:**
- `std::string generateHTTPResponse(const HTTPRequest &request)`
- `std::string generateErrorResponse(int statusCode)`

**Data Structures:**
`struct HTTPResponse`

**Allowed Functions:**
`strcat`, `sprintf`, `snprintf`, `strerror`

### 6. Static File Handler Module

**Responsibilities:**
- Serves static files from the filesystem.
- Determines MIME types.
- Handles file uploads.

**Description:**
This module serves static files requested by clients. It reads the requested file from the filesystem, determines the correct MIME type, and includes the file content in the HTTP response. It also handles file uploads for POST requests by saving the uploaded files to the appropriate location on the server.

**Key Functions:**
- `std::string serveStaticFile(const std::string &filePath)`
- `std::string handleFileUpload(const std::string &uploadPath, const std::string &fileData)`

**Allowed Functions:**
`open`, `read`, `write`, `close`, `stat`, `opendir`, `readdir`, `closedir`, `strerror`

### 7. Error Handling Module

**Responsibilities:**
- Manages error responses.
- Logs errors and other important events.
- Provides default error pages if none are provided.

**Description:**
This module handles errors that occur during the processing of client requests. It generates appropriate error responses and logs errors for debugging and monitoring purposes. If custom error pages are not provided, it supplies default error pages.

**Key Functions:**
- `void logError(const std::string &errorMessage)`
- `std::string getDefaultErrorPage(int statusCode)`

**Allowed Functions:**
`strerror`, `open`, `write`, `close`, `sprintf`

### 8. Utility Module

**Responsibilities:**
- Contains utility functions for tasks like string manipulation, logging, etc.
- Provides helper functions for network operations and file handling.

**Description:**
This module provides various utility functions that support the main functionality of the server. It includes functions for string manipulation, MIME type determination, logging, and other helper functions used across different modules.

**Key Functions:**
- `std::vector<std::string> splitString(const std::string &str, char delimiter)`
- `std::string getCurrentTime()`
- `std::string getMimeType(const std::string &fileExtension)`

**Allowed Functions:**
`strtok`, `strdup`, `strlen`, `strcpy`, `strcmp`, `strerror`, `time`

