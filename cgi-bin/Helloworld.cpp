#include <iostream>

int main()
{
	std::string content = "<html><head><title>Hello World</title></head><body><h1>Hello World!</h1></body></html>";
	std::cout << "HTTP/1.1 200 OK" << std::endl;
	std::cout << "Content-type: text/html" << std::endl;
	std::cout << "Content-Length: " << content.length() << std::endl << std::endl;
	std::cout << content;
	return 0;
}

