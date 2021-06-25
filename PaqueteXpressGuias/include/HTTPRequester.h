#pragma once
#include <string>
#include <iostream>
#include "Poco\Net\HTTPClientSession.h"
#include "Poco\Net\HTTPRequest.h"
#include "Poco\Net\HTTPResponse.h"
#include "Poco\URI.h"
enum class RequestType {
	GET,
	POST
};
class HTTPRequester
{
private:
	std::string m_url{};
	unsigned short m_port{};
	std::string m_requestContent;
	std::string m_requestContentType;
	std::string m_responseBody;
	RequestType m_requestType{ RequestType::GET };
public:
	HTTPRequester()=default;
	HTTPRequester(const std::string url);
	HTTPRequester(const std::string url, unsigned short port);
	void setUrl(const std::string url);
	void setContent(const std::string&);
	void setContentType(const std::string&);
	void setRequestType(RequestType type);
	Poco::Net::HTTPResponse::HTTPStatus Request(std::string& response, bool verbose = false);
};

