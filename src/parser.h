// parser.h
#pragma once
#include <vector>
#include <string_view>
#include <optional>
#include "command.h"

std::vector<std::string_view> tokenize(std::string_view input);
std::optional<Command> parse(const std::vector<std::string_view>& tokens);