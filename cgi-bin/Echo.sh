#!/bin/sh
# Echo CGI envs as headers and body
CONTENT=$(cat -)
printf "HTTP/1.1 200 OK\r\n"
printf "Content-Length: ${#CONTENT}\r\n"
printf "Content-Type: text/html\r\n"
printf "REQUEST_METHOD: \"$REQUEST_METHOD\"\r\n"
printf "CONTENT_TYPE: \"$CONTENT_TYPE\"\r\n"
printf "CONTENT_LENGTH: \"$CONTENT_LENGTH\"\r\n"

printf "REMOTE_ADDR: \"$REMOTE_ADDR\"\r\n"
printf "REMOTE_PORT: \"$REMOTE_PORT\"\r\n"
printf "SERVER_PORT: \"$SERVER_PORT\"\r\n"

printf "REQUEST_URI: \"$REQUEST_URI\"\r\n"
printf "QUERY_STRING: \"$QUERY_STRING\"\r\n"

# all headers from request now available with the HTTP_ prefix
printf "HTTP_HOST: \"$HTTP_HOST\"\r\n"
printf "HTTP_USER_AGENT: \"$HTTP_USER_AGENT\"\r\n"
printf "HTTP_ACCEPT: \"$HTTP_ACCEPT\"\r\n"
printf "HTTP_REFERER: \"$HTTP_REFERER\"\r\n"

# username from basic auth (e.g. without password)
printf "REMOTE_USER: \"$REMOTE_USER\"\r\n"

printf "SCRIPT_NAME: \"$SCRIPT_NAME\"\r\n"
printf "PATH_INFO: \"$PATH_INFO\"\r\n"
printf "PATH: \"$PATH\"\r\n"
printf "PWD: \"$PWD\"\r\n"

printf "SERVER_PROTOCOL: \"$SERVER_PROTOCOL\"\r\n"
printf "SERVER_SOFTWARE: \"$SERVER_SOFTWARE\"\r\n"

printf "\r\n"
printf "$CONTENT"