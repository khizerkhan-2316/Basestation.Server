#include <iostream>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include <filesystem>

namespace fs = std::filesystem;

struct book_t
{
    book_t() = default;

    book_t(std::string author, std::string title)
        : m_author{std::move(author)}, m_title{std::move(title)} {}

    template <typename JSON_IO>
    void json_io(JSON_IO &io)
    {
        io &json_dto::mandatory("author", m_author) &json_dto::mandatory("title", m_title);
    }

    std::string m_author;
    std::string m_title;
};

using book_collection_t = std::vector<book_t>;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

class books_handler_t
{
public:
    explicit books_handler_t(book_collection_t &books)
        : m_books(books) {}

    books_handler_t(const books_handler_t &) = delete;
    books_handler_t(books_handler_t &&) = delete;

    auto on_books_list(const restinio::request_handle_t &req, rr::route_params_t) const
    {
        auto resp = init_resp(req->create_response());

        resp.set_body("Book collection (book count: " + std::to_string(m_books.size()) + ")\n");

        for (std::size_t i = 0; i < m_books.size(); ++i)
        {
            resp.append_body(std::to_string(i + 1) + ". ");
            const auto &b = m_books[i];
            resp.append_body(b.m_title + "[" + b.m_author + "]\n");
        }

        return resp.done();
    }

    auto on_submarine_info(const restinio::request_handle_t &req, rr::route_params_t) const
    {
        auto resp = req->create_response();

        // Set the Content-Type header to application/json
        resp.append_header("Content-Type", "application/json")
            .append_header("Access-Control-Allow-Origin", "*")
            .append_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");

        // JSON response body
        std::string json_body = R"({"message": "hello world!!!"})";
        resp.set_body(json_body);

        return resp.done();
    }

private:
    book_collection_t &m_books;

    template <typename RESP>
    static RESP init_resp(RESP resp)
    {
        resp.append_header("Server", "RESTinio sample server /v.0.6")
            .append_header_date_field()
            .append_header("Content-Type", "text/plain; charset=utf-8");

        return resp;
    }

    template <typename RESP>
    static void mark_as_bad_request(RESP &resp)
    {
        resp.header().status_line(restinio::status_bad_request());
    }
};

auto server_handler(book_collection_t & book_collection)
{
    auto router = std::make_unique<router_t>();
    auto handler = std::make_shared<books_handler_t>(std::ref(book_collection));

    auto by = [&](auto method) {
        using namespace std::placeholders;
        return std::bind(method, handler, _1, _2);
    };

    auto method_not_allowed = [](const auto & req, auto) {
        return req->create_response(restinio::status_method_not_allowed())
                .connection_close()
                .done();
    };

    // Serve the index.html at '/'
    router->http_get("/", [](const restinio::request_handle_t& req, rr::route_params_t) {
    std::string index_file_path = "./index.html";
    std::cout << "Looking for file at: " << index_file_path << std::endl;

    std::ifstream file_stream(index_file_path);
    if (!file_stream.is_open()) {
        std::cerr << "Failed to open file: " << index_file_path << std::endl;
        auto resp = req->create_response(restinio::status_not_found());
        resp.set_body("index.html not found");
        return resp.done();
    }

    std::string file_content((std::istreambuf_iterator<char>(file_stream)),
                             std::istreambuf_iterator<char>());
    std::cout << "File content loaded successfully, sending response..." << std::endl;

    auto resp = req->create_response();
    resp.append_header("Content-Type", "text/html; charset=utf-8")
        .set_body(file_content);
    return resp.done();
    });


    // Optional: Handle other requests (like the books list) if needed
    router->http_get("/books", by(&books_handler_t::on_books_list));
    router->http_get("/api/submarines", by(&books_handler_t::on_submarine_info));

    // Disable all other methods for '/'
    router->add_handler(
        restinio::router::none_of_methods(restinio::http_method_get()),
        "/", method_not_allowed);

    return router;
}



int main()
{
    using namespace std::chrono;

    std::cout << "Server running at 192.168.0.1:8080" << std::endl;
    try
    {
        using traits_t = restinio::traits_t<
            restinio::asio_timer_manager_t,
            restinio::single_threaded_ostream_logger_t,
            router_t>;

        book_collection_t book_collection{
            {"Agatha Christie", "Murder on the Orient Express"},
            {"Agatha Christie", "Sleeping Murder"},
            {"B. Stroustrup", "The C++ Programming Language"}};

        restinio::run(
            restinio::on_this_thread<traits_t>()
                .address("192.168.0.1")
                .port(8080)
                .request_handler(server_handler(book_collection))
                .read_next_http_message_timelimit(10s)
                .write_http_response_timelimit(1s)
                .handle_request_timeout(1s));
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
