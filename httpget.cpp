#include <string>
#include <sstream>
#include <stdexcept>
#include <cassert>

#include <tchar.h>
#include <Windows.h>
#include <wininet.h>

#include "httpget.h"

namespace httpget
{

Session::Session(const char *host_url) : host_url_string(host_url)
{
    if (!(hIntSession = ::InternetOpen(
            _T("HttpGetRequest"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0))) {
        throw std::runtime_error("Problem with internet connection occurred");
    }
    if (!(hHttpSession = InternetConnect(
            hIntSession, _T(host_url), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, 0))) {
        throw std::runtime_error("Could not connect to " + std::string(host_url));
    }
}

void Session::Send(const char *suffix)
{
    if (hHttpRequest) {
        ::InternetCloseHandle(hHttpRequest);
    }

    if (!(hHttpRequest = HttpOpenRequest(
            hHttpSession,
            _T("GET"),
            _T(suffix),
            0, 0, 0, INTERNET_FLAG_RELOAD, 0))) {
        throw std::runtime_error(std::string("Could not open request to ") +
                host_url_string + " " + suffix);
    }
                
    if( !HttpSendRequest(hHttpRequest, NULL, 0, NULL, 0)) {
        DWORD dwErr = GetLastError();
        std::string error_string =
            "Could not send GET request. Wininet error: " + std::to_string(dwErr);
        throw std::runtime_error(error_string);
    }
    response_ready = true;
}

std::ostream &operator<<(std::ostream &os, Session &session)
{
    assert(session.response_ready);
    CHAR szBuffer[BUF_SIZE];
    DWORD dwRead=0;
    while(::InternetReadFile(session.hHttpRequest, szBuffer, sizeof(szBuffer)-1, &dwRead) && dwRead) {
        szBuffer[dwRead] = 0;
        os << szBuffer;
        dwRead=0;
    }
    session.response_ready = false;
    return os;
}

Session::~Session()
{
    if (hHttpRequest) {
        ::InternetCloseHandle(hHttpRequest);
    }
    ::InternetCloseHandle(hHttpSession);
    ::InternetCloseHandle(hIntSession);
}

}
