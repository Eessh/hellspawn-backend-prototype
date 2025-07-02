#include "./AuthController.hpp"
#include <jwt-cpp/jwt.h>
#include "../models/user.hpp"
#include "AuthController.hpp"

#define JWT_COOKIE_NAME "Token"
#define JWT_CLAIM_NAME "username"
#define JWT_ISSUER_NAME "Hellspawn"
#define JWT_SECRET "fuck you"

namespace hellspawn::rest::controllers
{

void AuthController::login(const drogon::HttpRequestPtr& request,
                           std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                           std::string&& username,
                           std::string&& password) const noexcept
{
    LOG_DEBUG << "username: " << username << ", password: " << password;

    const db::models::User user{username, password};

    Json::Value response_body{};
    response_body["success"] = true;
    response_body["data"] = user.to_json_value();

    const auto jwt_token = jwt::create()
                             .set_type("JWT")
                             .set_payload_claim(JWT_CLAIM_NAME, jwt::claim(username))
                             .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24 * 7))
                             .set_issuer(JWT_ISSUER_NAME)
                             .sign(jwt::algorithm::hs512(JWT_SECRET));

    drogon::Cookie jwt_cookie{JWT_COOKIE_NAME, jwt_token};
    jwt_cookie.setHttpOnly(true);
    jwt_cookie.setMaxAge(24 * 60 * 60);
    jwt_cookie.setSameSite(drogon::Cookie::SameSite::kStrict);
    jwt_cookie.setSecure(true);

    auto response = drogon::HttpResponse::newHttpJsonResponse(response_body);
    response->addCookie(jwt_cookie);

    return callback(response);
}

void AuthController::check(const drogon::HttpRequestPtr& request,
                           std::function<void(const drogon::HttpResponsePtr&)>&& callback) const noexcept
{
    const std::string jwt_cookie = request->getCookie(JWT_COOKIE_NAME);

    Json::Value response_body{};

    if(jwt_cookie.empty())
    {
        response_body["success"] = false;
        response_body["data"] = "No auth cookie found.";

        drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpJsonResponse(response_body);
        response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);

        return callback(response);
    }

    try
    {
        const auto decoded_jwt_cookie = jwt::decode(jwt_cookie);
        const auto decoded_claim = decoded_jwt_cookie.get_payload_claim(JWT_CLAIM_NAME);
        const std::string decoded_username = decoded_claim.as_string();

        LOG_DEBUG << "username: " << decoded_username;

        const db::models::User user{decoded_username, nullptr};

        response_body["success"] = true;
        response_body["data"] = user.to_json_value_without_password();

        const auto jwt_token = jwt::create()
                                 .set_type("JWT")
                                 .set_payload_claim("username", jwt::claim(decoded_username))
                                 .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24 * 7))
                                 .set_issuer("Hellspawn")
                                 .sign(jwt::algorithm::hs512(JWT_SECRET));

        drogon::Cookie jwt_cookie{JWT_COOKIE_NAME, jwt_token};
        jwt_cookie.setHttpOnly(true);
        jwt_cookie.setMaxAge(24 * 60 * 60);
        jwt_cookie.setSameSite(drogon::Cookie::SameSite::kStrict);
        jwt_cookie.setSecure(true);

        auto response = drogon::HttpResponse::newHttpJsonResponse(response_body);
        response->addCookie(jwt_cookie);

        return callback(response);
    }
    catch(std::invalid_argument& error)
    {
        response_body["success"] = false;
        response_body["data"] = "Invalid auth cookie found.";

        drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpJsonResponse(response_body);
        response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);

        return callback(response);
    }
    catch(std::runtime_error& error)
    {
        response_body["success"] = false;
        response_body["data"] = "Invalid auth cookie found.";

        drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpJsonResponse(response_body);
        response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);

        return callback(response);
    }
}

void AuthController::logout(const drogon::HttpRequestPtr& request,
                            std::function<void(const drogon::HttpResponsePtr&)>&& callback) const noexcept
{
    callback(drogon::HttpResponse::newHttpResponse());
}

}; // namespace hellspawn::rest::controllers
