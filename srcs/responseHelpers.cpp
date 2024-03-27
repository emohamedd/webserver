/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHelpers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoigag <hoigag@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:18:19 by hoigag            #+#    #+#             */
/*   Updated: 2024/03/27 13:13:39 by hoigag           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseHelpers.hpp"

string getStatusReason(string& status)
{
	if (status == "200")
		return "OK";
	else if (status == "400")
		return "Bad Request";
	else if (status == "403")
		return "Forbidden";
	else if (status == "404")
		return "Not Found";
	else if (status == "405")
		return "Method Not Allowed";
	else if (status == "413")
		return "Request Entity Too Large";
	else if (status == "414")
		return "Request-URI Too Long";
	else if (status == "500")
		return "Internal Server Error";
	else if (status == "501")
		return "Not Implemented";
	else if (status == "301")
		return "Moved Permanently";
	else
		return "";
}



long long toBytes(string& bodyLimit)
{
	char* end;
	long long value = strtol(bodyLimit.c_str(), &end, 10);
	char unit = tolower(*end);

	switch (unit)
	{
		case 'k':
			value *= 1024;
			break;
		case 'm':
			value *= 1024 * 1024;
			break;
		case 'g':
			value *= 1024 * 1024 * 1024;
			break;
		case 'b':
		default:
			break;
	}

	return value;
}

// when i submit a file its got downloaded!! and its uploaded to the server
bool bodyLimitExceeded(Request& req, Location& loc)
{
	// cerr << "req.getBody().size() == " << req.getBody().size() << endl;
	// cerr << "toBytes(loc.bodyLimit) == " << toBytes(loc.bodyLimit) << endl;
	if (toBytes(loc.bodyLimit) < (long long)req.getBody().size())
		return true;
	return false;
}

bool chrURL(string& url)
{
	string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-./?:;=@[]!&'*+,%";

	for (size_t i = 0; i < url.size(); i++)	
	{
		if (str.find(url[i]) == string::npos)
			return true;
	}
	return false;
}
bool verifyPostReq(Request& req)
{
	if (req.getMethod() == "POST")
	{
		map<string, string> headers = req.getHeaders();
		if (headers.find("Content-Length") == headers.end() && headers.find("Transfer-Encoding") == headers.end())
			return true;
	}
	return false;
}

bool transferEncodingChunked(Request& req)
{
	map<string, string> headers = req.getHeaders();
	if (headers.find("Transfer-Encoding") != headers.end() && headers["Transfer-Encoding"] != "chunked")
	{
		cerr << RED << "Transfer-Encoding: " << headers["Transfer-Encoding"] << RESET << endl;	
		return true;
	}
	return false;
}


Location getMatchingLocation(const string& url, ConfigData& conf)
{
	Location* longestMatch = nullptr;
	
	for (size_t i = 0; i < conf.locations.size(); i++)
	{
		Location* loc = &conf.locations[i];
		if (url.find(loc->path) == 0)
		{
			if (longestMatch == nullptr || loc->path.size() > longestMatch->path.size())
				longestMatch = loc;
		}
	}
	return *longestMatch;
}