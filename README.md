<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HTTP Server Modules</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
        }
        .module {
            margin-bottom: 2em;
        }
        h2 {
            background-color: #f4f4f4;
            padding: 0.5em;
            border-left: 4px solid #007BFF;
        }
        h3 {
            color: #007BFF;
        }
        ul {
            list-style-type: none;
            padding: 0;
        }
        ul li {
            padding: 0.5em 0;
        }
    </style>
</head>
<body>
    <div class="module">
        <h2>1. Main Server Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Entry point of the server.</li>
            <li>Sets up socket, binds, listens, and accepts connections.</li>
            <li>Manages the main event loop using <code>poll()</code> for non-blocking I/O.</li>
            <li>Reads the configuration file.</li>
            <li>Initializes the thread pool.</li>
            <li>Ensures server listens on multiple ports as defined in the configuration file.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module initializes the server by reading the configuration file to get settings such as ports and the number of threads. It then sets up the server sockets, binding them to the specified ports and setting them to listen for incoming connections. The main event loop uses <code>poll()</code> to monitor all server sockets for incoming connections and delegates handling of accepted connections to worker threads from the thread pool.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>int main(int argc, char* argv[])</code></li>
            <li><code>void initializeServer(const std::string &configFilePath)</code></li>
            <li><code>void setupSockets()</code></li>
            <li><code>void mainEventLoop()</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>socket, setsockopt, bind, listen, accept, poll, close, read, write, open, fcntl</code></p>
    </div>

    <div class="module">
        <h2>2. Configuration Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Parses the configuration file.</li>
            <li>Provides server settings such as ports, document root, and other configurations.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module is responsible for reading and parsing the configuration file specified at startup. It extracts configuration settings like ports to listen on, the document root for serving files, and other relevant server settings. These settings are stored in a structured format for easy access by other modules.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>void loadConfiguration(const std::string &configFilePath)</code></li>
            <li><code>std::string getSetting(const std::string &key)</code></li>
        </ul>
        <h3>Data Structures:</h3>
        <p><code>struct ServerConfig</code></p>
        <h3>Allowed Functions:</h3>
        <p><code>open, read, close, strerror</code></p>
    </div>

    <div class="module">
        <h2>3. Connection Handler Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Manages individual client connections.</li>
            <li>Reads and parses HTTP requests.</li>
            <li>Generates and sends HTTP responses.</li>
            <li>Handles GET, POST, and DELETE methods.</li>
            <li>Serves static files and processes file uploads.</li>
            <li>Ensures non-blocking read and write operations using <code>poll()</code>.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module handles the lifecycle of a client connection. It reads the HTTP request from the client, parses it to determine the request method and target resource, and generates an appropriate HTTP response. For GET requests, it serves static files. For POST requests, it handles file uploads. For DELETE requests, it deletes specified resources. It ensures all I/O operations are non-blocking and monitored using <code>poll()</code>.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>void handleClientConnection(int clientSocket)</code></li>
            <li><code>void processHTTPRequest(int clientSocket)</code></li>
            <li><code>void sendHTTPResponse(int clientSocket, const std::string &response)</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>recv, send, read, write, poll, strerror, stat, open, close, dup, dup2, pipe, fork, execve</code></p>
    </div>

    <div class="module">
        <h2>4. Thread Pool Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Manages a pool of worker threads.</li>
            <li>Assigns client requests to threads for concurrent handling.</li>
            <li>Uses a task queue for managing client requests.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module initializes and manages a pool of worker threads to handle client connections concurrently. It maintains a task queue where incoming client requests are enqueued. Worker threads dequeue tasks and process them, ensuring efficient handling of multiple clients simultaneously without blocking the main event loop.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>void initializeThreadPool(int numThreads)</code></li>
            <li><code>void assignTaskToThread(std::function<void()> task)</code></li>
            <li><code>void workerThreadFunction()</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>pthread_create, pthread_join, pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_wait, pthread_cond_signal</code></p>
    </div>

    <div class="module">
        <h2>5. HTTP Parser Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Parses HTTP requests.</li>
            <li>Extracts method, URI, headers, and other relevant information.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module is responsible for parsing the raw HTTP request data received from the client. It extracts the HTTP method (GET, POST, DELETE), URI, headers, and other necessary information. The parsed data is structured into an <code>HTTPRequest</code> object for easy processing by the connection handler module.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>HTTPRequest parseHTTPRequest(const std::string &rawRequest)</code></li>
        </ul>
        <h3>Data Structures:</h3>
        <p><code>struct HTTPRequest</code></p>
        <h3>Allowed Functions:</h3>
        <p><code>strtok, strdup, strlen, strcpy, strcmp</code></p>
    </div>

    <div class="module">
        <h2>6. HTTP Response Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Generates HTTP responses.</li>
            <li>Formats headers and body for different types of responses.</li>
            <li>Provides default error pages.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module generates HTTP responses based on the processed request data. It formats the response headers and body, ensuring proper HTTP status codes and content. It also provides default error pages for standard HTTP errors like 404 Not Found and 500 Internal Server Error.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>std::string generateHTTPResponse(const HTTPRequest &request)</code></li>
            <li><code>std::string generateErrorResponse(int statusCode)</code></li>
        </ul>
        <h3>Data Structures:</h3>
        <p><code>struct HTTPResponse</code></p>
        <h3>Allowed Functions:</h3>
        <p><code>strcat, sprintf, snprintf, strerror</code></p>
    </div>

    <div class="module">
        <h2>7. Static File Handler Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Serves static files from the filesystem.</li>
            <li>Determines MIME types.</li>
            <li>Handles file uploads.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module is responsible for serving static files requested by clients. It reads the requested file from the filesystem, determines the correct MIME type, and includes the file content in the HTTP response. It also handles file uploads for POST requests by saving the uploaded files to the appropriate location on the server.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>std::string serveStaticFile(const std::string &filePath)</code></li>
            <li><code>std::string handleFileUpload(const std::string &uploadPath, const std::string &fileData)</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>open, read, write, close, stat, opendir, readdir, closedir, strerror</code></p>
    </div>

    <div class="module">
        <h2>8. Error Handling Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Manages error responses.</li>
            <li>Logs errors and other important events.</li>
            <li>Provides default error pages if none are provided.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module handles errors that occur during the processing of client requests. It generates appropriate error responses and logs errors for debugging and monitoring purposes. If custom error pages are not provided, it supplies default error pages.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>void logError(const std::string &errorMessage)</code></li>
            <li><code>std::string getDefaultErrorPage(int statusCode)</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>strerror, open, write, close, sprintf</code></p>
    </div>

    <div class="module">
        <h2>9. Utility Module</h2>
        <h3>Responsibilities:</h3>
        <ul>
            <li>Contains utility functions for tasks like string manipulation, logging, etc.</li>
            <li>Provides helper functions for network operations and file handling.</li>
        </ul>
        <h3>Description:</h3>
        <p>This module provides various utility functions that support the main functionality of the server. It includes functions for string manipulation, MIME type determination, logging, and other helper functions that are used across different modules.</p>
        <h3>Key Functions:</h3>
        <ul>
            <li><code>std::vector<std::string> splitString(const std::string &str, char delimiter)</code></li>
            <li><code>std::string getCurrentTime()</code></li>
            <li><code>std::string getMimeType(const std::string &fileExtension)</code></li>
        </ul>
        <h3>Allowed Functions:</h3>
        <p><code>strtok, strdup, strlen, strcpy, strcmp, strerror, time</code></p>
    </div>
</body>
</html>
