#pragma once

#include <stdexcept>
#include <string>

class KvError : public std::runtime_error {
public:
    explicit KvError(const std::string& msg) : std::runtime_error(msg) {}
};

class ParseError : public KvError {
public:
    explicit ParseError(const std::string& msg) : KvError("Parse error: " + msg) {}
};

class TypeMismatchError : public KvError {
    std::string key_;
    std::string expectedType_;  
    std::string actualType_;
public:
    TypeMismatchError(const std::string& key, const std::string& expected, const std::string& actual)
        : KvError("Type mismatch for key '" + key + "': expected " + expected + ", got " + actual),
          key_(key), expectedType_(expected), actualType_(actual) {}
    const std::string& key() const { return key_; }
    const std::string& expectedType() const { return expectedType_; }
    const std::string& actualType() const { return actualType_; }
};

class KeyNotFoundError : public KvError {
    std::string key_;
public:
    explicit KeyNotFoundError(const std::string& key) : KvError("KeyNotFoundError: key - " + key), key_(key) {}
    const std::string& key() const { return key_; }
};

class OutOfMemoryError : public KvError {
public:
    explicit OutOfMemoryError(const std::string& msg = "Out of memory") : KvError(msg) {}
};