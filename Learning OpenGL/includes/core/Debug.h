#pragma once

#define ASSERT(x) if (!(x))	__debugbreak();
#define glCall(x) GLClearError();\
					x;\
					ASSERT(GLCheckError(#x, __FILE__, __LINE__))
#include <string>
#include <vector>
#include <fstream>

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

class Debug
{
private:
	Debug();
	Debug(const Debug& o) = delete;
	~Debug();

public:
	static void error(const std::string& s);
	static void warn(const std::string& s);
	static void log(const std::string& s);

	template <class T>
	static void log(const T& s);
	
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

template <class T>
void Debug::log(const T& s) {
	Debug::log(std::to_string(s));
}

