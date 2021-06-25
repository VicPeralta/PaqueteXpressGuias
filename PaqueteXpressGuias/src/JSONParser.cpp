#include "pch.h"
#include "JSONParser.h"
#include "Poco\JSON\Parser.h"
#include "Poco\SharedPtr.h"
#include "Poco\JSON\Object.h"
#include "Poco\JSON\JSONException.h"
#include "Poco/Dynamic/Var.h"


LoginData JSONParser::ParseLogin(std::string data)
{
	LoginData result{};
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parseResult = parser.parse(data);
	Poco::JSON::Object::Ptr content = parseResult.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var body = content->get("body");
	Poco::JSON::Object::Ptr bodyData = body.extract< Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var response = bodyData->get("response");
	Poco::JSON::Object::Ptr responseData = response.extract< Poco::JSON::Object::Ptr>();
	auto success = responseData->getValue<bool>("success");
	if (success) {
		result.success = true;
		auto token = responseData->get("data");
		auto tokenData = token.extract<Poco::JSON::Object::Ptr>();
		result.token = tokenData->getValue<std::string>("token");
	}
	else {
		result.success = false;
		auto messages = responseData->getArray("messages");
		result.error = messages->getObject(0)->getValue<std::string>("description");
	}
	return result;
}

CartaData JSONParser::ParseCarta(std::string data)
{
	CartaData result{};
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parseResult = parser.parse(data);
	Poco::JSON::Object::Ptr content = parseResult.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var body = content->get("body");
	Poco::JSON::Object::Ptr bodyData = body.extract< Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var response = bodyData->get("response");
	Poco::JSON::Object::Ptr responseData = response.extract< Poco::JSON::Object::Ptr>();
	auto success = responseData->getValue<bool>("success");
	if (success) {
		result.success = true;
		result.rastreo = responseData->getValue<std::string>("data");
		result.folio= responseData->getValue<std::string>("objectDTO");
	}
	else {
		result.success = false;
		auto messages = responseData->getArray("messages");
		result.error = messages->getObject(0)->getValue<std::string>("description");
	}
	return result;
}

ZPLData JSONParser::PaserZPL(std::string data)
{
	ZPLData result{};
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parseResult = parser.parse(data);
	Poco::JSON::Object::Ptr content = parseResult.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var body = content->get("body");
	Poco::JSON::Object::Ptr bodyData = body.extract< Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var response = bodyData->get("response");
	Poco::JSON::Object::Ptr responseData = response.extract< Poco::JSON::Object::Ptr>();
	auto success = responseData->getValue<bool>("success");
	if (success) {
		result.success = true;
		Poco::Dynamic::Var data = responseData->get("data");
		Poco::JSON::Object::Ptr dataData = data.extract< Poco::JSON::Object::Ptr>();
		Poco::Dynamic::Var solicitud = dataData->get("retornoSolicitud");
		Poco::JSON::Object::Ptr solicitudData = solicitud.extract< Poco::JSON::Object::Ptr>();
		result.zpl = solicitudData->getValue<std::string>("cadenaImpresion");
	}
	else {
		result.success = false;
		auto messages = responseData->getArray("messages");
		result.error = messages->getObject(0)->getValue<std::string>("description");
	}
	return result;
}

std::string JSONParser::UpdateToken(const std::string& data, std::string newToken)
{
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parseResult;
	parseResult = parser.parse(data);
	Poco::JSON::Object::Ptr content = parseResult.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var header = content->get("header");
	Poco::JSON::Object::Ptr bodyData = header.extract< Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var security = bodyData->get("security");
	Poco::JSON::Object::Ptr securityData = security.extract< Poco::JSON::Object::Ptr>();
	auto token = securityData->getValue<std::string>("token");
	securityData->set("token", newToken);
	return parseResult.toString();
}

