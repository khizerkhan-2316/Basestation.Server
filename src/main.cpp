#include <iostream>
#include <restinio/all.hpp>
#include "./controllers/SubmarineController.hpp" // Include your SubmarineController header file

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

// Function to configure the RESTinio server for the /api/submarines route
auto server_handler() {
    auto router = std::make_unique<router_t>();
    auto submarine_controller = std::make_shared<SubmarineController>();

    // Handle only the /api/submarines GET request
    router->http_get("/api/submarines", [submarine_controller](const auto &req, auto params) {
        return submarine_controller->on_submarine_info(req, params);
    });

    return router;
}

int main() {
    using namespace std::chrono;

    std::cout << "Server running at http://192.168.0.1:8080" << std::endl;

    try {
        using traits_t = restinio::traits_t<
            restinio::asio_timer_manager_t,
            restinio::single_threaded_ostream_logger_t,
            router_t>;

        restinio::run(
            restinio::on_this_thread<traits_t>()
                .address("192.168.0.1")
                .port(8080)
                .request_handler(server_handler())
                .read_next_http_message_timelimit(10s)
                .write_http_response_timelimit(1s)
                .handle_request_timeout(1s));
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
