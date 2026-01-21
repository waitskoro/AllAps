#pragma once

#include<atomic>
#include<mutex>

class SequentialIdProvider
{
    static SequentialIdProvider singleton;
    SequentialIdProvider()
        : cur{ 0 }
        , cur_cel{ 0 }
    {}

    std::mutex m_mutex;
    std::atomic<long long> cur;
    std::atomic<long long> cur_cel;

public:
    SequentialIdProvider(const SequentialIdProvider&) = delete;
    SequentialIdProvider&operator=(const SequentialIdProvider&) = delete;

    std::atomic<long long> next();
    std::atomic<long long> next_cel();
    static SequentialIdProvider& get();
};
