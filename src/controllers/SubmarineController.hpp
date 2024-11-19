#ifndef SUBMARINE_CONTROLLER_HPP
#define SUBMARINE_CONTROLLER_HPP

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

class SubmarineController {
public:
    SubmarineController() = default;

    // Handler for the `/api/submarines` route
auto on_submarine_info(const restinio::request_handle_t &req, const restinio::router::route_params_t &params) const {
        auto resp = req->create_response();

        // Set CORS and Content-Type headers
        resp.append_header("Content-Type", "application/json; charset=utf-8")
            .append_header("Access-Control-Allow-Origin", "*")
            .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

        // JSON response body
        std::string json_body = R"({"message": "Submarine API is working!"})";
        resp.set_body(json_body);

        return resp.done();
    }
};

#endif // SUBMARINE_CONTROLLER_HPP
