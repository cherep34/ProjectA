#pragma once

#include <vector>
#include <string_view>
#include "command.h"
#include "errors.h" 

std::vector<std::string_view> tokenize(std::string_view input);

Command parse(const std::vector<std::string_view>& tokens);