#include "HttpParser.hpp"
#include "gtest/gtest.h"

//if on mac with homebrew use this command to compile
//compile g++ -std=c++14 -I /opt/homebrew/include/ -L /opt/homebrew/lib/ -lgtest -lgtest_main -pthread ParserTest.cpp HttpParser.cpp -o ParserTest

TEST(HttpParserTest, ParsesValidRequest) {
	std::string request =
		"GET /path?query=value HTTP/1.1\r\n"
		"Host: www.example.com\r\n"
		"User-Agent: curl/7.64.1\r\n"
		"Accept: */*\r\n"
		"\r\n";

	HttpParser parser(request);

	EXPECT_EQ(parser.getMethod(), "GET");
	EXPECT_EQ(parser.getUrl(), "/path?query=value");
	EXPECT_EQ(parser.getVersion(), "HTTP/1.1");
	EXPECT_EQ(parser.getHeaders()["Host"], "www.example.com");
	EXPECT_EQ(parser.getHeaders()["User-Agent"], "curl/7.64.1");
	EXPECT_EQ(parser.getHeaders()["Accept"], "*/*");
}

TEST(HttpParserTest, ThrowsOnInvalidRequestLine) {
	std::string request = "INVALID REQUEST LINE\r\n";

	EXPECT_THROW(HttpParser parser(request), std::runtime_error);
}

TEST(HttpParserTest, ThrowsOnInvalidHeaderLine) {
	std::string request =
		"GET / HTTP/1.1\r\n"
		"InvalidHeader\r\n"
		"\r\n";

	EXPECT_THROW(HttpParser parser(request), std::runtime_error);
}


TEST(HttpParserTest, ParsesGetRequest) {
    std::string request = "GET /path?query=value HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "\r\n";
    //HttpParser parser(request);
    ASSERT_NO_THROW(HttpParser parser(request));
    // Add more assertions to verify the parsed result if needed
}

TEST(HttpParserTest, ParsesPostRequest) {
    std::string request = "POST /submit HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "Content-Length: 11\r\n"
                          "\r\n"
                          "Hello World";
    //HttpParser parser;
    ASSERT_NO_THROW(HttpParser parser(request));
    // Add more assertions to verify the parsed result if needed
}

TEST(HttpParserTest, ParsesDeleteRequest) {
    std::string request = "DELETE /resource/123 HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "\r\n";
    //HttpParser parser;
    ASSERT_NO_THROW(HttpParser parser(request));
    // Add more assertions to verify the parsed result if needed
}

// Add more test cases as needed

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
