//
// Created by owner on 10/21/24.
//

#ifndef WS_DEMO_ITOKENHANDLER_H
#define WS_DEMO_ITOKENHANDLER_H

#include <string>

class IEventHandler {
public:
    virtual ~IEventHandler() {}
    virtual const std::string& getCreatedToken() const = 0;
    virtual void onTokenCreated(const char* token) = 0;
};


#endif //WS_DEMO_ITOKENHANDLER_H
