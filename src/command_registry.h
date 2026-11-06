#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <charconv>
#include <type_traits>
#include <system_error>
#include <concepts>
#include "function_traits.h"


template<typename T>
concept ConvertibleArg = std::is_same_v<T, std::string_view> || 
                         std::is_same_v<T, std::string> || 
                         std::is_arithmetic_v<T>;


template<typename F>
concept CommandHandler = requires {
    typename function_traits<F>::return_type;
} && []<size_t... Is>(std::index_sequence<Is...>) {
    return (ConvertibleArg<typename function_traits<F>::template arg_type<Is>> && ...);
}(std::make_index_sequence<function_arity_v<F>>{});


template<typename T>
T convert(std::string_view sv) {
    if constexpr (std::is_same_v<T, std::string_view>) {
        return sv;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return std::string(sv);
    } else if constexpr (std::is_arithmetic_v<T>) {
        T result{};
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);
        if (ec != std::errc{}) {
            throw std::runtime_error("Cannot convert '" + std::string(sv) + "' to number");
        }
        return result;
    } else {
        static_assert(sizeof(T) == 0, 
            "Unsupported type for convert<T>. Only string_view, string, and arithmetic types are supported.");
    }
}


class CommandRegistry {
    using Handler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, Handler> handlers_;
    
public:
        template<CommandHandler F>
        void register_command(const std::string& name, F handler) {
            constexpr size_t arity = function_arity_v<F>;
            
            handlers_.insert_or_assign(name, [handler, arity](const std::vector<std::string>& args) {
                if (args.size() != arity) {
                    throw std::runtime_error(
                        "Command expects " + std::to_string(arity) + 
                        " argument(s), got " + std::to_string(args.size()));
                }
                call_with_args(handler, args, std::make_index_sequence<arity>{});
            }); 
        }
    
    void execute(const std::string& name, const std::vector<std::string>& args) {
        auto it = handlers_.find(name);
        if (it == handlers_.end()) {
            throw std::runtime_error("Unknown command: " + name);
        }
        it->second(args);
    }
    
    bool has_command(const std::string& name) const {
        return handlers_.count(name) > 0;
    }
    
    size_t size() const { return handlers_.size(); }
    
private:
    template<typename F, size_t... Is>
    static void call_with_args(F handler, const std::vector<std::string>& args, 
                               std::index_sequence<Is...>) {
        handler(convert<function_arg_t<F, Is>>(args[Is])...);
    }
};