#pragma once

#ifndef UTIL_H
#define UTIL_H
#include <functional>
#include <string>
#include <thread>


#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct Color
{
	Color(float r, float g, float b, float a);
	Color(float r, float g, float b);
	Color(const Color& c);
	Color();
	~Color();

	Color& operator=(const Color& other);
	explicit operator glm::vec4();
	explicit operator glm::vec3();

	static Color fromRGBA(int r, int g, int b, int a);
	static Color fromRGB(int r, int g, int b);
	static Color fromHex(const std::string& hexColor);
	static Color fromHex(long hexColor);
	static Color random();

	const float& red()		const { return _red;	}
	const float& green()	const { return _green;	}
	const float& blue()		const { return _blue;	}
	const float& alpha()	const { return _alpha;	}

private:
	static float validateInput(float input, float min, float max);

private:
	float _red;
	float _green;
	float _blue;
	float _alpha;
};

struct IntervalObject {

	IntervalObject(std::thread* t, bool* flag)
		: id(rand() % UINT64_MAX), thread(t), executing(flag)
	{}

	void terminate() {
		*executing = false;
	}

private:
	const uint64_t id;
	std::thread* thread;
	bool* executing;
};

IntervalObject setInterval(std::function<void()> task, unsigned long intervalMS);

struct TimeoutObject {

	TimeoutObject(std::thread* t, bool* flag)
		: id(rand() % UINT64_MAX), thread(t), executing(flag)
	{}

	void cancel() {
		*executing = false;
	}

private:
	const uint64_t id;
	std::thread* thread;
	bool* executing;
};
TimeoutObject setTimeout(std::function<void()> task, unsigned long startAfterMS);

#endif


