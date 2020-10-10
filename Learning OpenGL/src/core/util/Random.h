#pragma once
#include <random>

namespace Shado {

	class Random
	{
	public:
		static void init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Float(float min, float max)
		{
			return Float() * (max - min) + min;
		}

		static double Double()
		{
			return (double)s_Distribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max();
		}

		static double Double(double min, double max)
		{
			return Double() * (max - min) + min;
		}

		static int Int()
		{
			return int(s_Distribution(s_RandomEngine) / std::numeric_limits<uint32_t>::max());
		}

		static int Int(int min, int max)
		{
			return int(Double() * double(max - min) + (double)min);
		}
		
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
	
}
