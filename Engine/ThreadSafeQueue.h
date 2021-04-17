#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue()
		: m_Queue{}
		, m_Mutex{}
	{};
	~ThreadSafeQueue()
	{};

	void Push(T& t)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		m_Queue.push(t);
	}

	T Pop()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (m_Queue.empty())
			return T{};

		T message = m_Queue.front();
		m_Queue.pop();
		return message;
	}

	bool IsEmpty() const
	{
		return m_Queue.empty();
	}

private:
	std::queue<T> m_Queue;
	std::mutex m_Mutex;
};