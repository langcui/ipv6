#include "../include/socketaddress.h"
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







