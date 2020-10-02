#pragma once

#define ASSERT(x) if (!(x))	__debugbreak();
#define glCall(x) GLClearError();\
					x;\
					ASSERT(GLCheckError(#x, __FILE__, __LINE__))
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <variant>

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

namespace Shado {
	class Debug
	{
	private:
		Debug();
		Debug(const Debug& o) = delete;
		~Debug();

	public:
		static void error(std::string format, std::vector<std::string> args);
		static void error(const std::string& s);
		static void warn(std::string format, std::vector<std::string> args);
		static void warn(const std::string& s);
		static void log(const std::string& s);
		static void log(std::string format, std::vector<std::string> args);

		static void Assert(bool condition, const std::string& message);

		static void dumpAndClear();
		static void dump();

		static void setStream(std::ostream& stream);
	
		static Debug& get();
		static void destroy();

	private:
		static Debug* singleton;

		std::vector<std::string> buffer;
		std::ostream stream;
	};
}
