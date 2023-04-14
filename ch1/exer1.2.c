#include <stdio.h>
#include <stdlib.h>

struct link_info {
    int packet_size_kB;
    int prop_delay_ms;
    float bitrate;
};

struct link_info read_data();
float calc_delay(const struct link_info * const link_info);
void print_results(const float delay, const struct link_info * const link_info);

int main(int argc, char **argv) {

    struct link_info link_info = read_data();

    print_results(calc_delay(&link_info), &link_info);
    
    return 0;
}

struct link_info read_data() {

    struct link_info link_info;

    do {
        puts("Enter packet size (kB)");
        scanf("%d", &(link_info.packet_size_kB));
        if (link_info.packet_size_kB <= 0) {
            system("cls");
            puts("Packet size must be positive");
        }
    } while (link_info.packet_size_kB <= 0);
    system("cls");

    do {
        puts("Enter propagation delay (ms)");
        scanf("%d", &(link_info.prop_delay_ms));
        if (link_info.prop_delay_ms < 0) {
            system("cls");
            puts("Propagation delay must be non negative");
        }
    } while (link_info.prop_delay_ms < 0);
    system("cls");

    do {
        puts("Enter bitrate (kB/s)");
        scanf("%f", &(link_info.bitrate));
        if (link_info.bitrate < 0) {
            system("cls");
            puts("Bitrate must be positive");
        }
    } while (link_info.bitrate <= 0);
    system("cls");

    return link_info;
}

float calc_delay(const struct link_info * link_info) {
    return ( (float) link_info->prop_delay_ms/1000) + (link_info->packet_size_kB/link_info->bitrate);
}

void print_results(const float delay, const struct link_info * const link_info){
    printf( "Packet size:\t %dkB\n"
            "Prop delay\t %.3fsec\n"
            "Bitrate:\t %.3fkB/sec\n"
            "Delay:\t\t %.4fsec",
            link_info->packet_size_kB, ( (float) link_info->prop_delay_ms/1000), link_info->bitrate, delay);
}