#include <iostream>
#include <restinio/all.hpp>
#include <restinio/websocket/websocket.hpp>

#include "./controllers/AnalysisController.hpp"  // Include your SubmarineController header file
#include "./controllers/SubmarineController.hpp"  // Include your SubmarineController header file

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

// Function to configure the RESTinio server for the /api/submarines route
auto server_handler() {
  auto router = std::make_unique<router_t>();
  auto submarine_controller = std::make_shared<SubmarineController>();
  auto analysis_controller = std::make_shared<AnalysisController>();

  auto by = [&](auto method) {
    using namespace std::placeholders;
    return std::bind(method, analysis_controller, _1, _2);
  };

  // Handle only the /api/submarines GET request
  /* router->http_get(
      "/api/submarines", [submarine_controller](const auto &req, auto params) {
        return submarine_controller->on_submarine_info(req, params);
      });
 */

  router->http_get("/api/submarine/:id", by(&SubmarineController::on_submarine_info));
 
  router->http_get("/live", by(&AnalysisController::on_live_update));

  router->add_handler(restinio::http_method_options(), "/",
                      by(&AnalysisController::options));
  router->http_get("/get", by(&AnalysisController::testAli));

  // router->http_post("api/handleStartMeasurements",
  //                   by(&AnalysisController::handlestartAnalysis));

  router->http_get("/api/getMeasurementResults/:id",
                   by(&AnalysisController::getAnalysisResults));

  //   router->http_get("api/getAllMeasurements",
  //                    by(&AnalysisController::getAllAnalysis));

  router->http_get("/api/getSubmarineMeasurements/:id",
                   by(&AnalysisController::getSubmarineAnalysis));

  router->http_post("/api/approveMeasurements/:id",
                    by(&AnalysisController::approveAnalysis));

  router->http_post("/api/rejectMeasurements/:id",
                    by(&AnalysisController::rejectAnalysis));

  router->http_post("/api/postMeasurements",
                    by(&AnalysisController::postMeasurements));
  router->http_post("/api/handleSubmarineSelection",
                    by(&AnalysisController::handleSubmarineSelection));

  return router;
}

int main() {
  using namespace std::chrono;

  std::cout << "Server running at http://192.168.0.1:8080" << std::endl;

  try {
    using traits_t =
        restinio::traits_t<restinio::asio_timer_manager_t,
                           restinio::single_threaded_ostream_logger_t,
                           router_t>;

    restinio::run(restinio::on_this_thread<traits_t>()
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
