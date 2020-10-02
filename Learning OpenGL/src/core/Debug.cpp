#define _CRT_SECURE_NO_WARNINGS
#include "Debug.h"
#include <chrono>
#include <GL/glew.h>
#include <iostream>
#include <ctime>
#include <Windows.h>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL error]\t(" << error << ")\t@\t" << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

// ******************** Debug Class ********************
namespace Shado {
	Debug* Debug::singleton = new Debug();

	Debug::Debug()
		: stream(std::cout.rdbuf())
	{
	}

	Debug::~Debug() {
	}

	void Debug::error(std::string format, std::vector<std::string> args) {
		std::string result;

		bool open = false;
		int i = 0;
		for (char c : format) {

			if (c == '{' || c == '}') {
				open = !open;
				continue;
			}

			if (open) {
				result += args[i];
				i++;
				continue;
			}

			result += c;
		}

		Debug::error(result);
	}

	void Debug::error(const std::string& s) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::string date = std::ctime(&end_time);
		date = date.substr(0, date.length() - 1);

		std::string message = date + " [ERROR] " + s;

		get().buffer.push_back(message);

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << date;

		SetConsoleTextAttribute(hConsole, 4);
		get().stream << " [ERROR] ";

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << s << std::endl;
	}

	void Debug::warn(std::string format, std::vector<std::string> args) {
		std::string result;

		bool open = false;
		int i = 0;
		for (char c : format) {

			if (c == '{' || c == '}') {
				open = !open;
				continue;
			}

			if (open) {
				result += args[i];
				i++;
				continue;
			}

			result += c;
		}

		Debug::warn(result);
	}

	void Debug::warn(const std::string& s) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::string date = std::ctime(&end_time);
		date = date.substr(0, date.length() - 1);

		std::string message = date + " [WARNING] " + s;

		get().buffer.push_back(message);

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << date;

		SetConsoleTextAttribute(hConsole, 6);
		get().stream << " [WARNING] ";

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << s << std::endl;
	}

	void Debug::log(const std::string& s) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::string date = std::ctime(&end_time);
		date = date.substr(0, date.length() - 1);

		std::string message = date + " [INFO] " + s;

		get().buffer.push_back(message);

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << date;

		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		get().stream << " [INFO] ";

		SetConsoleTextAttribute(hConsole, 7);
		get().stream << s << std::endl;
	}

	void Debug::log(std::string format, std::vector<std::string> args) {

		std::string result;

		bool open = false;
		int i = 0;
		for (char c : format) {

			if (c == '{' || c == '}') {
				open = !open;
				continue;
			}

			if (open) {
				result += args[i];
				i++;
				continue;
			}

			result += c;
		}

		Debug::log(result);
	}

	void Debug::Assert(bool condition, const std::string& message) {
		if (!condition) {
			Debug::error("Assertion Failed!\t" + message);
			//__debugbreak();
		}
	}

	void Debug::dumpAndClear() {
		dump();
		get().buffer.clear();
	}

	void Debug::dump() {
		auto& _b = get().buffer;
		for (const std::string& s : _b)
			get().stream << s << std::endl;
	}

	void Debug::setStream(std::ostream& stream) {
		get().stream.rdbuf(stream.rdbuf());
	}

	Debug& Debug::get() {
		return *singleton;
	}

	void Debug::destroy() {
		delete singleton;
	}
}
