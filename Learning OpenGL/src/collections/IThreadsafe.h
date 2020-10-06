#pragma once
#include <mutex>

namespace Shado {
	class IThreadSafe {


	protected:
		std::mutex mutex;
	};
	
}
