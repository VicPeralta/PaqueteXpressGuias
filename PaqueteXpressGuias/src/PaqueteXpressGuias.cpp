
#include "pch.h"
#include <iostream>
#include <fstream>
#include <chrono>

#include "HTTPRequester.h"
#include "JSONParser.h"
#include "utils.h"
#include "Settings.h"
#include "Logger.h"

void showUsage();
std::string getDataFromFile(std::string fileName);
std::string requestZPL(std::string url, std::string token, std::string rastreo, std::string user, std::string pwd);
std::string requestPDF(std::string url, std::string rastreo, std::string type);
std::string requestLogin(std::string url, std::string user, std::string pwd);
CartaData requestCarta(std::string token, std::string url, std::string file);

int main(int argc, char* argv[])
{
	if (argc != 4) {
		showUsage();
		return 1;
	}
	auto start = std::chrono::high_resolution_clock::now();
	auto& settings = Settings::getInstance();
	try {
		settings.loadSettingsFromJson("config.json");
		settings.addSetting("archivoJson", argv[1]);
		settings.addSetting("formatoEtiqueta", argv[2]);
		settings.addSetting("directorioSalida", argv[3]);
		std::string response{};
		std::cout << "Solicitando token...\n";
		std::string token = requestLogin(settings.getValue<std::string>("urlLogin"),
			settings.getValue<std::string>("user"),
			settings.getValue<std::string>("pwd"));
		std::cout << "Token obtenido ok\n";
		std::cout << "Solicitando carta porte...\n";
		auto info = requestCarta(token, settings.getValue<std::string>("urlCarta"),
			settings.getValue<std::string>("archivoJson"));
		std::cout << "Carta porte obtenida\n";
		std::cout << "Rastreo:\t" << info.rastreo << "\n";
		std::cout << "Folio:  \t" << info.folio << "\n";
		if (settings.getValue<std::string>("formatoEtiqueta") == "ZPL") {
			std::cout << "Obteniendo ZPL...\n";
			auto zpl = requestZPL(settings.getValue<std::string>("urlZpl"), token, info.rastreo,
				settings.getValue<std::string>("user"), settings.getValue<std::string>("pwd"));
			Logger::logToFile(settings.getValue<std::string>("archivoJson"), info.rastreo, "zpl",
				settings.getValue<std::string>("directorioSalida"), zpl);
		}
		else {
			std::cout << "Obteniendo PDF...\n";
			auto pdf = requestPDF(settings.getValue<std::string>("urlPDF"), info.rastreo, settings.getValue<std::string>("formatoEtiqueta"));
			Logger::logToFile(settings.getValue<std::string>("archivoJson"), info.rastreo, "pdf",
				settings.getValue<std::string>("directorioSalida"), pdf);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms\n";
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
	}
}

void showUsage() {
	ConsoleColor cc;
	std::cout << "PaqueteXpressGuias.exe [";
	std::cout << ConsoleColor::foregroundGreen << "nombre archivo json a procesar" << ConsoleColor::foregroundDefault;
	std::cout << "][";
	std::cout << ConsoleColor::foregroundGreen << "Formato de etiqueta" << ConsoleColor::foregroundDefault;
	std::cout << "][";
	std::cout << ConsoleColor::foregroundGreen << "Directorio de salida" << ConsoleColor::foregroundDefault << "]\n";
	std::cout << ConsoleColor::foregroundGreen << "Ejemplo:\n" << ConsoleColor::foregroundDefault;
	std::cout << R"(PaqueteXpressGuias.exe 2804722021.json PDF c:\WMS\out)" << "\n";
	std::cout << ConsoleColor::foregroundGreen << "Opciones Formato de etiqueta:\n" << ConsoleColor::foregroundDefault;
	auto m = u8"Archivo PDF tamaño carta";
	auto m1 = u8"Archivo PDF tamaño etiqueta";
	std::cout << ConsoleColor::foregroundRed << "PDFCarta\t" << ConsoleColor::foregroundDefault << reinterpret_cast<const char*>(m) << "\n";
	std::cout << ConsoleColor::foregroundRed << "PDFLabel\t" << ConsoleColor::foregroundDefault << reinterpret_cast<const char*>(m1) << "\n";
	std::cout << ConsoleColor::foregroundRed << "ZPL\t" << ConsoleColor::foregroundDefault << "Archivo formato ZPL" << "\n";
}

std::string getDataFromFile(std::string fileName) {
	std::ifstream is(fileName, std::ios::binary);
	std::string result;
	char buffer[1024];
	while (is) {
		is.read(buffer, 1024);
		result.append(buffer, is.gcount());
	}
	is.close();
	return result;
}

std::string requestZPL(std::string url, std::string token, std::string rastreo, std::string user, std::string pwd) {
#ifdef _DEBUG
	bool verbose = true;
#else
	bool verbose = false;
#endif // DEBUG
	ZPLData zplData{};
	try {
		std::string json{ R"({"header": { "security": {"token": ")" };
		json += token;
		json += R"(","user": ")";
		json += user;
		json += R"(","password":")";
		json += pwd;
		json += R"("}},"body": {"request": {"data": {"header": {},"solicitudEnvio": {"datosAdicionales": {
                        "datoAdicional": [{"claveDataAd": "getZPL","valorDataAd": "1"}]},"rastreo": ")";
		json += rastreo;
		json += R"("}}}}})";
		HTTPRequester httpRequester{ url };
		httpRequester.setRequestType(RequestType::POST);
		httpRequester.setContentType("application/json");
		httpRequester.setContent(json);
		std::string response{};
		auto res = httpRequester.Request(response, verbose);

		if (res == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
			JSONParser parser;
			zplData = parser.PaserZPL(response);
			if (!zplData.success) {
				std::string messageError{ "Error al obtener ZPL" };
				messageError += zplData.error;
				throw messageError;
			}
		}
		else {
			std::string messageError{ "Error obteniendo codigo ZPL" };
			messageError += res;
			throw messageError;
		}
	}
	catch (std::exception& ex) {
		std::string messageError{ "Error obteniendo codigo ZPL" };
		throw messageError;
	}
	return zplData.zpl;
}

std::string requestPDF(std::string url, std::string rastreo, std::string type) {
#ifdef _DEBUG
	bool verbose = true;
#else
	bool verbose = false;
#endif // DEBUG
	url += rastreo;
	if (type == "PDFLabel")
		url += "&measure=4x6";
	HTTPRequester httpRequester{ url };
	httpRequester.setRequestType(RequestType::GET);
	httpRequester.setContentType("application/pdf");
	std::string response{};
	auto res = httpRequester.Request(response, verbose);
	if (res == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
		return response;
	}
	else {
		std::string messageError{ "Error al obtener PDF " };
		messageError += res;
		throw messageError;
	}
}

std::string requestLogin(std::string url, std::string user, std::string pwd) {
	std::string response{};
	std::string token{};
	LoginData loginInfo{};
#ifdef _DEBUG
	bool verbose = true;
#else
	bool verbose = false;
#endif // DEBUG

	try {
		HTTPRequester httpRequester{ url };
		httpRequester.setRequestType(RequestType::POST);
		httpRequester.setContentType("application/json");
		std::string dataLogin{ R"({"header": { "security": { "user": ")" };
		dataLogin += user;
		dataLogin += R"(", "password": " )";
		dataLogin += pwd;
		dataLogin += R"("}}})";
		httpRequester.setContent(dataLogin);
		auto res = httpRequester.Request(response, verbose);
		if (res == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
			JSONParser parser;
			loginInfo = parser.ParseLogin(response);
			if (!loginInfo.success) {
				std::string messageError{ "Login no exitoso " };
				messageError += loginInfo.error;
				throw messageError;
			}
		}
		else {
			std::string messageError{ "Login rechazado " };
			messageError += res;
			throw messageError;
		}
	}
	catch (std::exception& ex) {
		std::string messageError{ "Error al hacer login" };
		messageError += ex.what();
		throw messageError;
	}
	return loginInfo.token;
}

CartaData requestCarta(std::string token, std::string url, std::string file) {
	CartaData cartaInfo{};
#ifdef _DEBUG
	bool verbose = true;
#else
	bool verbose = false;
#endif // DEBUG
	try {
		JSONParser parser;
		std::string response{};
		HTTPRequester httpRequester{ url };
		httpRequester.setRequestType(RequestType::POST);
		httpRequester.setContentType("application/json");
		auto content = getDataFromFile(file);
		if (content.empty()) throw "Archivo json invalido";
		auto updatedContent = parser.UpdateToken(content, token);
		httpRequester.setContent(updatedContent);
		auto res = httpRequester.Request(response, verbose);
		if (res == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
			cartaInfo = parser.ParseCarta(response);
			if (!cartaInfo.success) {
				std::string messageError{ "Carta porte denegada " };
				messageError += cartaInfo.error;
				throw messageError;
			}
		}
		else {
			std::string messageError{ "Solicitud de carta porte rechazada " };
			messageError += res;
			throw messageError;
		}
	}
	catch (std::exception& ex) {
		std::string messageError{ "Error al solicitar carta porte " };
		messageError += ex.what();
		throw messageError;
	}
	return cartaInfo;
}
