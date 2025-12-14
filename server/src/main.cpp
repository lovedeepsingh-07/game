#include <chrono>
#include <common/common.hpp>
#include <common/constants.hpp>
#include <common/error.hpp>
#include <net.h>
#include <thread>

constexpr int delta_time_ms = 1000 / constants::TARGET_FPS;
constexpr int port = 9090;

int main() {
    try {
        net::server::setup((uint16_t)port);
    } catch (rust::Error e) {
        common::error(fmt::format(
            "failed to setup the server, {}", error::Error::from_rust(e).to_string()
        ));
        std::exit(1);
    }

    common::info("Listening for incoming connections on :9090");

    while (true) {
        try {
            net::server::update(delta_time_ms);
        } catch (rust::Error e) {
            common::error(fmt::format(
                "failed to update server, {}", error::Error::from_rust(e).to_string()
            ));
            break;
        }

        try {
            net::server::send_packets();
        } catch (rust::Error e) {
            common::error(fmt::format(
                "failed to send packets, {}", error::Error::from_rust(e).to_string()
            ));
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delta_time_ms));
    }
    return 0;
}
