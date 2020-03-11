#include "../include/socketaddress.h"

int GetIpUnion(socketaddress &sockAddr, int domain, string sIp, int port)
{
    char *addr1 = strdup(sIp.c_str());

    if(AF_INET == domain)
    {
        struct  in_addr in4;
        inet_pton(AF_INET, addr1, &in4);
        
        sockaddr_in addr4;
        addr4.sin_family = AF_INET;
        addr4.sin_addr = in4;

        sockAddr.setInet(addr4);
        sockAddr.setPort(port);
    }

    if(AF_INET6 == domain)
    {
        struct in6_addr ip;
        inet_pton(AF_INET6, addr1, &ip);

        struct sockaddr_in6 in6;
        in6.sin6_addr  = ip;
        in6.sin6_family = AF_INET6;
        in6.sin6_scope_id = 0;
        in6.sin6_flowinfo = 0;
        // in6.sin6_len = sizeof(struct sockaddr_in6); 安卓没有这个值

        sockAddr.setInet6(in6);
        sockAddr.setPort(port);
    }

    return 0;
}



int GetIpUnionOld(socketaddress &sockAddr, int domain, string sIp, int port)
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
    // int family[] = {AF_INET6, AF_INET};
    int sockettype[] = {SOCK_STREAM, SOCK_DGRAM};
    //int sockettype[] = {SOCK_DGRAM, SOCK_STREAM};

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

            int fflags = fcntl(fd, F_GETFL);
            printf("fcntl get fflags[%d], O_NONBLOCK[%0x].\n", fflags, O_NONBLOCK);
            fflags &= ~O_NONBLOCK;
            fflags |= MSG_NOSIGNAL;
            int status = fcntl(fd, F_SETFL, fflags);
            printf("fcntl return[%d], set fflags[%d], ~O_NONBLOCK[%0x].\n", status, fflags, ~O_NONBLOCK);
            signal(SIGPIPE, SIG_IGN);
            int ret = connect(fd, sockAddr.getSockAddr(), sockAddr.getSockLen());
            printf("connect[%d][%d], [%s].\n", ret, errno, strerror(errno));
            char buf[100] = {0};
            if(sockettype[j] == SOCK_STREAM)
            {
                printf("send i[%d], j[%d].\n", i, j );
                int size = send(fd, buf, 100, 0);
                printf("send return[%d] i[%d], j[%d].\n", size,  i, j );
                printf("send[%d][%d], [%s].\n", size, errno, strerror(errno));
            } else 
            {
                printf("sendto i[%d], j[%d].\n", i, j );
                int size = sendto(fd, buf, 100, 0, sockAddr.getSockAddr(), sockAddr.getSockLen());
                printf("sendto return[%d] i[%d], j[%d].\n", size,  i, j );
                printf("sendto[%d][%d], [%s].\n", size, errno, strerror(errno));
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


