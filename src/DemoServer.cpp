// DemoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameServer.h"
#include "Config.h"

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <iostream>


namespace plog {
	static std::unordered_map<std::string, Severity> const log_levels = { // ...severityFromString function?
		{"none", Severity::none},
		{"fatal", Severity::fatal},
		{"error", Severity::error},
		{"warning", Severity::warning},
		{"info", Severity::info},
		{"debug", Severity::debug},
		{"verbose", Severity::verbose}
	};

	class ConsoleFormatter {
	public:
		static util::nstring header() {
			return util::nstring();
		}

		static util::nstring format(const Record& record) {
			util::nstringstream ss;
			ss << "[" << record.getFunc() << "@" << record.getLine() << "] ";
			ss << record.getMessage() << "\n";
			return ss.str();
		}
	};
}

int main()
{
	// Config
	config::load_config("settings.xml");

	// Logger
	auto it = plog::log_levels.find(config::SEVERITY);
	plog::Severity max_severity = it != plog::log_levels.end() ? it->second : plog::Severity::info;
	static plog::ColorConsoleAppender<plog::ConsoleFormatter> consoleAppender;
	plog::init(max_severity, config::LOG_FILE.c_str()).addAppender(&consoleAppender);

	// Server
	GameServer server(config::GAME_PORT);
	
	/*std::cout << "Press any key to close the program.\n";
	std::cin.get();
	std::cin.get();*/
}
