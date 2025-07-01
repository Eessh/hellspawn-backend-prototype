#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>
#include <functional>

#define IP "127.0.0.1"
#define PORT 4000

using namespace drogon;

int main()
{
    app().registerHandler("/",
                          [](const HttpRequestPtr& request, std::function<void(const HttpResponsePtr&)>&& callback) {
                              LOG_INFO << "connected: " << (request->connected() ? "true" : "false");

                              auto resp = HttpResponse::newHttpResponse();
                              resp->setBody("Hellspawn backend running ...");
                              resp->setContentTypeCode(ContentType::CT_TEXT_PLAIN);

                              callback(resp);
                          },
                          {Get});

    app().registerHandler("/echo?text={text}",
                          [](const HttpRequestPtr& request,
                             std::function<void(const HttpResponsePtr&)>&& callback,
                             const std::string& text) {
                              LOG_INFO << "connected: " << (request->connected() ? "true" : "false");

                              auto resp = HttpResponse::newHttpResponse();
                              resp->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
                              resp->setBody("{\"text\":\"" + text + "\"}");

                              callback(resp);
                          },
                          {Get});

    LOG_INFO << "Hellspawn server listening on - " << IP << ":" << PORT;
    app().addListener(IP, PORT).run();

    return 0;
}
