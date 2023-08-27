#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

// Base fixture with common setup and teardown
class BaseTestFixture : public ::testing::Test {
protected:
    BaseTestFixture() : io_context(), ssl_context(ssl::context::sslv23) {
        ssl_context.set_default_verify_paths();
        spdlog::info("Welcome to spdlog!");
    }

    asio::io_context io_context;
    ssl::context ssl_context;
};

// Derived fixture for specific test cases
class MyTestFixture : public BaseTestFixture {
};

TEST_F(MyTestFixture, TestHttpClient) {
    ssl::stream<tcp::socket> socket(io_context, ssl_context);
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("id.twitch.tv", "https");

    boost::asio::connect(socket.next_layer(), endpoints.begin(), endpoints.end());

    socket.handshake(ssl::stream_base::client);

    http::request<http::string_body> req{http::verb::post, "/oauth2/token", 11};
    req.set(http::field::host, "id.twitch.tv");
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    std::string client_id = "q6yhym92s2q8bou0xx0o4mvq4qexqz";
    std::string client_secret = "8m8ptibbflgyhobbsdt0jzlg5ulf8a";
    req.body() = "client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
    req.prepare_payload();

    http::write(socket, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    spdlog::info("Response status code: {}", res.result_int());
    spdlog::info("Response body: {}", beast::buffers_to_string(res.body().data()));
    EXPECT_EQ(res.result(), http::status::ok);
    nlohmann::json json_response = nlohmann::json::parse(beast::buffers_to_string(res.body().data()));

    // Extract the "access_token" from the JSON
    std::string access_token = json_response["access_token"];

    spdlog::info("Access token: {}", access_token);    

    http::request<http::string_body> req2{http::verb::post, "/helix/polls", 11};
    req2.set(http::field::host, "api.twitch.tv");
    req2.set(http::field::authorization, "Bearer " + access_token); // Use "Bearer" prefix
    req2.set(http::field::content_type, "application/json");

    std::string json_payload = R"(
        {
            "broadcaster_id": "123456",
            "title": "Streaming next Tuesday. Which time works best for you?",
            "choices": [
                {"title": "9AM"},
                {"title": "10AM"},
                {"title": "7PM"},
                {"title": "8PM"},
                {"title": "9PM"}
            ],
            "duration": 300
        }
    )";

    req2.body() = json_payload;
    req2.prepare_payload();

    // Send the HTTP request
    http::write(socket, req2);

    // Read the HTTP response
    beast::flat_buffer buffer2;
    http::response<http::dynamic_body> res2;
    http::read(socket, buffer2, res2);

    spdlog::info("Response status code: {}", res2.result_int());
    spdlog::info("Response body: {}", beast::buffers_to_string(res2.body().data()));
    EXPECT_EQ(res2.result(), http::status::ok);
}

/*
    http::request<http::string_body> req{http::verb::post, "/helix/polls", 11};
    req.set(http::field::host, "api.twitch.tv");
    req.set(http::field::authorization, "sm6wd2usxlhz9z2kg0hs8sxhejroz8");
    req.set(http::field::content_type, "application/json");

    std::string json_payload = R"(
        {
            "broadcaster_id": "123456",
            "title": "Streaming next Tuesday. Which time works best for you?",
            "choices": [
                {"title": "9AM"},
                {"title": "10AM"},
                {"title": "7PM"},
                {"title": "8PM"},
                {"title": "9PM"}
            ],
            "duration": 300
        }
    )";

    req.body() = json_payload;
    req.prepare_payload();

    // Send the HTTP request
    http::write(socket, req);

    // Read the HTTP response
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    spdlog::info("Response status code: {}", res.result_int());
    spdlog::info("Response body: {}", beast::buffers_to_string(res.body().data()));
    EXPECT_EQ(res.result(), http::status::ok);

*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
