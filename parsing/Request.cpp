/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogorfti <ogorfti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:47:33 by ogorfti           #+#    #+#             */
/*   Updated: 2023/11/08 18:05:12 by ogorfti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*-------------------- Member functions --------------------*/

void    Request::splitLines()
{
	size_t begin = 0;
	size_t end = 0;

	while ((end = request_.find("\r\n", begin)) != std::string::npos)
	{
		this->lines_.push_back(request_.substr(begin, end - begin));
		begin = end + 2;
	}
}

void	Request::parseFirstLine()
{
	std::string firstLine = lines_[0];
	size_t methodEnd = firstLine.find(" ");
	size_t urlEnd = firstLine.find(" ", methodEnd + 1);

	this->method_ = firstLine.substr(0, methodEnd);
	this->url_ = firstLine.substr(methodEnd + 1, urlEnd - methodEnd - 1);
	this->protocol_ = firstLine.substr(urlEnd + 1);
}

void Request::parseHeaders()
{
	size_t pos = 0;
	for (size_t i = 1; i < this->lines_.size(); i++)
	{
		pos = lines_[i].find(":");
		if (pos != std::string::npos)
		{
			std::string key = lines_[i].substr(0, pos);
			std::string value = lines_[i].substr(pos + 2);
			headers_[key] = value;
		}
	}
}

void Request::parseBody()
{
	size_t pos = request_.find("\r\n\r\n");
	this->body_ = request_.substr(pos + 4);
}

void Request::parseRequest()
{
	splitLines();
	parseFirstLine();
	parseHeaders();
	parseBody();
}

/*-------------------- Getters --------------------*/

const std::string& Request::getMethod() const
{
	return (method_);
}

const std::string& Request::getURL() const
{
	return (url_);
}

const std::string& Request::getProtocol() const
{
	return (protocol_);
}
const std::string& Request::getBody() const
{
	return (body_);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
	return (headers_);
}

/*-------------------- Tmp --------------------*/

// int main()
// {
// 	std::string host = "api.example.com";
//     std::string requestBody = "{ \"key1\": \"value1\", \"key2\": \"value2\" }";
    
//     std::string httpRequest = "POST /api/data HTTP/1.1\r\n";
//     httpRequest += "Host: " + host + "\r\n";
//     httpRequest += "User-Agent: Mozilla/5.0\r\n";
//     httpRequest += "Accept: application/json\r\n";
//     httpRequest += "Accept-Language: en-US,en;q=0.5\r\n";
//     httpRequest += "Accept-Encoding: gzip, deflate, br\r\n";
//     httpRequest += "Connection: keep-alive\r\n";
//     httpRequest += "Content-Type: application/json\r\n";
//     httpRequest += "\r\n";
//     httpRequest += requestBody;

//     Request parser(httpRequest);
//     parser.parseRequest();

// 	std::cout << "--------" << std::endl;
//     std::cout << "Method: " << parser.getMethod() << std::endl;
//     std::cout << "URL: " << parser.getURL() << std::endl;
//     std::cout << "Protocol: " << parser.getProtocol() << std::endl;
    
// 	std::cout << "--------" << std::endl;
// 	std::cout << "Headers:" << std::endl;
// 	const std::map<std::string, std::string>& headers = parser.getHeaders();

// 	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
// 		std::cout << it->first << ": " << it->second << std::endl;
// 	}

// 	std::cout << "--------" << std::endl;
//     std::cout << "Request Body: " << parser.getBody() << std::endl;

//     return 0;
// }
