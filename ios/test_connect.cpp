#include "../include/socketaddress.h"

int GetIpUnion(socketaddress &sockAddr, int domain, string sIp, int port)
{

    struct in6_addr ip;
    char *addr1 = strdup(sIp.c_str());
    inet_pton(AF_INET6, addr1, &ip);

    struct sockaddr_in6 in6;
    in6.sin6_addr  = ip;
    in6.sin6_family = AF_INET6;
    in6.sin6_len = sizeof(struct sockaddr_in6);
    in6.sin6_scope_id = 0;
    in6.sin6_flowinfo = 0;
    
    sockAddr.setInet6(in6);
    sockAddr.setPort(port);

    return 0;
}

int TestConnect( socketaddress sockAddr)
{
    int family[] = {AF_INET6};
    int sockettype[] = {SOCK_STREAM, SOCK_DGRAM};
    // int sockettype[] = {SOCK_DGRAM, SOCK_STREAM};

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


            signal(SIGPIPE, SIG_IGN);

            char buf[100] = {0};
            if(sockettype[j] == SOCK_STREAM)
            {
                int ret = connect(fd, sockAddr.getSockAddr(), sockAddr.getSockLen());
                printf("connect getIpStr[%s].\n", sockAddr.getIpStr().c_str());
                printf("connect ret[%d], socketlen[%d], errno[%d], strerror[%s].\n", ret, sockAddr.getSockLen(), errno, strerror(errno));
                int size = send(fd, buf, 100, 0);
                printf("send size[%d], errno[%d], strerror[%s].\n", size, errno, strerror(errno));
            } else 
            {
                socketaddress mysocketaddress;
                sockaddr_in6 myaddr6;
                memset(&myaddr6, 0, sizeof(myaddr6));
                //addr6.sin6_len = sizeof(struct sockaddr_in6);
                printf("myaddr6.sin6_scope_id[%d].\n", myaddr6.sin6_scope_id);
                myaddr6.sin6_len = sizeof(sockaddr_in6);
                myaddr6.sin6_scope_id = 0;
                myaddr6.sin6_family = AF_INET6;
                myaddr6.sin6_flowinfo = 0;
                myaddr6.sin6_addr = in6addr_any;

                mysocketaddress.setInet6(myaddr6);
                mysocketaddress.setPort(9989);

                printf("before bind ip[%s], port[%s].\n", mysocketaddress.getIpStr().c_str(), mysocketaddress.getPortStr().c_str());
                int ret = bind(fd, (const struct sockaddr *)mysocketaddress.getSockAddr(), mysocketaddress.getSockLen());
                printf("bind ret[%d], socketlen[%d], errno[%d], strerror[%s].\n", ret, mysocketaddress.getSockLen(), errno, strerror(errno));
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
    //if(AF_INET == domain)
    //   sIp = "::ffff:" + sIp;    
    //    sIp = "64:ff9b::" + sIp;    

    // domain = AF_INET6;
    printf("input ip[%s], port[%d], domain[%d].\n", sIp.c_str(), port, domain);

    GetIpUnion(sockAddr, domain, sIp, port);
    printf("socketaddress:ai_family[%d], port[%d][%s], ip[%s].\n"
            , sockAddr.getSockAddrFamily(), sockAddr.getPort(), sockAddr.getPortStr().c_str(), sockAddr.getIpStr().c_str());

    TestConnect(sockAddr);

    exit(EXIT_SUCCESS);
}


