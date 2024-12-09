#include <iostream>
#include <restinio/all.hpp>
#include <restinio/websocket/websocket.hpp>

#include "./controllers/MeasurementController.hpp"  // Include your SubmarineController header file
#include "./dataaccess/SQLite.hpp"

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

// Function to configure the RESTinio server for the /api/submarines route
auto server_handler() {
  auto router = std::make_unique<router_t>();
  auto measurement_controller = std::make_shared<MeasurementController>();

  auto by = [&](auto method) {
    using namespace std::placeholders;
    return std::bind(method, measurement_controller, _1, _2);
  };

  // WEBSOCKET ENDPOINT
  router->http_get("/live", by(&MeasurementController::on_live_update));

  // WEBKLIENT GET ALL MEASUREMENTS FROM DATABASE ON LOAD.
  router->http_get("/measurements",
                   by(&MeasurementController::getMeasurements));

  // CORS ENDPOINT - PREFLIGHT REQUEST
  router->add_handler(restinio::http_method_options(), "/",
                      by(&MeasurementController::options));

  // SUBMARINE POST MEASUREMENTS AFTER A DIVE
  router->http_post("/api/postMeasurements",
                    by(&MeasurementController::postMeasurements));

  // SUBMARINE POST HARDWARETEST RESULT

  
  router->http_post("/api/postHardwareTestResult",
                    by(&MeasurementController::postHardwareTest));

  // WEBKLIENT POST MEASUREMENT DEPTH
  router->http_post("/api/PostMeasurementDepth",
                    by(&MeasurementController::postMeasurementDepth));


                    //cors options: 


             router->add_handler(restinio::http_method_options(), R"(/api/PostMeasurementDepth)",
                      by(&MeasurementController::options)); 
                      

  // SUBMARINE GET MEASUREMENT DEPTH
  router->http_get("/api/getMeasurementDepth",
                   by(&MeasurementController::getMeasurementDepth));

  return router;
}

int main() {
  using namespace std::chrono;

  std::cout << "Server running at http://192.168.0.1:8080" << std::endl;

  try {
    SQLite& db = SQLite::getInstance("Measurement.db");

    if (db.initializeScript()) {
      std::cout << "Database and table initialized successfully." << std::endl;
    } else {
      std::cerr << "Failed to initialize database or table." << std::endl;
    }

    using traits_t =
        restinio::traits_t<restinio::asio_timer_manager_t,
                           restinio::single_threaded_ostream_logger_t,
                           router_t>;

    restinio::run(restinio::on_this_thread<traits_t>()
                      .address("192.168.0.1")
                      .port(8080)
                      .request_handler(server_handler())
                      .read_next_http_message_timelimit(100s)
                      .write_http_response_timelimit(10s)
                      .handle_request_timeout(10s));
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}