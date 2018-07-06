#ifndef HTTPGET_H
#define HTTPGET_H

/** Note that in order to use these methods
    linking with wininet.lib is required
**/
#include <sstream>
#include <ostream>

#include <tchar.h>
#include <Windows.h>
#include <wininet.h>

namespace httpget
{

constexpr size_t BUF_SIZE = 1025;

// Note that if an exception was thrown by any method
// there is no guarantee that the object is still usable or
// no memory leaks happened

class Session
{
public:
    Session(const char *host_url);
    ~Session();

    void Send(const char *suffix);
    friend std::ostream& operator<<(std::ostream &os, Session &session);
private:
    HINTERNET hIntSession = NULL;
    HINTERNET hHttpSession = NULL;
    HINTERNET hHttpRequest = NULL;

    bool response_ready = false;
    std::string host_url_string;

};

}

#endif // HTTPGET_H
