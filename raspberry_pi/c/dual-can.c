/***********************************
 * Seengreat Dual-CH CAN HAT demo
 * Author(s):Andy Li from Seengreat
 * ********************************/
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <pthread.h>
#include <time.h>

/****wire connection instructions*****
 * can0             can1
 *  H----------------H
 *  L----------------L
 * GND              GND
 * *****************************/

#define CAN0_ID 0x655
#define CAN1_ID 0x656

void *frame_receive(void *sock)
{
    u_int8_t  i;
    int s;
    struct can_frame frame;
    struct sockaddr_can addr;
    struct ifreq ifr;
    unsigned int len = sizeof(addr);//the len type must be define as "unsigned int"
                                    //otherwise the return value "addr.can_ifindex" is incorrect
    s = *((int *)(&sock));
    while (1)
    {
        recvfrom(s,&frame,sizeof(struct can_frame),0,(struct sockaddr*)&addr,&len);
        ifr.ifr_ifindex = addr.can_ifindex;
        ioctl(s, SIOCGIFNAME,&ifr);

        if(frame.can_id == CAN0_ID)//the frame from can0
        {
            printf("%s had received a can frame from can0 \r\n", ifr.ifr_name);
            printf("  id=0x%X dlc=%d Data: ", frame.can_id, frame.can_dlc);
            for (i = 0; i < frame.can_dlc; ++i)
            {
                printf("0x%02X ", frame.data[i]);
            }
            printf("\n");
        }
        if(frame.can_id == CAN1_ID)//the frame from can1
        {
            printf("%s had received a can frame from can1 \r\n",ifr.ifr_name);
            printf("  id=0x%X dlc=%d Data: ", frame.can_id, frame.can_dlc);
            for (i = 0; i < frame.can_dlc; ++i)
            {
                printf("0x%02X ", frame.data[i]);
            }
            printf("\n");
        }
        
    }
}

int main( void) 
{
    int nbytes = 0;
    pthread_t tid;
    void *tret=NULL;
    struct sockaddr_can sockCan0, sockCan1;
    struct ifreq ifr;
    struct can_frame frame[2] = {{0}, {0}};
    
    system("sudo ifconfig can0 down");
    system("sudo ifconfig can1 down");

    system("sudo ip link set can0 type can bitrate 100000");
    system("sudo ifconfig can0 up");

    system("sudo ip link set can1 type can bitrate 100000");
    system("sudo ifconfig can1 up");
    
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = 0; //Important,the interface index must be 0 £¡£¡£¡
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("S:%d \r\n", s);

    // get the interface index of can0
    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);
    sockCan0.can_family = AF_CAN;
    sockCan0.can_ifindex = ifr.ifr_ifindex;

    // get the interface index of can1
    strcpy(ifr.ifr_name, "can1");
    ioctl(s, SIOCGIFINDEX, &ifr);
    sockCan1.can_family = AF_CAN;
    sockCan1.can_ifindex = ifr.ifr_ifindex;


    // clear filter configure
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    // set filter configure
    struct can_filter rfilter[2];
    rfilter[0].can_id = 0x655;
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = 0x656;
    rfilter[1].can_mask = CAN_SFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, rfilter, sizeof(rfilter));

    // create the receiving thread
    pthread_create(&tid,NULL,frame_receive, (void *)s);
    
    // prepare the message data to be sent to can0
    frame[0].can_id = CAN0_ID;
    frame[0].can_dlc = 8;
    frame[0].data[0] = 0x61;
    frame[0].data[1] = 0x00;
    frame[0].data[2] = 0x22;
    frame[0].data[3] = 0x00;
    frame[0].data[4] = 0x00;
    frame[0].data[5] = 0x00;
    frame[0].data[6] = 0x00;
    frame[0].data[7] = 0x00;

    // prepare the message data to be sent to can1
    frame[1].can_id = CAN1_ID;
    frame[1].can_dlc = 8;
    frame[1].data[0] = 0x61;
    frame[1].data[1] = 0x10;
    frame[1].data[2] = 0x11;
    frame[1].data[3] = 0x00;
    frame[1].data[4] = 0x00;
    frame[1].data[5] = 0x00;
    frame[1].data[6] = 0x00;
    frame[1].data[7] = 0x00;

    // send data to can0 interface, the address is sockCan0
    // because can0 and can1 are connected by wire,this frame will be received by can1
    nbytes = sendto(s, &frame[0], sizeof(struct can_frame),
                0, (struct sockaddr*)&sockCan0, sizeof(sockCan0));

    if (nbytes < sizeof(struct can_frame))
    {
        printf("Error: sendto can0\n");
    }
    
    // send data to can1 interface, the address is sockCan1
    // because can0 and can1 are connected by wire,this frame will be received by can0
    nbytes = 0;
    nbytes = sendto(s, &frame[1], sizeof(struct can_frame),
            0, (struct sockaddr*)&sockCan1, sizeof(sockCan1));
    
    if (nbytes < sizeof(struct can_frame))
    {
        printf("Error: sendto can1\n");
    }

    pthread_join(tid, NULL);

    close(s);
    return 0;

}
