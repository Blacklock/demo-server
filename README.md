# Demo Server

Demo Server is a multiplayer game server written in C++. The server uses a custom network protocol built on top of UDP, that implements multiple features such as a snapshot system, reliable packets, and packet fragmentation.

## Usage

**Setup (Windows)**

1. Compile using [Visual Studio 2017](https://visualstudio.microsoft.com)
2. Build the solution with `Build -> Build Solution`
3. Run the Google Test unit tests with `Test -> Run -> All Tests`
4. Build & run `DemoServer.exe` to launch the server. A custom Unity game client is being used to interact with the server.

## Roadmap

Check the [project board](https://github.com/Blacklock/demo-server/projects/1) for the features roadmap.
