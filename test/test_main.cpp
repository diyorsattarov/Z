#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

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
    spdlog::info("Start of test fixture!");
    ssl::stream<tcp::socket> socket(io_context, ssl_context);
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("id.twitch.tv", "https");

    boost::asio::connect(socket.next_layer(), endpoints.begin(), endpoints.end());

    socket.handshake(ssl::stream_base::client);

    http::request<http::string_body> req{http::verb::post, "/oauth2/token", 11};
    req.set(http::field::host, "id.twitch.tv");
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    std::string client_id = "client_id";
    std::string client_secret = "client_secret";
    req.body() = "client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
    req.prepare_payload();

    http::write(socket, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);

    spdlog::info("Response status code: {}", res.result_int());
    spdlog::info("Response body: {}", beast::buffers_to_string(res.body().data()));
    EXPECT_EQ(res.result(), http::status::ok);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
