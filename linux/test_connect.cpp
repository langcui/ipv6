#include "../include/socketaddress.h"

int GetIpUnion(socketaddress &sockAddr, int domain, string sIp, int port)
{
    unsigned char buf[sizeof(struct in6_addr)];
    char str[INET6_ADDRSTRLEN];
    int s = inet_pton(domain, sIp.c_str(), buf);
    if (s <= 0) {
        if (s == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", str);

    if(AF_INET == domain)
    {
        sockaddr_in addr4;
        addr4.sin_family = AF_INET; 
        memcpy(&addr4.sin_addr, buf, sizeof(struct in_addr));
        sockAddr.setInet(addr4);
        sockAddr.setPort(port);
    }

    if(AF_INET6 == domain)
    {
        sockaddr_in6 addr6;
        //addr6.sin6_len = sizeof(struct sockaddr_in6);
        printf("addr6.sin6_scope_id[%d].\n", addr6.sin6_scope_id);
        addr6.sin6_scope_id = 0;
        addr6.sin6_family = AF_INET6;
        memcpy(&addr6.sin6_addr, buf, sizeof(struct in6_addr));
        sockAddr.setInet6(addr6);
        sockAddr.setPort(port);
    }

    return 0;
}

int TestConnect( socketaddress sockAddr)
{
    int family[] = {AF_INET, AF_INET6};
    int sockettype[] = {SOCK_STREAM, SOCK_DGRAM};

    for(int i = 0; i < sizeof(family)/sizeof(int); ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            errno = 0;
            printf("------------\n");
            printf("family[%d], sockaddr.family[%d], AF_INET[%d], AF_INET6[%d], AF_UNSPEC[%d].\n"
                    , family[i], sockAddr.getSockAddrFamily(), AF_INET, AF_INET6, AF_UNSPEC);
            printf("sockettype[%d], SOCK_STREAM[%d], SOCK_DGRAM[%d].\n", sockettype[j], SOCK_STREAM, SOCK_DGRAM);
            int fd = socket(family[i], sockettype[j], 0);
            printf("socket:fd[%d], errno[%d], strerror[%s].\n", 0, errno, strerror(errno));

            int ret = connect(fd, sockAddr.getSockAddr(), sockAddr.getSockLen());
            printf("connect ret[%d], socketlen[%d], errno[%d], strerror[%s].\n", ret, sockAddr.getSockLen(), errno, strerror(errno));

            char buf[100] = {0};
            if(sockettype[j] == SOCK_STREAM)
            {
                int size = send(fd, buf, 100, 0);
                printf("send size[%d], errno[%d], strerror[%s].\n", size, errno, strerror(errno));
            } else 
            {
                int size = sendto(fd, buf, 100, 0, sockAddr.getSockAddr(), sockAddr.getSockLen());
                printf("sendto size[%d], errno[%d], strerror[%s].\n", size, errno, strerror(errno));
            }

            printf("------------\n");
            close(fd);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    socketaddress sockAddr;

    int domain, s;
    char str[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    string sIp = argv[1];
    int port = atoi(argv[2]);
    domain =  socketaddress::isIpv6(sIp) ? AF_INET6 : AF_INET;
    printf("input ip[%s], port[%d], domain[%d].\n", sIp.c_str(), port, domain);

    GetIpUnion(sockAddr, domain, sIp, port);
    printf("socketaddress:ai_family[%d], port[%d][%s], ip[%s].\n"
            , sockAddr.getSockAddrFamily(), sockAddr.getPort(), sockAddr.getPortStr().c_str(), sockAddr.getIpStr().c_str());

    TestConnect(sockAddr);

    exit(EXIT_SUCCESS);
}


