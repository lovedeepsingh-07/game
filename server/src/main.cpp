#include <common/common.hpp>

int main() {
    net::setup_server();
    net::update_server(16);
    return 0;
}
