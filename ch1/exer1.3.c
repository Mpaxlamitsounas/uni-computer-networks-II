#include <stdio.h>
#include <stdlib.h>

struct network_info {
    int node_count;
    float bitrate;
    int packet_size;
    int edge_node_conn;
};

struct network_info read_data();
float * calc_delay(const struct network_info * const network_info);
void print_results(const float * const delay);

int main(int argc, char **argv) {

    struct network_info network_info = read_data();

    print_results(calc_delay(&network_info));
    
    return 0;
}

struct network_info read_data() {

    struct network_info network_info;

    do {
        puts("Enter network node count");
        scanf("%d", &(network_info.node_count));
        if (network_info.node_count <= 0) {
            system("cls");
            puts("Node count must be positive");
        }
    } while (network_info.node_count <= 0);
    system("cls");

    do {
        puts("Enter bitrate\nMake sure data size unit matches that of packet");
        scanf("%f", &(network_info.bitrate));
        if (network_info.bitrate <= 0) {
            system("cls");
            puts("Bitrate must be positive");
        }
    } while (network_info.bitrate <= 0);
    system("cls");

    do {
        puts("Enter packet size");
        scanf("%d", &(network_info.packet_size));
        if (network_info.packet_size <= 0) {
            system("cls");
            puts("Packet size must be positive");
        }
    } while (network_info.packet_size <= 0);
    system("cls");

    do {
        puts("Enter edge server node connection");
        scanf("%d", &(network_info.edge_node_conn));
        if (network_info.edge_node_conn <= 0 || network_info.edge_node_conn > network_info.node_count) {
            system("cls");
            printf("The connection must be with a node between 1 and node count (%d)\n", network_info.node_count);

        }
    } while (network_info.edge_node_conn <= 0 || network_info.edge_node_conn > network_info.node_count);
    system("cls");

    return network_info;
}

float * calc_delay(const struct network_info * network_info) {
    return (float[2]) { 
        (network_info->node_count * network_info->packet_size) / network_info->bitrate, 
        (network_info->edge_node_conn * network_info->packet_size) / network_info->bitrate
    };
}

void print_results(const float * const delay){
    printf( "To server:\t %.3fsec\n"
            "To edge:\t %.3fsec\n"
            "Difference:\t %.3fsec\n",
            delay[0], delay[1], delay[0]-delay[1]);
}