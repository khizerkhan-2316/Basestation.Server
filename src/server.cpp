#include <restinio/all.hpp>





using router_t = restinio::router::express_router_t<>;


int main()
{

    auto router = std::make_shared<router_t>();

    router->http_get("/", [](auto req, auto) {
        return req->create_response()
            .append_header(restinio::http_field::content_type, "text/html")
            .set_body_file("src/static/index.html")  // Path to your index.html file
            .done();

      });


      init_api_routes(*router); 


       restinio::run(
        restinio::on_this_thread<restinio::default_traits_t>()
            .address("0.0.0.0")
            .port(80)
            .request_handler(router)
    );

    return 0;
    


}