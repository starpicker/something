#include "capture.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pcap.h"
#include "misc.h"

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN                1518*2

/* ethernet headers are always exactly 14 bytes [1] */
// equal to sizeof(sniff_ethernet)
#define SIZE_ETHERNET           14

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN          6

/* Ethernet header */
struct sniff_ethernet
{
    u_char   ether_dhost[ETHER_ADDR_LEN]; /* destination host address */
    u_char   ether_shost[ETHER_ADDR_LEN]; /* source host address */
    u_short  ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip
{
    u_char  ip_vhl; /* version << 4 | header length >> 2 */
    u_char  ip_tos; /* type of service */
    u_short ip_len; /* total length */
    u_short ip_id; /* identification */
    u_short ip_off; /* fragment offset field */
#define IP_RF 0x8000 /* reserved fragment flag */
#define IP_DF 0x4000 /* dont fragment flag */
#define IP_MF 0x2000 /* more fragments flag */
#define IP_OFFMASK 0x1fff /* mask for fragmenting bits */
    u_char  ip_ttl; /* time to live */
    u_char  ip_p; /* protocol */
    u_short ip_sum; /* checksum */

    struct in_addr ip_src,ip_dst; /* source and dest address */
};

#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)  (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp
{
    u_short th_sport; /* source port */
    u_short th_dport; /* destination port */
    tcp_seq th_seq; /* sequence number */
    tcp_seq th_ack; /* acknowledgement number */
    u_char th_offx2; /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win; /* window */
    u_short th_sum; /* checksum */
    u_short th_urp; /* urgent pointer */
};

void print_hex_ascii_line(const u_char* payload, int len, int offset);
void print_payload(const u_char* payload, int len);
void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet);

/*
 * print data in rows of 16 bytes: offset hex ascii
 *
 * 00000 47 45 54 20 2f 20 48 54 54 50 2f 31 2e 31 0d 0a GET / HTTP/1.1..
 */
void print_hex_ascii_line(const u_char* payload, int len, int offset)
{
    int i;
    int gap;
    const u_char* ch;

    /* offset */
    printf("%05d ", offset);

    /* hex */
    ch = payload;
    for(i = 0; i < len; i++)
    {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if(i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if(len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if(len < 16)
    {
        gap = 16 - len;
        for(i = 0; i < gap; i++)
        {
            printf(" ");
        }
    }
    printf(" ");

    /* ascii (if printable) */
    ch = payload;
    for(i = 0; i < len; i++)
    {
        if(isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");

    return;
}

void print_payload_ascii(const u_char* payload, int len)
{
    const u_char* ch;
    /* ascii (if printable) */
    ch = payload;
    for(auto i = 0; i < len; i++)
    {
        if(isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    fflush(stdout);
}

/*
 * print packet payload data (avoid printing binary data)
 */
void print_payload(const u_char* payload, int len)
{

    int len_rem = len;
    int line_width = 16;            /* number of bytes per line */
    int line_len;
    int offset = 0;                    /* zero-based offset counter */
    const u_char* ch = payload;

    if(len <= 0)
        return;

    /* data fits on one line */
    if(len <= line_width)
    {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for( ;; )
    {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if(len_rem <= line_width)
        {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }

    return;
}

void inet_ntoa_b(struct in_addr inetAddress, char* pString)
{
    unsigned char* pIp = (unsigned char*)&inetAddress;
    char* pFirst;
    char* pLast;
    char temp;
    unsigned long digit, mod;

    int i;

    if(pString == NULL)
    {
        return;
    }

    for(i = 0; i < 4; i++)
    {
        digit = pIp[i];

        pFirst = pString;
        do
        {
            mod = digit;

            digit /= 10;
            mod -= digit * 10;

            *pString++ = (unsigned char)(mod + '0');
        }while(digit !=0);

        pLast = pString - 1;


        temp = *pFirst;
        *pFirst = *pLast;
        *pLast = temp;

        *pString++ = '.';
    }

    *(pString-1) = '\0';
}

/*
 * dissect/print packet
 */
void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet)
{
    /* declare pointers to packet headers */
    //const struct sniff_ethernet* ethernet; /* The ethernet header [1] */
    const struct sniff_ip* ip; /* The IP header */
    const struct sniff_tcp* tcp; /* The TCP header */
    const u_char* payload; /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;

    string keyword = *((string*)args);

    /* define ethernet header */
    //ethernet = (struct sniff_ethernet*)(packet);

    /* define/compute ip header offset */
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if(size_ip < 20)
    {
        rprintf("Invalid IP header length: %u bytes\n", size_ip);

        return;
    }

    /* determine protocol */
    switch(ip->ip_p)
    {
        case IPPROTO_TCP:
            //printf(" Protocol: TCP\n");
            break;
        case IPPROTO_UDP:
            //printf(" Protocol: UDP\n");

            return;
        case IPPROTO_ICMP:
            //printf(" Protocol: ICMP\n");

            return;
        case IPPROTO_IP:
            //printf(" Protocol: IP\n");

            return;
        default:
            //printf(" Protocol: unknown\n");

            return;
    }

    /*
     * OK, this packet is TCP.
     */

    /* define/compute tcp header offset */
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if(size_tcp < 20)
    {
        rprintf("Invalid TCP header length: %u bytes\n", size_tcp);

        return;
    }

    /* define/compute tcp payload (segment) offset */
    payload = (u_char*)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    /*
     * Print payload data; it might be binary, so don't just
     * treat it as a string.
     */
    if(size_payload > 0)
    {
        if(keyword.size() && string((const char*)payload, size_payload).find(keyword) == string::npos)
            return;

        char srcIp[32];
        char dstIp[32];
        inet_ntoa_b(ip->ip_src, srcIp);
        inet_ntoa_b(ip->ip_dst, dstIp);

        /* print source and destination IP addresses */
        MLLOG("%s:%d => %s:%d", srcIp/*inet_ntoa(ip->ip_src)*/, ntohs(tcp->th_sport),
                                        dstIp/*inet_ntoa(ip->ip_dst)*/, ntohs(tcp->th_dport));

        print_payload_ascii(payload, size_payload);
        //print_payload(payload, size_payload);
    }

    return;
}

int capture(const string srcIp, short sourcePort,
               const string dstIp, short dstPort,
               int number, const string keyword)
{
    char* dev = NULL;                     /* capture device name */
    char errbuf[PCAP_ERRBUF_SIZE];        /* error buffer */
    pcap_t* handle;                       /* packet capture handle */

    string srcIp_filter;
    string sourcePort_filter;
    string dstIp_filter;
    string dstPort_filter;

    if(srcIp.size())       srcIp_filter          = string("(src host ") + srcIp                      + ") and ";
    if(sourcePort)         sourcePort_filter     = string("(src port ") + std::to_string(sourcePort) + ") and ";
    if(dstIp.size())       dstIp_filter          = string("(dst host ") + dstIp                      + ") and ";
    if(dstPort)            dstPort_filter        = string("(dst port ") + std::to_string(dstPort)    + ") and ";

    string filter = (srcIp_filter + sourcePort_filter + dstIp_filter + dstPort_filter + "tcp");
    const char* filter_exp = filter.c_str();

    struct bpf_program fp;                /* compiled filter program (expression) */
    bpf_u_int32 mask;                     /* subnet mask */
    bpf_u_int32 net;                      /* ip */
    int num_packets = number;             /* number of packets to capture */

    dev = pcap_lookupdev(errbuf);
    if(dev == NULL)
    {
        fprintf(stderr, "Couldn't find default device: %s\n",
         errbuf);

        return -1;
    }

    /* get network number and mask associated with capture device */
    if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* print capture info */
    MLLOG("start capturing packet:\n    device: %s, number of packets to be captured: %d, filter expression: %s\n",
                    dev, num_packets, filter_exp);

    /* open capture device */
    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if(handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);

        return -1;
    }

    /* make sure we're capturing on an Ethernet device [2] */
    if(pcap_datalink(handle) != DLT_EN10MB)
    {
        fprintf(stderr, "%s is not an Ethernet\n", dev);

        return -1;
    }

    /* compile the filter expression */
    if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
                filter_exp, pcap_geterr(handle));

        return -1;
    }

    /* apply the compiled filter */
    if(pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
                filter_exp, pcap_geterr(handle));

        return -1;
    }

    MLLOG("capture start\n->->->->->->->->->->");

    /* now we can set our callback function */
    pcap_loop(handle, num_packets, got_packet, (u_char*)(&keyword));

    MLLOG("<-<-<-<-<-<-<-<-<-<-\ncapture complete");

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);

    return 0;

}
