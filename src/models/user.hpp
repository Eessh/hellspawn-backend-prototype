#pragma once

#include <drogon/drogon.h>
#include <string>

namespace hellspawn::db::models
{

class User
{
  public:
    User() noexcept = default;
    User(std::string_view username, std::string_view password) noexcept : _username(username), _password(password) {}

    inline const std::string_view get_username() const noexcept
    {
        return _username;
    }

    inline void set_username(std::string_view username) noexcept
    {
        _username = username;
    }

    inline const std::string_view get_password() const noexcept
    {
        return _password;
    }

    inline void set_password(std::string_view password) noexcept
    {
        _password = password;
    }

    inline void set_password_with_encryption(std::string_view unencrypted_password) noexcept
    {
        /// TODO: Encryption implementation pending
        _password = unencrypted_password;
    }

    Json::Value to_json_value() const noexcept
    {
        Json::Value serialized{};
        serialized["username"] = _username;
        serialized["password"] = _password;

        return serialized;
    }

    Json::Value to_json_value_without_password() const noexcept
    {
        Json::Value serialized{};
        serialized["username"] = _username;

        return serialized;
    }

  private:
    std::string _username;
    std::string _password;
};

} // namespace hellspawn::db::models
