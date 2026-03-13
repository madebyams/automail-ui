#include "config/config.hh"

#include <yaml-cpp/yaml.h>

using namespace config;

std::string config_t::to_json() const  {
    return json{
        {"hostname", hostname},
        {"port", port},
        {"username", username},
        {"password", password},
        {"ssl", ssl},
        {"mailbox", mailbox},
        {"client_type", type.to_string()}
    }.dump(2);
}

std::string config_t::to_yaml() const {
    YAML::Node node;
    YAML::Node imap_node;
    imap_node["hostname"] = hostname;
    imap_node["port"] = port;
    imap_node["username"] = username;
    imap_node["password"] = password;
    imap_node["ssl"] = ssl;
    imap_node["mailbox"] = mailbox;

    node["client_type"] = type.to_string();
    node["imap"] = imap_node;

    return YAML::Dump(node);
}