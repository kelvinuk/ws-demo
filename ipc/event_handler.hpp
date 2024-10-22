//
// Created by owner on 10/21/24.
//

#ifndef WS_DEMO_EVENT_HANDLER_HPP
#define WS_DEMO_EVENT_HANDLER_HPP

#include "../interface/ievent_handler.hpp"

class EventHandler : public IEventHandler
{
private:
    std::string token_;
public:
    EventHandler();
    virtual ~EventHandler();
    virtual const std::string& getCreatedToken() const;
    virtual void onTokenCreated(const char* token);
};


#endif //WS_DEMO_EVENT_HANDLER_HPP
