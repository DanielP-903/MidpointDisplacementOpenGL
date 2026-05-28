#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H

#include <functional>
#include <vector>
#include <algorithm>

template<typename ... Args>
class Event {
public:
	using Listener = std::function<void(Args ...)>;

	Event& operator+=(const Listener& listener) 
	{
		listeners.emplace_back(listener);
		return *this;
	}

	template<typename T>
	Event& operator+=(std::pair<T*, void (T::*)(Args...)> bound)
	{
		auto [instance, method] = bound;
		listeners.emplace_back([=](Args... args) { (instance->*method)(args...); });
		return *this;
	}


	// TODO: find a better way of doing this
	//Event& operator-=(const Listener& listener) 
	//{
	//	listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
	//	return *this;
	//}

	void Invoke(Args ... args)
	{
		for (auto& func : listeners)
		{
			func(args ...);
		}
	}


private:
	std::vector<Listener> listeners;
};

template<typename T>
auto Bind(T* instance, void (T::* method)())
{
	return [instance, method]() { (instance->*method)(); };
}


#endif