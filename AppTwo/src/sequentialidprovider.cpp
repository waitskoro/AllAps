#include "sequentialidprovider.h"

SequentialIdProvider SequentialIdProvider::singleton;

SequentialIdProvider &SequentialIdProvider::get()
{
    return singleton;
}

std::atomic<long long> SequentialIdProvider::next()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return cur++;
}

std::atomic<long long> SequentialIdProvider::next_cel()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return cur_cel++;
}
