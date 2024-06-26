
#include <filesystem>
#include <fstream>
#include <sstream>
#include <print>
#include "Lexer.hpp"
#include "Error.hpp"


void run(std::string const& source)
{
    auto lexer = Lexer(source);
    auto const tokens = lexer.scan_tokens();

    for (auto const& token : tokens)
    {
        std::cout << token << '\n';
    }
}

void run_file(std::filesystem::path const& path)
{
    auto const file = std::ifstream{path};
    if (not file)
    {
        std::cerr << "file not existing: " << path << "\n";
        return;
    }
    auto stream = std::ostringstream{};
    stream << file.rdbuf();
    if (not stream)
    {
        std::cerr << "unable to read file" << path << "\n";
        return;
    }

    run(std::move(stream).str());
    if (had_error()) { std::exit(65); }
}

void run_prompt()
{
    for (;;)
    {
        std::cout << "> ";
        auto const line = []() -> std::string
        {
            std::string l{};
            std::getline(std::cin, l);
            if (not std::cin) { throw std::runtime_error("error while reading console"); }
            return l;
        }();

        run(line);
        reset_error();
    }
}

int main(int const argc, char const* const * const argv)
{
    if (argc > 2)
    {
        std::cout << "usage: jlox [script]\n";
        return 64;
    }
    else if (argc == 2)
    {
        run_file(argv[1]);
    }
    else
    {
        run_prompt();
    }
}
