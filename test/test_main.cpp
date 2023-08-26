#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

// Base fixture with common setup and teardown
class BaseTestFixture : public ::testing::Test {
protected:
    BaseTestFixture() : io_context(), ssl_context(boost::asio::ssl::context::sslv23) {
        ssl_context.set_default_verify_paths();
        spdlog::info("Welcome to spdlog!");
        // Create a console logger for the base fixture
        //console_logger = spdlog::stdout_logger_mt("console");
        //console_logger->set_pattern("[%H:%M:%S %z] [%l] %v");
    }

    boost::asio::io_context io_context;
    boost::asio::ssl::context ssl_context;
    //std::shared_ptr<spdlog::logger> console_logger; // Console logger instance
};


// Derived fixture for specific test cases
class MyTestFixture : public BaseTestFixture {
};

TEST_F(MyTestFixture, TestHttpClient) {
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_context, ssl_context);

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(socket.lowest_layer(), resolver.resolve("id.twitch.tv", "https"));

    socket.handshake(boost::asio::ssl::stream_base::client);

    std::string client_id = "your_client_id";
    std::string client_secret = "your_client_secret";

    std::string request =
        "POST /oauth2/token HTTP/1.1\r\n"
        "Host: id.twitch.tv\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: " + std::to_string(91 + client_id.length() + client_secret.length()) + "\r\n"
        "\r\n" +
        "client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";

    boost::asio::write(socket, boost::asio::buffer(request));

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    std::ostringstream response_stream;
    response_stream << &response;
    std::cout << response_stream.str();

    socket.shutdown();
    socket.lowest_layer().close();

    EXPECT_FALSE(response_stream.str().empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
