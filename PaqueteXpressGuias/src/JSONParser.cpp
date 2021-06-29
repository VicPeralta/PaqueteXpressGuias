#include "pch.h"
#include "JSONParser.h"


LoginData JSONParser::ParseLogin(std::string data)
{
	LoginData result{};
	try {
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
	catch (Poco::JSON::JSONException& ex) {
		std::string messageError{ "Error al analizar JSON " };
		messageError += ex.displayText();
		throw std::exception{ messageError.c_str() };
	}
	catch (std::exception& ex) {
		throw ex;
	}
}

CartaData JSONParser::ParseCarta(std::string data)
{
	CartaData result{};
	try {
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
			result.folio = responseData->getValue<std::string>("objectDTO");
		}
		else {
			result.success = false;
			auto messages = responseData->getArray("messages");
			result.error = messages->getObject(0)->getValue<std::string>("description");
		}
		return result;
	}
	catch (Poco::JSON::JSONException& ex) {
		std::string messageError{ "Error al analizar JSON " };
		messageError += ex.displayText();
		throw std::exception{ messageError.c_str() };
	}
	catch (std::exception& ex) {
		throw ex;
	}
}

ZPLData JSONParser::PaserZPL(std::string data)
{
	ZPLData result{};
	try {
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
			Poco::Dynamic::Var dataJson = responseData->get("data");
			Poco::JSON::Object::Ptr dataData = dataJson.extract< Poco::JSON::Object::Ptr>();
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
	catch (Poco::JSON::JSONException& ex) {
		std::string messageError{ "Error al analizar JSON " };
		messageError += ex.displayText();
		throw std::exception{ messageError.c_str() };
	}
	catch (std::exception& ex) {
		throw ex;
	}
}

std::string JSONParser::UpdateToken(const std::string& data, std::string newToken)
{
	try {
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
	catch (Poco::JSON::JSONException& ex) {
		std::string messageError{ "Error al analizar JSON " };
		messageError += ex.displayText();
		throw std::exception{ messageError.c_str() };
	}
	catch (std::exception& ex) {
		throw ex;
	}
}

