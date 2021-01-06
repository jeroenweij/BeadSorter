#include "mock.h"
#include <iostream>
#include <map>
#include <queue>

using valueList = std::queue<int32_t>;
std::map<std::string, valueList> mockMap;

int32_t get_mock_value(std::string function)
{
    const auto& entry = mockMap.find(function);

    if (entry == mockMap.end() || entry->second.empty())
    {
        std::cout << "Could not get mock value for function: " << function << std::endl;
        exit(1);
    }
    int32_t value = entry->second.front();

    entry->second.pop();
    return value;
}

void WillReturn(std::string function, int32_t value)
{
    const auto& entry = mockMap.find(function);

    if (entry == mockMap.end())
    {
        const auto result = mockMap.emplace(std::make_pair(function, valueList()));
        if (!result.second)
        {
            std::cout << "Map insert failed" << std::endl;
            exit(1);
        }

        result.first->second.push(value);
    }
    else
    {
        entry->second.push(value);
    }
}

void VerifyExpectations()
{
    for (const auto& entry : mockMap){
        if (!entry.second.empty()){
            std::cout << "Verify failed, found expectation for " << entry.first << std::endl;
            exit(1);
        }
    }
}
