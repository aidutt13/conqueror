#pragma once

#include <thread>

struct Thread {

	const std::unique_ptr<std::thread> operator=(std::thread& th) {
		m_thread = th;
		return m_thread;
	}
};