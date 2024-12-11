#pragma once

#ifndef ANALYSIS_CONTROLLER_HPP
#define ANALYSIS_CONTROLLER_HPP

#include <cstdint>
#include <fstream>
#include <json_dto/pub.hpp>
#include <restinio/all.hpp>
#include <vector>

#include "../models/Submarine.hpp"
#include "../models/MeasurementDepth.hpp"
#include "../services/MVPserver.hpp"

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

namespace rws = restinio::websocket::basic;
using traits_t =
    restinio::traits_t<restinio::asio_timer_manager_t,
                       restinio::single_threaded_ostream_logger_t, router_t>;
using ws_registry_t = std::map<std::uint64_t, rws::ws_handle_t>;



 

class MeasurementController {
 public:
  MeasurementController() { measurementDepth = ""; }

  auto getMeasurementDepth(const restinio::request_handle_t &req,
                           const restinio::router::route_params_t &params) {
    
        auto resp = init_resp(req->create_response());  // Create the response object

    if (measurementDepth != "") {
        // Set the response to 200 OK
        resp.append_header(restinio::http_field::content_type, "text/plain; charset=utf-8").set_body("P" + measurementDepth); 
        
        measurementDepth = "";
        
         // Add the measurement depth
        return resp.done();
    } else {
        // Set the response to 204 No Content
        resp.header().status_line(restinio::status_no_content());
        return resp.done();
    }

  }

  auto postHardwareTestResult(const restinio::request_handle_t &req,
                              rr ::route_params_t params) {
    auto resp = init_resp(req->create_response());

    std::string result = req->body();

    sendMessage(result);

    resp.set_body("Hardware test result received");
    return resp.done();
  }

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

  auto postMeasurementDepth(const restinio::request_handle_t &req,
                            const restinio::router::route_params_t &params) {
    auto resp = init_resp(req->create_response());

        // Extract the raw request body
        std::string body = req->body();

        // Output the raw body (for debugging)
        std::cout << "Received Body: " << body << std::endl;

        try {
            // Deserialize JSON body into MeasurementDepth object
            MeasurementDepth data;
            json_dto::from_json(body, data);

            // Now, measurementDepth is extracted from the JSON and stored in the Server instance
            measurementDepth = data.measurementDepth;

            // Log the measurementDepth
            std::cout << "Measurement Depth: " << measurementDepth << std::endl;

            // Set response body

    std::string response_body = R"({
        "message": "success",
        "details": "MeasurementDepth received successfully"
    })";

    // Set the response body
    resp.set_body(response_body);
            // Respond back to the client

            return resp.done();
        } catch (const std::exception& e) {
            // Handle errors (e.g., invalid JSON or missing key)
            std::cout << "Error parsing JSON: " << e.what() << std::endl;
            resp.set_body("Invalid JSON or missing 'measurementDepth' field");
            return resp.done();
        }


    }
  

  auto postMeasurements(const restinio::request_handle_t &req,
                        const restinio::router::route_params_t &params) {
    auto resp = init_resp(req->create_response());

    std::string requestBody = req->body();

    std::cout << "MEASUREMENTS RECIEVED FROM SUB: " << requestBody << std::endl;

    sendMessage(requestBody);

    systemController_.handleMeasurements(requestBody);

    resp.set_body("Measurements saved!");

    return resp.done();
  }

  auto getMeasurements(const restinio::request_handle_t &req,
                       const restinio::router::route_params_t &params) {
    auto resp = init_resp(req->create_response());

    resp.set_body(json_dto::to_json(systemController_.getAllMeasurements()));

    return resp.done();
  }

  auto options(const restinio::request_handle_t& req, rr::route_params_t)
    {
        auto resp = init_resp(req->create_response());
        resp.append_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT")
            .append_header("Access-Control-Allow-Headers", "Content-Type")
            .append_header("Access-Control-Max-Age", "86400");
        return resp.done();
    }


  auto postHardwareTest(const restinio::request_handle_t &req,
                        const restinio::router::route_params_t &params) {

    auto resp = init_resp(req->create_response());

    std::string body = req->body();

    std::cout << "REQUEST BODY:" << body << std::endl;

    uint8_t byte = static_cast<uint8_t>(body[0]);


    std::cout << "BYTE:" << byte << std::endl;

    bool isHardwareTestValid = systemController_.isHardwareTestValid(byte);

    std::cout << "isHardwareTestValid: " << isHardwareTestValid << std::endl;

    std::string responseMessage = isHardwareTestValid ? "success" : "failure";

    std::cout <<  "RESPONSE MESSAGE HARDWARE TEST:" << responseMessage << std::endl;

    std::string jsonResponse = "{ \"type\": \"hardwareTest\", \"payload\": { "
                            "\"status\": \"" + std::string(isHardwareTestValid ? "passed" : "failed") + "\", "
                            "\"details\": \"" + std::string(isHardwareTestValid ? "All tests successful" : "Some tests failed") + "\" } }";


    std::cout << "JSON RESPONSE:" << jsonResponse << std::endl;

    sendMessage(jsonResponse);

    return resp.done();
  }



 private:
  std::string measurementDepth;

  ws_registry_t m_registry;
  MVPServer systemController_;

  void sendMessage(std::string message) const {
    for (auto [k, v] : m_registry)
      v->send_message(rws::final_frame, rws::opcode_t::text_frame, message);
  }
  template < typename RESP >
    static RESP
        init_resp(RESP resp)
    {
        resp
            .append_header("Server", "RESTinio sample server /v.0.6")
            .append_header_date_field()
            .append_header("Content-Type", "application/json")
            .append_header("Access-Control-Allow-Origin", "*"); // Inkludere CORS:
        return resp;
    }
};

#endif  