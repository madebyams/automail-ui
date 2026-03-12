#pragma once

#include <list>
#include <string>

namespace mail {
    class client {
    public:
        virtual ~client() = default;
        virtual void init() = 0;
        virtual int delete_from_senders(std::list<std::string> senders) = 0;
        virtual void reload() = 0;
    };
}