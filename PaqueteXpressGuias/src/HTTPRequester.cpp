#include "pch.h"
#include "HTTPRequester.h"

HTTPRequester::HTTPRequester(const std::string url) : m_url{url}{}

HTTPRequester::HTTPRequester(const std::string url, unsigned short port) : m_url{ url }, m_port{ port }{}

void HTTPRequester::setUrl(const std::string url)
{
	m_url = url;
}

void HTTPRequester::setContent(const std::string& content)
{
	m_requestContent = content;
}

void HTTPRequester::setContentType(const std::string& contentType)
{
	m_requestContentType = contentType;
}

void HTTPRequester::setRequestType(RequestType type)
{
	m_requestType = type;
}

Poco::Net::HTTPResponse::HTTPStatus HTTPRequester::Request(std::string& response,bool verbose)
{
	if (m_url.empty())throw "No URL Set";
	Poco::URI uri(m_url);
	if (m_port != 0) uri.setPort(m_port);
	std::string path = uri.getPathAndQuery();
	if (path.empty()) path = "/";
	Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
	Poco::Net::HTTPRequest request{ Poco::Net::HTTPRequest::HTTPMessage::HTTP_1_1 };
	request.setURI(path);
	if (m_requestType == RequestType::GET)
		request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
	else	
		request.setMethod(Poco::Net::HTTPRequest::HTTP_POST);
	Poco::Net::HTTPResponse respond{};
	Poco::Net::HTTPResponse::HTTPStatus statusResult{};
	try {
		if (verbose) {
			std::cout << "Sending request to: " << session.getHost() << ":" << session.getPort() << "\n";
			std::cout << path << "\n";
		}
		request.setContentType(m_requestContentType);
		request.setContentLength(m_requestContent.length());
		auto& streamRequest = session.sendRequest(request);
		streamRequest.write(m_requestContent.c_str(), m_requestContent.size());
		auto& streamResponse = session.receiveResponse(respond);
		statusResult = respond.getStatus();
		if (verbose)
			std::cout << "Status: " << statusResult << "\n";
		std::string responseBody{};
		while (streamResponse) {
			char buffer[1024];
			streamResponse.read(buffer, 1023);
			responseBody.append(buffer, streamResponse.gcount());
		};
		response = std::move(responseBody);
	}
	catch (std::exception& ex) {
		throw ex;
	}
	return statusResult;
}
