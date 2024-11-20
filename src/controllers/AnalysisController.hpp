#ifndef ANALYSIS_CONTROLLER_HPP
#define ANALYSIS_CONTROLLER_HPP

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include <vector>


class AnalysisController 
{
    public:
    auto handlestartAnalysis();

    // Handler for the `/api/analysis/:id` route
    auto getAnalysisResults(const restinio::request_handle_t &req, const restinio::router::route_params_t &params) const 
    {
        // Create a response object
        auto resp = req->create_response();

        // Set CORS and Content-Type headers
        resp.append_header("Content-Type", "application/json; charset=utf-8")
            .append_header("Access-Control-Allow-Origin", "*")
            .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

        // Get the `id` parameter from the URL
        std::string id = params["id"];

        // Create a fake Analysis object (replace this with real data fetching)
        std::string json_body = R"({
            "id": ")" + id + R"(",
            "status": "success",
            "result": "Analysis data for id )" + id + R"("
        })";

        // Set the JSON body as the response
        resp.set_body(json_body);

        // Return the response
        return resp.done();
    }

   std::vector<Analysis> getAllAnalysis() {
        try {
            return analysisRepo.getAllAnalysis();
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving all analyses: " << e.what() << std::endl;
            throw;
        }
    }

    // Handler for the `/api/submarine/:submarineId/analysis` route
auto getSubmarineAnalysis(const restinio::request_handle_t &req, const restinio::router::route_params_t &params) const
{
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

    // Get the `submarineId` parameter from the URL
    std::string submarineId = params["submarineId"];

    // Create a fake Analysis list (replace this with real data fetching)                      SKAL RETTES!!!!
    std::string json_body = R"({
        "submarineId": ")" + submarineId + R"(",
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

    // Return the response
    return resp.done();
}


 // Handler for the `/api/analysis/:id/approve` route
auto approveAnalysis(const restinio::request_handle_t &req, const restinio::router::route_params_t &params) const
{
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    // Get the `id` parameter from the URL
    std::string id = params["id"];

    // Logic to approve the analysis (replace this with real data handling)
    bool success = true; // Simulate success in approval
    std::string message;

    if (success)
    {
        message = "Analysis with ID " + id + " has been approved.";
    }
    else
    {
        message = "Failed to approve analysis with ID " + id + ".";
    }

    // Create a JSON response body
    std::string json_body = R"({
        "id": ")" + id + R"(",
        "status": ")" + (success ? "success" : "failure") + R"(",
        "message": ")" + message + R"("
    })";

    // Set the JSON body as the response
    resp.set_body(json_body);

    // Return the response
    return resp.done();
}


  // Handler for the `/api/analysis/:id/reject` route
auto rejectAnalysis(const restinio::request_handle_t &req, const restinio::router::route_params_t &params) const
{
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    // Get the `id` parameter from the URL
    std::string id = params["id"];

    // Logic to reject the analysis (replace this with real data handling)
    bool success = true; // Simulate success in rejection
    std::string message;

    if (success)
    {
        message = "Analysis with ID " + id + " has been rejected.";
    }
    else
    {
        message = "Failed to reject analysis with ID " + id + ".";
    }

    // Create a JSON response body
    std::string json_body = R"({
        "id": ")" + id + R"(",
        "status": ")" + (success ? "success" : "failure") + R"(",
        "message": ")" + message + R"("
    })";

    // Set the JSON body as the response
    resp.set_body(json_body);

    // Return the response
    return resp.done();
}


   // Handler for the `/api/submarine/select` route
auto handleSubmarineSelection(const restinio::request_handle_t &req) const
{
    // Create a response object
    auto resp = req->create_response();

    // Set CORS and Content-Type headers
    resp.append_header("Content-Type", "application/json; charset=utf-8")
        .append_header("Access-Control-Allow-Origin", "*")
        .append_header("Access-Control-Allow-Methods", "POST, OPTIONS");

    try
    {
        // Parse the JSON body from the request
        auto body = req->body();
        nlohmann::json json_body = nlohmann::json::parse(body);

        // Extract submarine details
        std::string submarineId = json_body.at("submarineId").get<std::string>();
        std::string name = json_body.at("name").get<std::string>();
        std::string type = json_body.at("type").get<std::string>();

        // Create a Submarine object (replace this with your actual Submarine model)
        Submarine submarine{submarineId, name, type};

        // Logic to handle submarine selection (replace with actual logic)
        bool success = true; // Simulate successful handling
        std::string message;

        if (success)
        {
            message = "Submarine with ID " + submarineId + " has been successfully selected.";
        }
        else
        {
            message = "Failed to handle submarine selection for ID " + submarineId + ".";
        }

        // Create a JSON response body
        std::string response_body = R"({
            "status": ")" + (success ? "success" : "failure") + R"(",
            "message": ")" + message + R"("
        })";

        // Set the JSON body as the response
        resp.set_body(response_body);
    }
    catch (const std::exception &e)
    {
        // Handle JSON parsing or extraction errors
        std::string error_message = R"({
            "status": "failure",
            "message": "Invalid request data: )" + std::string(e.what()) + R"("
        })";
        resp.set_body(error_message);
    }

    // Return the response
    return resp.done();
}





    private:
    
     AnalysisController() = default;


}


#endif // SUBMARINE_CONTROLLER_HPP