// TestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "pch.h"
#include "utils.h"
#include "db.h"

#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_error.h"
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>

#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <vector>
#include <exception>



using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

typedef  std::pair<utility::string_t, web::json::value> jsonvalue;
 

int main()
{
	//DB db("192.168.2.104","test","test","test");
	//feedback feed;
	//feed.comment = "Tare";
	//feed.rate = 1;
	//feed.sessionid = 1;
	//feed.userid = "george";
	//db.insertFeedback(feed);
	//const std::vector<feedback>* arr = db.getFeedback(1);
	//int test;
	//sql::mysql::MySQL_Driver *driver;
	//sql::Connection *con;
	//try {
	//	driver = sql::mysql::get_mysql_driver_instance();
	//	con = driver->connect("tcp://192.168.2.104:3306", "test", "test");
	//	con->setCatalog("test");
	//	con->setSchema("test");
	//	sql::Statement * stmt = con->createStatement();
	//	//sql::Statement  stmt = con->createStatement();
	//	//stmt->executeQuery("insert into session(start,end) values(now(),now())");
	//	sql::ResultSet* res = stmt->executeQuery("SELECT idsession, start ,end from session");
	//	while (res->next()) {
	//		// You can use either numeric offsets...
	//		std::cout << "id = " << res->getInt(1); // getInt(1) returns the first column
	//		// ... or column names for accessing results.
	//		// The latter is recommended.

	//	}
	//	
	//	delete res;
	//	delete stmt;
	//	int rate = 5;
	//	std::stringstream ss;
	//	ss << "aelect * from feedback where rate = " << rate;
	//	std::string q = ss.str();
	//	std::cout << q << std::endl;
	//	stmt = con->createStatement();
	//	res = stmt->executeQuery(q);
	//	while (res->next()) {
	//		// You can use either numeric offsets...
	//		std::cout << "id = " << res->getInt(1); // getInt(1) returns the first column
	//		// ... or column names for accessing results.
	//		// The latter is recommended.
	//		std::cout << ", rate = '" << res->getString("rate") << "'" << std::endl;
	//		std::cout << ", comment = '" << res->getString("comment") << "'" << std::endl;
	//	}
	//	delete res;
	//	delete stmt;
	//	delete con;
	//}
	//catch (std::exception &ex)
	//{
	//	std::cout << "Standard exception: " << ex.what() << std::endl;
	//	exit(-1);
	//}
	////std::cout << conn.getSchema() << std::endl;

	
	while (true) {
		std::cout << "TEST" << std::endl << "1: feedback?rate=5";
		std::cout << "2: POST feedback " << std::endl;
		int test;
		std::cin >> test;
		auto fileStream = std::make_shared<ostream>();

		if (test == 1) {
			// Open stream to output file.
			pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
			{
				*fileStream = outFile;

				// Create http_client to send the request.
				http_client client(U("http://localhost:11111/api/feedback"));

				// Build request URI and start the request.
				uri_builder builder(U(""));
				builder.append_query(U("rate"), U("1"));
				return client.request(methods::GET, builder.to_string());
			})

				// Handle response headers arriving.
				.then([=](http_response response)
			{
				printf("Received response status code:%u\n", response.status_code());

				// Write response body into the file.
				return response.body().read_to_end(fileStream->streambuf());
			})

				// Close the file stream.
				.then([=](size_t)
			{
				return fileStream->close();
			});

			// Wait for all the outstanding I/O to complete and handle any exceptions
			try
			{
				requestTask.wait();
			}
			catch (const std::exception &e)
			{
				printf("Error exception:%s\n", e.what());
			}
		}
		else 		if (test == 2) {
			// Open stream to output file.
			pplx::task<void> requestTask2 = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
			{
				*fileStream = outFile;
				
				
				// Create http_client to send the request.
				http_client client(U("http://localhost:34568/api/feedback/"));
				
			
				utility::stringstream_t ss;
				int rate = 4;
				utility::string_t comment = U("Foarte tare;");
				ss << "{ \"rate\" : " << rate << ", \"comment\" : \"" << comment << "\"}";
				std::wcout << ss.str() << std::endl;
				json::value objRate = json::value::parse(ss.str());

				utility::ostringstream_t buf;
			
				uri_builder builder(U("/1"));
				http_request msg(methods::POST);
				msg.headers().add(U("Ubi-UserId"), U("ionut"));
				
				msg.set_request_uri(builder.to_string());
				msg.set_body(objRate);
				//response["ionut"] = json::value::number(1);
				//response["status”] = json::value::string("ready!”);
				// Build request URI and start the request.

			
				return client.request(msg);
			})

				// Handle response headers arriving.
				.then([=](http_response response)
			{
				printf("Received response status code:%u\n", response.status_code());

				// Write response body into the file.
				return response.body().read_to_end(fileStream->streambuf());
			})

				// Close the file stream.
				.then([=](size_t)
			{
				return fileStream->close();
			});

			// Wait for all the outstanding I/O to complete and handle any exceptions
			try
			{
				requestTask2.wait();
			}
			catch (const std::exception &e)
			{
				printf("Error exception:%s\n", e.what());
			}
		}
		else 		if (test == 3) {
			try {
				http_client clientSync(U("http://localhost:11111/api/"));
				http_response resp = clientSync.request(methods::GET, U("feedback/")).get();
				std::wcout << resp.status_code() << std::endl;
				Utils utils;
				json::value jsonResp = resp.extract_json().get();
				//std::wcout << jsonResp.serialize() << std::endl;
				std::wcout << utils.DisplayJSONValue(jsonResp);
			}
			catch (std::exception &ex) {
				std::cout << ex.what() << std::endl;
			}
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
