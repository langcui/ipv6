#include "../include/socketaddress.h"

void socketaddress::setsockaddrsv46(std::string sIp, uint32_t port)
{
    if(sIp.empty() || (sIp == "0"))
    {
        sIp = "0.0.0.0";
    }

#ifdef __APPLE__  // ios 需要将ipv4地址转成ipv6地址: ::ffff:220.181.38.148
    if( ! isIpv6(sIp) )
    {
        sIp = "::ffff:" + sIp;
    }
#endif

    int domain = isIpv6(sIp) ? AF_INET6 : AF_INET;
    char *addr1 = strdup(sIp.c_str());
    if(AF_INET == domain)
    {
        struct  in_addr in4;
        memset(&in4, sizeof(in4), 0);
        int iRet = inet_pton(AF_INET, addr1, &in4);
        printf("inet_pton return[%d], AF_INET, addr1[%s].\n", iRet, addr1);

        sockaddr_in addr4;
        memset(&addr4, sizeof(addr4), 0);
        addr4.sin_family = AF_INET;
        addr4.sin_addr = in4;

        setInet(addr4);
        setPort(port);
    }

    if(AF_INET6 == domain)
    {
        struct in6_addr in6;
        memset(&in6, sizeof(in6), 0);
        int iRet = inet_pton(AF_INET6, addr1, &in6);
        printf("inet_pton return[%d], AF_INET, addr1[%s].\n", iRet, addr1);

        struct sockaddr_in6 addr6;
        memset(&addr6, sizeof(addr6), 0);
        addr6.sin6_addr  = in6;
        addr6.sin6_family = AF_INET6;
        addr6.sin6_scope_id = 0;
        addr6.sin6_flowinfo = 0;

#ifdef __APPLE__
        addr6.sin6_len = sizeof(struct sockaddr_in6); 安卓没有这个值
#endif

        setInet6(addr6);
        setPort(port);
    }
}


void socketaddress::setInet(sockaddr_in addr4)
{
    mAddr.sin = addr4;
}

void socketaddress::setInet6(sockaddr_in6 addr6)
{
    mAddr.sin6 = addr6;
}

void socketaddress::setPort(uint16_t nport)
{
    if(mAddr.sa.sa_family == AF_INET)
    {
        mAddr.sin.sin_port = htons(nport);
    }else if(mAddr.sa.sa_family == AF_INET6)
    {
        mAddr.sin6.sin6_port = htons(nport);
    }
}

uint16_t socketaddress::getPort()
{
    if(mAddr.sa.sa_family == AF_INET)
    {
        return ntohs(mAddr.sin.sin_port);
    }else if(mAddr.sa.sa_family == AF_INET6)
    {
        return ntohs(mAddr.sin6.sin6_port);
    }
    return 0;
}

sa_family_t socketaddress::getSockAddrFamily()
{
    return mAddr.sa.sa_family;
}

sockaddr* socketaddress::getSockAddr()
{
    return &mAddr.sa;
}

socklen_t socketaddress::getSockLen()
{
    socklen_t sl = 0;
    if(mAddr.sa.sa_family == AF_INET)
    {
        sl = sizeof(sockaddr_in);
    }
    else if(mAddr.sa.sa_family == AF_INET6)
    {
        sl = sizeof(sockaddr_in6);
    }
    return sl;
}

std::string socketaddress::getIpStr()
{
    std::string resStr = "";
    if(mAddr.sa.sa_family == AF_INET)
    {
        char addr1[INET_ADDRSTRLEN] = {'\0'};
        ::inet_ntop(AF_INET,&(((sockaddr_in*)(&mAddr.sin))->sin_addr), addr1, sizeof(addr1));
        resStr = std::string(addr1);
    }
    else if(mAddr.sa.sa_family == AF_INET6)
    {
        char addr2[INET6_ADDRSTRLEN] = {'\0'};
        ::inet_ntop(AF_INET6,&(((sockaddr_in6*)(&mAddr.sin6))->sin6_addr), addr2, sizeof(addr2));
        resStr = std::string(addr2);
    }
    return resStr;
}

std::string socketaddress::getPortStr()
{
    uint16_t hport = 0;// = htons(hport);
    if(mAddr.sa.sa_family == AF_INET)
    {
        hport = ntohs(mAddr.sin.sin_port);
    }
    else if(mAddr.sa.sa_family == AF_INET6)
    {
        hport = ntohs(mAddr.sin6.sin6_port );
    }
    std::stringstream ss;
    ss << hport;
    return ss.str();
}

bool socketaddress::isIpv6(string ip)
{
    return ip.find(":") != string::npos;
}







