/*
int sendMessage(char* buf, int len, char* ip)
{
    
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockFd < 0)
    {
        printf("Error: create udp send socket failed !!!\n");
        return -1;
    }
    
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(5060);
    clientAddr.sin_addr.s_addr = inet_addr("192.168.0.1");
    if(bind(sockFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)))
    {
        printf("Error: cannot bind the udp port\n");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5060);
    ip = "6.6.6.6";
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    if(connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error: cannot connect the udp port\n");
        return -1;
    }

    int length = int(write(sockFd, buf, len));
    if(length < 0)
    {
        printf("send socket failed !!!");
    }

}
*/
