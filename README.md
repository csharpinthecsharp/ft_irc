*This project has been created as part of the 42 curriculum by njung, ltrillar.*

## Description

This project is an IRC (Internet Relay Chat) server implemented in C++98. It is a school project for 42.

The server can handle multiple clients concurrently and supports basic IRC features like creating channels, sending private messages, and using server commands. The project structure includes classes for the Server, Client, Channel, and Message handling.

There is also a `WeatherBot.py` in the repository, which seems to be a bot that can connect to the server.

## Instructions

### Compilation

To compile the server, simply run `make` at the root of the repository:

```bash
make
```

This will create an executable named `ircserv`.

### Execution

To run the server, execute the compiled binary with a port number and a password:

```bash
./ircserv <port> <password>
```

For example:

```bash
./ircserv 6667 mypassword
```

### Cleaning

To remove the compiled object files, run:

```bash
make clean
```

To remove the object files and the executable, run:

```bash
make fclean
```

To recompile the project from scratch, run:

```bash
make re
```

## Resources

Here are some resources that are useful for understanding the IRC protocol:

*   [RFC 2812 - Internet Relay Chat: Client Protocol](https://tools.ietf.org/html/rfc2812)
*   [IRC Tutorial](https://www.irchelp.org/irchelp/irctutorial.html)
*   [A short IRC tutorial](https://www.javatpoint.com/irc)

### AI Usage

AI (specifically GitHub Copilot) was used for the following tasks in this project:

*   Generating this `README.md` file based on the project structure and user requirements.
*   Assisting with boilerplate code and simple function implementations.
*   Providing suggestions for debugging and code optimization.
