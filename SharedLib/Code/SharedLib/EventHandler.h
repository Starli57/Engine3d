#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

template<typename... Args>	
class EventHandler
{
public:
	using HandlerType = std::function<void(Args...)>;

    void AddHandler(const HandlerType& handler) 
    {
        handlers.push_back(handler);
    }

    void RemoveHandler(const HandlerType& handler)
    {
        auto it = std::remove_if(handlers.begin(), handlers.end(),
            [&](const HandlerType& h) { return h == handler; });

        handlers.erase(it, handlers.end());
    }

    void Invoke(Args... args) const 
    {
        for (const auto& handler : handlers)
        {
            handler(args...);
        }
    }

private:
	std::vector<HandlerType> handlers;
};

