#pragma once

#include <iomanip>
#include <iostream>

#define ENABLE_PRINT_TIMING

using namespace std::chrono_literals;
using namespace std::string_literals;

namespace ts::utils
{

void log(const std::string& msg)
{
    std::cout << "\n[ :: " << msg << " :: ]\n" <<std::endl;
}

void log_test(const std::string& msg, unsigned line_width = 40)
{
    std::cout << "\n"
        << "[ " << std::setfill(':') << ':' << std::setw(line_width) << " ]"
        << "\n[ :: " << std::setfill(' ') << std::setw(line_width-7) << msg << " :: ]\n"
        << "[ " << std::setfill(':') << ':' << std::setw(line_width) << " ]"
        << std::endl;
}

auto check_scheduled = [](ts::task_scheduler& s, unsigned num_tasks)
{
    for(auto i = 1; i <= num_tasks; ++i)
    {
        const auto task_id = "task_id_"s + std::to_string(i);
        std::cout << std::boolalpha << task_id << " scheduled: " << s.is_scheduled(task_id) << std::endl;
    }
    std::cout << "\n" << std::endl;
};

auto check_enabled = [](ts::task_scheduler& s, unsigned num_tasks)
{
    for(auto i = 1; i <= num_tasks; ++i)
    {
        const auto task_id = "task_id_"s + std::to_string(i);
        std::cout << std::boolalpha << task_id << " enabled: " << s.is_enabled(task_id) << std::endl;
    }
    std::cout << "\n" << std::endl;
};

auto check_tasks = [](const std::string& msg, ts::task_scheduler& s, unsigned num_tasks, std::chrono::seconds wait_time = 5s)
{
    std::this_thread::sleep_for(wait_time);
    log(msg);
    check_scheduled(s, num_tasks);
    check_enabled(s, num_tasks);
};

class timer
{
public:
    explicit timer() { start(); }
    ~timer() { stop(); }

protected:
    void start()
    {
        #if !defined(ENABLE_PRINT_TIMING)
            return;
        #endif

        is_running = true;
        timer_thread = std::thread([this]
        {
            unsigned i = 0; 
            while(is_running) 
            {
                std::cout << "." << i++ << std::endl;
                std::this_thread::sleep_for(1s);
            }
        });
    }

    void stop()
    {
        #if !defined(ENABLE_PRINT_TIMING)
            return;
        #endif

        if (!is_running)
            return;

        is_running.store(false);        
        if (timer_thread.joinable())
            timer_thread.join();
    }

private:
    std::atomic_bool is_running;
    std::thread timer_thread;
};

}

/*
// Debug Helpers:

template<class TupType, size_t... I>
void print_tuple(const TupType& _tup, std::index_sequence<I...>)
{
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

template<class... T>
void print_tuple(const std::tuple<T...>& _tup)
{
    print_tuple(_tup, std::make_index_sequence<sizeof...(T)>());
}
*/