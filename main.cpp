
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
	const std::string main_file_contents(R"(
#include <type_traits>
#include <exception>
#include <iostream>
#include <utility>
#include <chrono>
#include <thread>
namespace
{
	template<typename F, typename... Args>
	decltype(auto) runtime(F&& f, Args&&... args)
	{
		if constexpr (std::is_same_v<std::invoke_result_t<F, Args...>, void>)
		{
			auto ti{ std::chrono::high_resolution_clock::now() };
			std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
			auto tf{ std::chrono::high_resolution_clock::now() };
			return std::chrono::duration<double>(tf - ti).count();
		}
		if constexpr (!std::is_same_v<std::invoke_result_t<F, Args...>, void>)
		{
			auto ti{ std::chrono::high_resolution_clock::now() };
			auto retval{ std::invoke(std::forward<F>(f), std::forward<Args>(args)...) };
			auto tf{ std::chrono::high_resolution_clock::now() };
			return std::make_pair(std::move(retval), std::chrono::duration<double>(tf - ti).count());
		}
	}
}
int main()
{
	auto countdown = [](size_t nsec)
		{
			std::cout << "\n\ncountdown: T-";
			for (auto i{ static_cast<int>(nsec) }; i >= 0; --i)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << i << ' ';
			}
		};
	try
	{
		
		return 0;
	}
	catch (const std::exception& xxx)
	{
		std::cerr << xxx.what() << std::endl;
		return 1;
	}
}
)"
	);
	try
	{
		std::ofstream main_output_stream{};		
		main_output_stream.open("main.cpp", std::ios::out);
		if (!main_output_stream.is_open())
			throw std::runtime_error("ERROR: Cannot open main.cpp file.");		
		main_output_stream << main_file_contents;
		main_output_stream.flush();
		main_output_stream.close();

		return 0;
	}
	catch (const std::exception& xxx)
	{
		std::cerr << xxx.what() << std::endl;
		return 1;
	}
}