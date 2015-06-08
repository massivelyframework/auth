/**
 * Server
 */

#ifndef MASSIVELY_AUTH__SERVER__H_
#define MASSIVELY_AUTH__SERVER__H_

#include "massively_auth/options.hpp"

namespace massively {
namespace auth      {

class Server {
public:
    Server(int argc, char *argv[]);
    ~Server();
private:
    Options m_options;
};

} // end namespace auth
} // end namsepace massively

#endif // MASSIVELY_AUTH__SERVER__H_

