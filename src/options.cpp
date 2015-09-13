/**
 * Options
 */

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/blank.hpp>
#include <boost/program_options.hpp>

#include "massively_auth/options.hpp"

namespace massively {
namespace auth      {

namespace { // anonymous namespace

template <typename T = boost::blank>
struct Option {
    Option(char const * long_opt,
           char         short_opt,
           char const * description,
           T            default_value)
        : long_opt(long_opt)
        , short_opt(short_opt)
        , description(description)
        , default_value(default_value)
    { }

    Option(char const * long_opt,
           char         short_opt,
           char const * description)
        : long_opt(long_opt)
        , short_opt(short_opt)
        , description(description)
        , default_value()
    { }

    char const * long_opt;
    char         short_opt;
    char const * description;
    T            default_value;

    auto name() const
    {
        return std::string(long_opt) + ',' + short_opt;
    }
};

char const LOG_TOPIC[]          = "server.options";

Option<>         const OPTION_CONFIG = { "config", 'c', "configuration file" };
Option<>         const OPTION_HELP   = { "help",   'h', "print help message" };
Option<uint16_t> const OPTION_PORT   = { "port",   'p', "port", 9001 };

void print_help_and_exit(
        boost::program_options::options_description const & desc,
        bool error = true)
{
    std::ostream & out = error ? std::cerr : std::cout;
    int const status_code = error ? EXIT_FAILURE : EXIT_SUCCESS;

    out << desc << std::endl;
    std::exit(status_code);
}

} // end anonymous namespace

Options::Options(int argc, char *argv[])
    : m_options()
    , m_log(LOG_TOPIC)
    , m_port(OPTION_PORT.default_value)
{
    namespace po = boost::program_options;

    po::options_description generic_options("Generic options");
    generic_options.add_options()
        (OPTION_PORT.name().c_str(),
         po::value<uint16_t>()->default_value(OPTION_PORT.default_value),
         OPTION_PORT.description);

    po::options_description command_line_options("Command line options");
    command_line_options.add_options()
        (OPTION_HELP.name().c_str(), OPTION_HELP.description)
        (OPTION_CONFIG.name().c_str(), po::value<std::string>(), OPTION_CONFIG.description);
    command_line_options.add(generic_options);

    try {
        po::store(po::parse_command_line(argc, argv, command_line_options), m_options);

        if (m_options.count(OPTION_HELP.long_opt)) {
            print_help_and_exit(command_line_options, false);
        }

        if (m_options.count(OPTION_CONFIG.long_opt)) {
            auto config_filename = m_options[OPTION_CONFIG.long_opt].as<std::string>();
            m_log.info("Loading config from file: {}", config_filename);
            std::ifstream config_file(config_filename);
            if (!config_file) {
                m_log.error("Could not open config file: {}", config_filename);
                throw std::runtime_error("Could not open file.");
            }
            po::store(po::parse_config_file(config_file, generic_options), m_options);
        }

        po::notify(m_options);

        m_port = m_options[OPTION_PORT.long_opt].as<uint16_t>();
        m_log.info("Port: {}", m_port);
    }
    catch(const po::unknown_option &e) {
        std::cerr << "Unknown option: " << e.get_option_name() << std::endl;
        print_help_and_exit(command_line_options);
    }
    catch(std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        print_help_and_exit(command_line_options);
    }
}

Options::~Options() { }

} // end namespace auth
} // end namsepace massively

