/**
 * Options
 */

#ifndef MASSIVELY_AUTH__OPTIONS__H_
#define MASSIVELY_AUTH__OPTIONS__H_

#include <cstdint>

#include <boost/program_options.hpp>

#include "massively_auth/log.hpp"

namespace massively {
namespace auth      {

class Options {
public:
    Options(int argc, char *argv[]);
    ~Options();

    inline uint16_t port() const { return m_port; }

private:
    boost::program_options::variables_map m_options;
    Log m_log;

    uint16_t m_port;
};

} // end namespace auth
} // end namespace massively

#endif // MASSIVELY_AUTH__OPTIONS__H_

