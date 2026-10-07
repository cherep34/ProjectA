#pragma once

#include <vector>
#include <string_view>
#include <expected>  
#include "command.h"
#include "errors.h"

std::vector<std::string_view> tokenize(std::string_view input);

//ParseError
Command parse(const std::vector<std::string_view>& tokens);

//std::expected
std::expected<Command, ParseError> parse_expected(const std::vector<std::string_view>& tokens);