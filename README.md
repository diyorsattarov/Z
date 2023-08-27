markdown

# Twitch Chat Bot in C++

This is a simple Twitch chat bot written in C++ that interacts with the Twitch chat using the Twitch IRC protocol. The bot is designed to join a specified Twitch channel and respond to commands from viewers.

## Features

- Join Twitch chat channel and listen for messages.
- Respond to specific commands from viewers.
- Customizable command handling and response logic.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- CMake (minimum version 3.2)
- C++ compiler (supporting C++11 or later)
- Boost/Beast
- spdlog
- googletest
- nholmman/json
## Getting Started
```
git clone https://github.com/diyooor/cpp-chatbot
cmake .
cmake --build .
./test/bin/test_main.exe
```
## Testing
```
$ ./test/bin/test_main.exe
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from MyTestFixture
[ RUN      ] MyTestFixture.TestHttpClient
[2023-08-26 17:16:46.897] [info] Welcome to spdlog!
[2023-08-26 17:16:46.898] [info] Start of test fixture!
[2023-08-26 17:16:47.401] [info] Response status code: 200
[2023-08-26 17:16:47.402] [info] Response body: 
{
    "access_token":".....",
    "expires_in":5453292,
    "token_type":"bearer"
}
[       OK ] MyTestFixture.TestHttpClient (728 ms)
[----------] 1 test from MyTestFixture (735 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (753 ms total)
[  PASSED  ] 1 test.

```

## Contributing

Contributions are welcome! If you'd like to contribute to this project, feel free to create a pull request.
License

This project is licensed under the MIT License. See the LICENSE file for details.