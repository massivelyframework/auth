/**
 * Server
 */

#include "massively_auth/server.hpp"

namespace massively {
namespace auth      {

Server::Server(int argc, char *argv[])
    : m_options(argc, argv)
{
}

Server::~Server() {
}

} // end namespace auth
} // end namsepace massively

