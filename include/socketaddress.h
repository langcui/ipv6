#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>



using namespace std;

union ipaddr_union {
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    bool compare(const ipaddr_union& ip) const{
        if(this->sa.sa_family != ip.sa.sa_family) return false;

        if(this->sa.sa_family == AF_INET)
        {
            return this->sin.sin_addr.s_addr == ip.sin.sin_addr.s_addr ? true : false;
        }else if(this->sa.sa_family == AF_INET6)
        {
            char addr1[INET6_ADDRSTRLEN] = { 0 };
            char addr2[INET6_ADDRSTRLEN] = { 0 };
#if defined(_WIN32)
            ::inet_ntop(AF_INET6, (PVOID)&(this->sin6.sin6_addr), addr1, sizeof(addr1));
            ::inet_ntop(AF_INET6, (PVOID)&(ip.sin6.sin6_addr), addr2, sizeof(addr2));
#else
            ::inet_ntop(AF_INET6, &(this->sin6.sin6_addr), addr1, sizeof(addr1));
            ::inet_ntop(AF_INET6, &(ip.sin6.sin6_addr), addr2, sizeof(addr2));
#endif
            return strcmp(addr1, addr2) == 0 ? true : false;
        }
        return false;
    }
};
class socketaddress
{
    public:
        socketaddress()
        {
            mIsSupportIPv6 = false;
            memset(&mAddr,0,sizeof(ipaddr_union));
        }
        void setInet(sockaddr_in addr4);
        void setInet6(sockaddr_in6 addr6);
        void setPort(uint16_t port);
        uint16_t getPort();
        void setSupportIPv6(bool isSupport)
        {
            mIsSupportIPv6 = isSupport;
        }
        bool getIsSupportIPv6(){return mIsSupportIPv6;}

        sa_family_t getSockAddrFamily();
        sockaddr* getSockAddr();
        socklen_t getSockLen();

        //以下2个方法用于debug 和打日志的
        std::string getIpStr();
        std::string getPortStr();

        static bool isIpv6(string ip);
    private:
        bool mIsSupportIPv6;//是否可以直接使用，能够兼容apple IPv6
        ipaddr_union mAddr;
};


