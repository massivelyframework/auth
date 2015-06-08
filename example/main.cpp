/**
 * Example authentication server
 */

#include <cstdlib>

#include <massively_auth/server.hpp>

int main(int argc, char *argv[])
{
    massively::auth::Server server(argc, argv);

    return EXIT_SUCCESS;
}

