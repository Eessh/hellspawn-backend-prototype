#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>
#include <functional>

#define IP "127.0.0.1"
#define PORT 4000

using namespace drogon;

/// Configure Cross-Origin Resource Sharing (CORS) support.
///
///  This function registers both synchronous pre-processing advice for handling
///  OPTIONS preflight requests and post-handling advice to inject CORS headers
///  into all responses dynamically based on the incoming request headers.
void setupCors()
{
    // Register sync advice to handle CORS preflight (OPTIONS) requests
    app().registerSyncAdvice([](const HttpRequestPtr& req) -> HttpResponsePtr {
        if(req->method() == HttpMethod::Options)
        {
            auto resp = HttpResponse::newHttpResponse();

            // Set Access-Control-Allow-Origin header based on the Origin
            // request header
            const auto& origin = req->getHeader("Origin");
            if(!origin.empty())
            {
                resp->addHeader("Access-Control-Allow-Origin", origin);
            }

            // Set Access-Control-Allow-Methods based on the requested method
            const auto& requestMethod = req->getHeader("Access-Control-Request-Method");
            if(!requestMethod.empty())
            {
                resp->addHeader("Access-Control-Allow-Methods", requestMethod);
            }

            // Allow credentials to be included in cross-origin requests
            resp->addHeader("Access-Control-Allow-Credentials", "true");

            // Set allowed headers from the Access-Control-Request-Headers
            // header
            const auto& requestHeaders = req->getHeader("Access-Control-Request-Headers");
            if(!requestHeaders.empty())
            {
                resp->addHeader("Access-Control-Allow-Headers", requestHeaders);
            }

            return std::move(resp);
        }
        return {};
    });

    // Register post-handling advice to add CORS headers to all responses
    app().registerPostHandlingAdvice([](const HttpRequestPtr& req, const HttpResponsePtr& resp) -> void {
        // Set Access-Control-Allow-Origin based on the Origin request
        // header
        const auto& origin = req->getHeader("Origin");
        if(!origin.empty())
        {
            resp->addHeader("Access-Control-Allow-Origin", origin);
        }

        // Reflect the requested Access-Control-Request-Method back in the
        // response
        const auto& requestMethod = req->getHeader("Access-Control-Request-Method");
        if(!requestMethod.empty())
        {
            resp->addHeader("Access-Control-Allow-Methods", requestMethod);
        }

        // Allow credentials to be included in cross-origin requests
        resp->addHeader("Access-Control-Allow-Credentials", "true");

        // Reflect the requested Access-Control-Request-Headers back
        const auto& requestHeaders = req->getHeader("Access-Control-Request-Headers");
        if(!requestHeaders.empty())
        {
            resp->addHeader("Access-Control-Allow-Headers", requestHeaders);
        }
    });
}

int main()
{
    app().registerHandler("/",
                          [](const HttpRequestPtr& request, std::function<void(const HttpResponsePtr&)>&& callback) {
                              LOG_INFO << "connected: " << (request->connected() ? "true" : "false");

                              auto resp = HttpResponse::newHttpResponse();
                              resp->setStatusCode(HttpStatusCode::k200OK);
                              resp->setContentTypeCode(ContentType::CT_TEXT_PLAIN);
                              resp->setBody("Hellspawn backend running ...");

                              callback(resp);
                          },
                          {Get, Options});

    app().registerHandler("/echo?text={text}",
                          [](const HttpRequestPtr& request,
                             std::function<void(const HttpResponsePtr&)>&& callback,
                             const std::string& text) {
                              LOG_INFO << "connected: " << (request->connected() ? "true" : "false");

                              auto resp = HttpResponse::newHttpResponse();
                              resp->setStatusCode(HttpStatusCode::k200OK);
                              resp->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
                              resp->setBody("{\"text\":\"" + text + "\"}");

                              callback(resp);
                          },
                          {Get, Options});

    setupCors();

    LOG_INFO << "Hellspawn server listening on - " << IP << ":" << PORT;
    app().addListener(IP, PORT).run();

    return 0;
}
