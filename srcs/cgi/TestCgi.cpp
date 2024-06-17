#include "CGI.hpp"
#include <gtest/gtest.h>

TEST(CGITest, TestRun) {
	// Initialize with appropriate values
	std::string httpRequest = "GET /cgi-bin/HelloWorld.cgi HTTP/1.1\r\n"
							  "Host: localhost\r\n"
							  "\r\n";

	HttpParser request(httpRequest);
	std::cout << request.getPath() << std::endl;
	CGI cgi(request);

	// Test that run() returns the expected output
	std::string output = cgi.run();
	std::string expectedOutput = "Content-type: text/html\n"
								 "Content-Length: 86\n"
								 "\n"
								 "<html><head><title>Hello World</title></head><body><h1>Hello World!</h1></body></html>";
	EXPECT_EQ(output, expectedOutput);
}
//TEST(CGITest, TestCheckRights) {
//	HttpParser request; // Initialize with appropriate values
//	CGI cgi(request);

//	// Test that checkRights() throws an exception for a non-existent file
//	EXPECT_THROW(cgi.checkRigths("/path/to/nonexistent/file"), std::runtime_error);

//	// Test that checkRights() throws an exception for a non-executable file
//	EXPECT_THROW(cgi.checkRigths("/path/to/nonexecutable/file"), std::runtime_error);

//	// Test that checkRights() does not throw an exception for an executable file
//	EXPECT_NO_THROW(cgi.checkRigths("/path/to/executable/file"));
//}

// Add more tests for other methods as needed...
