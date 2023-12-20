#pragma once

#include "win.h"
#include <map>

struct lambda_function {
	struct Function_t {
		virtual ~Function_t() = default;
	};

	template<typename ...A>
	struct Callback_t : Function_t {
		using Callback = std::function<void(A...)>;
		Callback Cb;

		Callback_t(Callback p_callback) : Cb(p_callback) {}
	};

	template<typename ...A>
	void Call(std::string Name, A&& ... Args)
	{
		using Function = Callback_t<A...>;
		using Callback = std::function<void(A...)>;

		const Function_t& Base = *Callbacks[Name];
		const Callback& Fn = static_cast<const Function&>(Base).Cb;

		Fn(std::forward<A>(Args)...);
	}

	template<typename ...Type, typename Function>
	void Add(std::string Name, Function Lambda)
	{
		using Callback = Callback_t<Type...>;
		Callbacks[Name] = std::unique_ptr<Callback>(new Callback(Lambda));
	}

	void Remove(std::string Name)
	{
		auto it = Callbacks.find(Name);
		Callbacks.erase(it);
	}

	void Clear()
	{
		Callbacks.clear();
	}

private:
	std::map<std::string, std::unique_ptr<Function_t>> Callbacks;
};