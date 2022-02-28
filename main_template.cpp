#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <memory>
#include <optional>

class noop_logger {
    void log(const std::string &) {}
};

class console_logger {
public:
    void log(const std::string &str) {
        std::cout << str;
    }
};

class file_logger {

public:
    explicit file_logger(const std::filesystem::path &path) : ofs_(path) {}

    void log(const std::string &str) {
        ofs_ << str;
    }

private:
    std::ofstream ofs_;
};

template<class Logger>
class app {

public:

    explicit app(Logger logger = noop_logger()) : logger_(std::move(logger)) {}

    void run() {
        for (int i = 0; i < 10; i++) {
            logger_.log("i=" + std::to_string(i) + "\n");
        }
    }

private:
    Logger logger_;

};

int main() {

    {
        console_logger logger;
        app myapp(logger);
        myapp.run();
    }

    {
        file_logger logger("log_template.txt");
        app myapp(std::move(logger));
        myapp.run();
    }

    return 0;
}
