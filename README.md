# Demo Server

Demo Server is a multiplayer game server written in C++. The server uses a custom network protocol built on top of UDP, that implements multiple features such as a snapshot system, reliable packets, and packet fragmentation.

## Usage

**Setup (Windows)**

1. Compile using [Visual Studio 2017](https://visualstudio.microsoft.com)
2. Build the solution with `Build -> Build Solution`
3. Run the Google Test unit tests with `Test -> Run -> All Tests`
4. Build & run `DemoServer.exe` to launch the server. A custom Unity game client is being used to interact with the server.

## Config

Located in [settings.xml](settings.xml).

| Variable               | Description                                                             |
|------------------------|-------------------------------------------------------------------------|
| GameServer/Port        | Specifies the port that the server will be listening on.                |
| GameServer/Protocol    | The game's protocol ID that clients use to connect.                     |
| Game/Debug             | Set to on/off. Enabling will print logs in the console.                 |
| Game/MaxConnections    | Maximum amount of connections per game instance.                        |
| Client/PacketLossCount | Amount of packets to use when calculating outgoing packet loss.         |
| Client/Timeout         | Time (ms) to wait before timing out the client. Set to 0 to disable.    |
| Client/ReliableResend  | Time (ms) to wait before resending a reliable message.                  |
| Logger/Severity        | Set to none, fatal, error, warning, info (default), debug, or verbose.  |
| Logger/LogFile         | Name of the log file.                                                   |
| NetSim/InLoss          | Percentage of incoming packets to skip.                                 |
| NetSim/OutLoss         | Percentage of outgoing packets to skip.                                 |

## Roadmap

Check the [project board](https://github.com/Blacklock/demo-server/projects/1) for the features roadmap.
