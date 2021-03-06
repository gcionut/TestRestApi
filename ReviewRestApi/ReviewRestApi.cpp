#include"pch.h"
#include <stdio.h>

#include <iostream>
/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * BlackJack_Servr.cpp - Simple server application for blackjack
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include <cpprest/http_listener.h>
#include "utils.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class ReviewRestApi : public HTTPEndpoint {
public:
	ReviewRestApi() :HTTPEndpoint() {}
	ReviewRestApi(const string_t& url) :HTTPEndpoint(url) {}
private:
	void handle_get(http_request message);
};
std::unique_ptr<ReviewRestApi> g_httpDealer;

void ReviewRestApi::handle_get(http_request message) {
	//	std::cout << message.extract_string() << std::endl;
		//std::cout << message.extract_string() << std::endl;
	auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
	if (paths.size() > 0) {
		if (paths[0] == U("session")) {
			for (int i = 1; i <= 15; i++) {

			}

		}
		else if (paths[0] == U("user")) {
			std::cout << "get user" << std::endl;
		}
		else if (paths[0] == U("feedback")) {
			std::cout << "get feedback" << std::endl;
			string_t q = message.relative_uri().query();
			auto query = message.relative_uri().split_query(q);
			int rate = std::stoi(query[L"rate"]);
			const std::vector<feedback> * arr = db.getFeedback(rate);
			//auto jsonarr = json::value::object(values);
			json::value arrObnect = json::value::array();
			int idx = 0;
			for (int i =0; i < arr->size(); i++) {
				const utility::string_t userID = utility::conversions::to_string_t((*arr)[i].userid);
				const utility::string_t comment = utility::conversions::to_string_t((*arr)[i].comment);
				json::value obj;
				obj[L"Ubi-UserId"] = json::value::string(userID);
				obj[L"sessionid"] = json::value::number(44);
				obj[L"rate"] = json::value::number(43.6);
				obj[L"comment"] = json::value::string(comment);

				arrObnect[idx++] = obj;
			}
			message.reply(status_codes::OK, arrObnect);
		}
	}

	std::string body = "TEST";

	message.reply(status_codes::OK);

	message.reply(status_codes::OK);
}

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "blackjack/dealer"
	utility::string_t custome = U("/api/");
	uri_builder uri(address);
	uri.append_path(custome);

	auto addr = uri.to_uri().to_string();
	g_httpDealer = std::unique_ptr<ReviewRestApi>(new ReviewRestApi(addr));
	g_httpDealer->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpDealer->close().wait();
	return;
}

//
// To start the server, run the below command with admin privileges:
// BlackJack_Server.exe <port>
// If port is not specified, will listen on 34568
//

int main(int argc, char* argv[])

{
	utility::string_t port = U("11111");


	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}