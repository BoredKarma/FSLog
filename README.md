# FSLog: Simple Logging Library for C++

FSLog is a simple, lightweight, and easy-to-use logging library for C++. It provides basic logging capabilities with different log levels and colored console output. The library is designed to be straightforward and not overly customizable.

## Features

- **Okay Compatibility**: FSLog is compatible with:
    - **Platform**: Windows, Linux.
    - **Compiler**: Visual Studio, GCC, Clang.
- **Different log levels**: FSLog supports different log levels including debug, info, warn, and error.
- **Colored console output**: Make your logs more readable and distinguishable with different colors for each log level. Using virtual terminal for windows.
- **Cool syntax**: FSLog library provides a simple, Rust-like syntax for logging.

## Usage

To use FSLog in your project, simply include the `fslog.h` header file in your source code.

```cpp
#include "fslog.h"
```

Below is a simple example of the 4 different logging levels:

```cpp
fslog::debug("This is a debug log message"); // example: "[12:36:42] [DEBUG] This is a debug log message"
fslog::info("Hello! I am {} and I am {} years old", "John", 25);
fslog::warn("The user has commited murder {} times", 2.5);
fslog::error("There was an error doing {}", "something");
```

Below is how you can have debug information(file, line number) in your logs:

```cpp
fslog::debug(FS_META, "This is a debug log message"); // example: "[12:36:42] [DEBUG] [main.cpp:21] This is a debug log message"
```

Below is how you can use fslog as a kind of checkpoint, printing the current location in your code:

```cpp
fslog::debug(FS_POINT); // example: "[12:36:42] [DEBUG] [main.cpp:21] int main(int, char**)"
```

## Credits

[Me](https://github.com/BoredKarma): everything
