#pragma once

#include <drogon/HttpController.h>

namespace hellspawn::rest::controllers
{

class AuthController : public drogon::HttpController<AuthController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::login, "/auth/login?username={1}&password={2}", drogon::HttpMethod::Post);
    ADD_METHOD_TO(AuthController::check, "/auth/check", drogon::HttpMethod::Get);
    ADD_METHOD_TO(AuthController::logout, "/auth/logout", drogon::HttpMethod::Get);
    METHOD_LIST_END

    void login(const drogon::HttpRequestPtr& request,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback,
               std::string&& username,
               std::string&& password) const noexcept;

    void check(const drogon::HttpRequestPtr& request,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback) const noexcept;

    void logout(const drogon::HttpRequestPtr& request,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback) const noexcept;
};

}; // namespace hellspawn::rest::controllers
