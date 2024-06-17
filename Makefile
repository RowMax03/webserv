NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = $(SRC) $(SERVER) $(SOCKET) $(CGI) $(CONFPARSER) $(HTTP_PARSER) $(HTTP_RESPONSE)

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_FILES = main.cpp
SRC_DIR = srcs/

SERVER = $(addprefix $(SERVER_DIR), $(SERVER_FILES))
SERVER_FILES = Server.cpp
SERVER_DIR = $(addprefix $(SRC_DIR), server/)

SOCKET = $(addprefix $(SOCKET_DIR), $(SOCKET_FILES))
SOCKET_FILES = ASocket.cpp ClientSocket.cpp ServerSocket.cpp
SOCKET_DIR = $(addprefix $(SRC_DIR), sockets/)

CGI = $(addprefix $(CGI_DIR), $(CGI_FILES))
CGI_FILES = CGI.cpp
CGI_DIR = $(addprefix $(SRC_DIR), cgi/)

CONFPARSER = $(addprefix $(CONFPARSER_DIR), $(CONFPARSER_FILES))
CONFPARSER_FILES = ConfigParser.cpp
CONFPARSER_DIR = $(addprefix $(SRC_DIR), conf_parser/)


HTTP_FILES =
HTTP_DIR = $(addprefix $(SRC_DIR), http/)

HTTP_PARSER = $(addprefix $(HTTP_PARSER_DIR), $(HTTP_PARSER_FILES))
HTTP_PARSER_FILES = HttpParser.cpp
HTTP_PARSER_DIR = $(addprefix $(HTTP_DIR), parser/)

HTTP_RESPONSE = $(addprefix $(HTTP_RESPONSE_DIR), $(HTTP_RESPONSE_FILES))
HTTP_RESPONSE_FILES =
HTTP_RESPONSE_DIR = $(addprefix $(HTTP_DIR), response/)

#objects

OBJ_DIR = .obj/
OBJ_FILES = $(SRCS:.cpp=.o)
ALL_OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRCS))
ALL_OBJ_DIR = $(sort $(dir $(ALL_OBJ)))
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(ALL_OBJ)

re: fclean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
