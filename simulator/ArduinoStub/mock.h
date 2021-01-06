#pragma once

#include <string>

#define RETURN_MOCK return get_mock_value(__func__)

int32_t get_mock_value(std::string function);
void WillReturn(std::string function, int32_t value);
void VerifyExpectations();
