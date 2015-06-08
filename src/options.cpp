/**
 * Options
 */

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "massively_auth/options.hpp"

namespace massively {
namespace auth      {

namespace { // anonymous namespace

char     const LOG_TOPIC[]          = "server.options";
char     const OPTION_HELP[]        = "help";
char     const OPTION_HELP_SHORT    = 'h';
char     const OPTION_HELP_DESC[]   = "print help message";
char     const OPTION_CONFIG[]      = "config";
char     const OPTION_CONFIG_SHORT  = 'c';
char     const OPTION_CONFIG_DESC[] = "configuration file";
char     const OPTION_PORT[]        = "port";
char     const OPTION_PORT_SHORT    = 'p';
uint16_t const OPTION_PORT_DEFAULT  = 9001;

auto option_name(char const * long_opt,
                 char         short_opt)
{
    return std::string(long_opt) + ',' + short_opt;
}

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
    , m_port(OPTION_PORT_DEFAULT)
{
    namespace po = boost::program_options;

    po::options_description generic_options("Generic options");
    generic_options.add_options()
        (option_name(OPTION_PORT, OPTION_PORT_SHORT).c_str(),
         po::value<uint16_t>()->default_value(OPTION_PORT_DEFAULT), OPTION_PORT);

    po::options_description command_line_options("Command line options");
    command_line_options.add_options()
        (option_name(OPTION_HELP, OPTION_HELP_SHORT).c_str(), OPTION_HELP_DESC)
        (option_name(OPTION_CONFIG, OPTION_CONFIG_SHORT).c_str(), po::value<std::string>(), OPTION_CONFIG_DESC);
    command_line_options.add(generic_options);

    try {
        po::store(po::parse_command_line(argc, argv, command_line_options), m_options);

        if (m_options.count(OPTION_HELP)) {
            print_help_and_exit(command_line_options, false);
        }

        if (m_options.count(OPTION_CONFIG)) {
            auto config_filename = m_options[OPTION_CONFIG].as<std::string>();
            m_log.info("Loading from config file: {}", config_filename);
            std::ifstream config_file(config_filename);
            po::store(po::parse_config_file(config_file, generic_options), m_options);
        }

        po::notify(m_options);

        m_port = m_options[OPTION_PORT].as<uint16_t>();
        m_log.info("Port: {}", m_port);
    }
    catch(const po::unknown_option &e) {
        std::cerr << "Unknown option: " << e.get_option_name() << std::endl;
        print_help_and_exit(command_line_options);
    }
}

Options::~Options() {
}

} // end namespace auth
} // end namsepace massively

