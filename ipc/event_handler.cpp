//
// Created by owner on 10/21/24.
//

#include "event_handler.hpp"


EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

const std::string& EventHandler::getCreatedToken() const
{
    return token_;
}

void EventHandler::onTokenCreated(const char* token)
{
    token_ = token;
}
