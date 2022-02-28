#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <memory>
#include <optional>

class logger {
public:
    virtual void log(const std::string &str) = 0;
};

class console_logger : public logger {
public:
    void log(const std::string &str) override {
        std::cout << str;
    }
};

class file_logger : public logger {

public:
    explicit file_logger(const std::filesystem::path &path) : ofs_(path) {}

    void log(const std::string &str) override {
        ofs_ << str;
    }

private:
    std::ofstream ofs_;
};

class app {

public:

    explicit app(std::unique_ptr<logger> logger) : logger_(std::move(logger)) {}

    void run() {
        for (int i = 0; i < 10; i++) {
            if (logger_) {
                logger_->log("i=" + std::to_string(i) + "\n");
            }
        }
    }

private:
    std::unique_ptr<logger> logger_;

};

int main() {

    {
        auto logger = std::make_unique<console_logger>();
        app myapp(std::move(logger));
        myapp.run();
    }

    {
        auto logger = std::make_unique<file_logger>("log.txt");
        app myapp(std::move(logger));
        myapp.run();
    }

    return 0;
}
