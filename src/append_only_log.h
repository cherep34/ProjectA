#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

class AppendOnlyLog {
    std::ofstream file_;
    std::string filename_;
    
public:
    explicit AppendOnlyLog(const std::string& filename) 
        : filename_(filename) 
    {
        file_.open(filename, std::ios::app);
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }
    

    ~AppendOnlyLog() noexcept {
        if (file_.is_open()) {
            file_.flush();
            file_.close();
        }
    }
    
    AppendOnlyLog(const AppendOnlyLog&) = delete;
    AppendOnlyLog& operator=(const AppendOnlyLog&) = delete;
    
    AppendOnlyLog(AppendOnlyLog&& other) noexcept
        : file_(std::move(other.file_))
        , filename_(std::move(other.filename_))
    {}
    
    AppendOnlyLog& operator=(AppendOnlyLog&& other) noexcept {
        if (this != &other) {
            if (file_.is_open()) {
                file_.flush();
                file_.close();
            }
            file_ = std::move(other.file_);
            filename_ = std::move(other.filename_);
        }
        return *this;
    }
    

    void log(const std::string& message) {
        if (file_.is_open()) {
            file_ << message << "\n";
            file_.flush(); 
        }
    }
    
    const std::string& filename() const { return filename_; }
};