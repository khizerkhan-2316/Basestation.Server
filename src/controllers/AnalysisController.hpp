#ifndef ANALYSIS_CONTROLLER_HPP
#define ANALYSIS_CONTROLLER_HPP

#include <json_dto/pub.hpp>
#include <restinio/all.hpp>
#include <vector>

#include "/home/stud/ngk/restinio/dev/sample/Basestation.Server/src/dataaccess/AnalysisRepo.hpp"
#include "/home/stud/ngk/restinio/dev/sample/Basestation.Server/src/models/Analysis.hpp"
#include "/home/stud/ngk/restinio/dev/sample/Basestation.Server/src/models/Submarine.hpp"

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

namespace rws = restinio::websocket::basic;
using traits_t =
    restinio::traits_t<restinio::asio_timer_manager_t,
                       restinio::single_threaded_ostream_logger_t, router_t>;
using ws_registry_t = std::map<std::uint64_t, rws::ws_handle_t>;

class AnalysisController {
 public:
  auto handlestartAnalysis();

  auto on_live_update(const restinio::request_handle_t &req,
                      rr ::route_params_t params) {
    if (restinio::http_connection_header_t::upgrade ==
        req->header().connection()) {
      auto wsh = rws ::upgrade<traits_t>(
          *req, rws::activation_t::immediate, [this](auto wsh, auto m) {
            wsh->send_message(rws::final_frame, rws::opcode_t::text_frame,
                              "Action: Updates coming here");
          });
      m_registry.emplace(wsh->connection_id(), wsh);
      init_resp(req->create_response()).done();
      return restinio::request_accepted();
    }
    return restinio::request_rejected();
  }

  auto options(restinio::request_handle_t req,
               restinio::router::route_params_t) {
    const auto methods = "OPTIONS, GET, POST, PATCH, DELETE, PUT";
    auto resp = init_resp(req->create_response());
    resp.append_header(restinio::http_field::access_control_allow_methods,
                       methods);
    resp.append_header(restinio::http_field::access_control_max_age, "86400");
    return resp.done();
  }

  // Handler for the `/api/analysis/:id` route
  auto getAnalysisResults(
      const restinio::request_handle_t &req,
      const restinio::router::route_params_t &params) const {
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

    // Get the `id` parameter from the URL
    std::string id = restinio::cast_to<std::string>(params["id"]);

    // Create a fake Analysis object (replace this with real data fetching)
    std::string json_body = R"({
            "id": ")" + id + R"(",
            "status": "success",
            "result": "Analysis data for id )" +
                            id + R"("
        })";

    // Set the JSON body as the response
    resp.set_body(json_body);

    // Return the response
    return resp.done();
  }

  //   std::vector<Analysis> getAllAnalysis() {
  //     try {
  //       return analysisRepo.getAllAnalysis();
  //     } catch (const std::exception &e) {
  //       std::cerr << "Error retrieving all analyses: " << e.what() <<
  //       std::endl; throw;
  //     }
  //   }DENNE METODE GIVER FEJL NÅ JEG FORSØGER AT COMPILE -ALI

  // Handler for the `/api/submarine/:submarineId/analysis` route

  auto testAli(const restinio::request_handle_t &req,
               const restinio::router::route_params_t &params) {
    auto resp = req->create_response();

    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

    resp.set_body("hej");
    sendMessage("det virker" + std::to_string(1));

    // Return the response
    return resp.done();
  }

  auto getSubmarineAnalysis(
      const restinio::request_handle_t &req,
      const restinio::router::route_params_t &params) const {
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

    // Get the `submarineId` parameter from the URL
    std::string submarineId =
        restinio::cast_to<std::string>(params["submarineId"]);

    // Create a fake Analysis list (replace this with real data fetching) SKAL
    // RETTES!!!!
    std::string json_body = R"({
        "submarineId": ")" + submarineId +
                            R"(",
        "status": "success",
        "analysis": [
            {
                "analysisId": "1",
                "type": "structural",
                "result": "No issues detected."
            },
            {
                "analysisId": "2",
                "type": "performance",
                "result": "Optimal performance observed."
            }
        ]
    })";

    // Set the JSON body as the response
    resp.set_body(json_body);
    sendMessage("det virker" + std::to_string(1));

    // Return the response
    return resp.done();
  }

  // Handler for the `/api/analysis/:id/approve` route
  auto approveAnalysis(const restinio::request_handle_t &req,
                       const restinio::router::route_params_t &params) const {
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    // Get the `id` parameter from the URL
    std::string id = restinio::cast_to<std::string>(params["id"]);

    // Logic to approve the analysis (replace this with real data handling)
    bool success = true;  // Simulate success in approval
    std::string message;

    if (success) {
      message = "Analysis with ID " + id + " has been approved.";
    } else {
      message = "Failed to approve analysis with ID " + id + ".";
    }

    // Create a JSON response body
    std::string json_body = R"({
        "id": ")" + id + R"(",
        "status": ")" + (success ? "success" : "failure") +
                            R"(",
        "message": ")" + message +
                            R"("
    })";

    // Set the JSON body as the response
    resp.set_body(json_body);

    // Return the response
    return resp.done();
  }

  // Handler for the `/api/analysis/:id/reject` route
  auto rejectAnalysis(const restinio::request_handle_t &req,
                      const restinio::router::route_params_t &params) const {
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    // Get the `id` parameter from the URL
    std::string id = restinio::cast_to<std::string>(params["id"]);

    // Logic to reject the analysis (replace this with real data handling)
    bool success = true;  // Simulate success in rejection
    std::string message;

    if (success) {
      message = "Analysis with ID " + id + " has been rejected.";
    } else {
      message = "Failed to reject analysis with ID " + id + ".";
    }

    // Create a JSON response body
    std::string json_body = R"({
        "id": ")" + id + R"(",
        "status": ")" + (success ? "success" : "failure") +
                            R"(",
        "message": ")" + message +
                            R"("
    })";

    // Set the JSON body as the response
    resp.set_body(json_body);

    // Return the response
    return resp.done();
  }

  // Handler for the `/api/submarine/select` route
  auto handleSubmarineSelection(
      const restinio::request_handle_t &req,
      const restinio::router::route_params_t &params) const {
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    try {
      //   // Parse the JSON body from the request
      //   auto body = req->body();
      //   nlohmann::json json_body = nlohmann::json::parse(body);
      //   // Extract submarine details
      //   std::string submarineId =
      //   json_body.at("submarineId").get<std::string>(); std::string name =
      //   json_body.at("name").get<std::string>();
      // //   std::string type = json_body.at("type").get<std::string>();
      //   // Create a Submarine object (replace this with your actual Submarine
      //   // model)
      //   Submarine submarine{submarineId, name, type};

      std::string requestBody = req->body();
      Submarine submarine;
      json_dto::from_json(requestBody, submarine);

      // Logic to handle submarine selection (replace with actual logic)
      bool success = true;  // Simulate successful handling
      std::string message;

      if (success) {
        message = "Submarine with ID " + submarine.id +
                  " has been successfully selected.";

      } else {
        message =
            "Failed to handle submarine selection for ID " + submarine.id + ".";
      }

      // Create a JSON response body
      //   std::string response_body = R"({
      //           "status": ")" + (success ? "success" : "failure") +
      //                               R"(",
      //           "message": ")" + message +
      //                               R"("
      //       })";

      std::string response_body = "test";

      // Set the JSON body as the response
      resp.set_body(response_body);
    } catch (const std::exception &e) {
      // Handle JSON parsing or extraction errors
      std::string error_message = R"({
              "status": "failure",
              "message": "Invalid request data: )" +
                                  std::string(e.what()) + R"("
          })";
      resp.set_body(error_message);
    }

    // Return the response
    return resp.done();
  }
  AnalysisController() = default;

 private:
  ws_registry_t m_registry;

  void sendMessage(std::string message) const {
    for (auto [k, v] : m_registry)
      v->send_message(rws::final_frame, rws::opcode_t::text_frame, message);
  }
  template <typename RESP>
  static RESP init_resp(RESP resp) {
    resp.append_header("Server", "RESTinio sample server /v.0.6")
        .append_header_date_field()
        .append_header("Content-Type", "application/json")
        .append_header(restinio::http_field::access_control_allow_origin, "*");
    return resp;
  }
};

#endif  // SUBMARINE_CONTROLLER_HPP