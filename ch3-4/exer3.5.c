#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int P;
float C;
float PROP;
double BEP;
int max_err;
float timeout;

void read_data();
float calc_D_suc();
float calc_D_err();
double p_suc();
int combinations(size_t n, size_t k);
int factorial(size_t k);

int main(int argc, char **argv) {

    read_data();

    // Η επιλογή το ACK ναι είναι 10% το μέγεθος του πακέτου είναι τυχαία
    int ack_size = 0.1*P;
    // time to send + time to arrive + ack time to send + ack time to arrive
    timeout = (float) ((P + ack_size) / C) + 2*PROP;

    // Calculate delay
    double D_err = calc_D_err(), D_suc = calc_D_suc();

    // Print estimated delay, timeout
    printf( "Delay_total:\t%.3f \n"
            "Delay_err:\t%.3f \n"
            "Delay_suc:\t%.3f \n"
            "Timeout:\t%.3f \n", 
            D_suc + D_err, D_err, D_suc, timeout);

    system("pause");

    return 0;
}

// packet_size / link_speed + time_to_arrive
float calc_D_suc() {
    return (float) (P / C) + PROP;
}

// time_per_retry * number_of_average_retriess
// (1-p_suc)/p_suc == 1/p_suc - 1
float calc_D_err() {
    return timeout * ((double) (1/p_suc()) - 1);
} 

// Using binomial distribution, counts errors
// P_suc = P(X = 0) + ... + P(X = max_errors)
// P(X = k) = combination(n, k) * BEP^k * (1-BEP)^(n-k)
double p_suc() {

    double prob_success = 0;
    for (size_t i = 0; i <= max_err; i++) 
        prob_success += combinations(P, i) * pow(BEP, i) * pow(1 - BEP, P-i);
    return prob_success;
}

// Simplified combinations
// n!/(k! * (n-k)!) == n * (n-1) * ... * (n - k + 1)/ k!
int combinations(size_t n, size_t k) {

    double result = (double) 1/factorial(k);
    while (k > 0) {
        result *= n - k + 1;
        k--;
    }
    return (int) result;
}

int factorial(size_t k) {

    if (k == 0)
        return 1;
    else 
        return k * factorial(k-1);
}

void read_data() {

    puts("Input packet size in bits");
    do {
        scanf("%d", &P);
        if (P <= 0) {
            system("cls");
            puts("Size must be positive integer");
        }
    } while (P <= 0);
    system("cls");

    puts("Input link bitrate");
    do {
        scanf("%f", &C);
        if (C <= 0) {
            system("cls");
            puts("Bitrate must be positive real");
        }
    } while (C <= 0);
    system("cls");

    puts("Input propagation delay");
    do {
        scanf("%f", &PROP);
        if (PROP < 0) {
            system("cls");
            puts("Delay must non negative real");
        }
    } while (PROP < 0);
    system("cls");

    puts("Input bit error probability");
    do {
        scanf("%lf", &BEP);
        if (BEP < 0 || BEP > 1) {
            system("cls");
            puts("Error probability must between 0 and 1");
        }
    } while (BEP < 0 || BEP > 1);
    system("cls");

    puts("Input max allowed errors in packet");
    do {
        scanf("%d", &max_err);
        if (max_err < 0 || max_err > 2) {
            system("cls");
            puts("Maximum errors must be between 0 and 2");
        }
    } while (max_err < 0 || max_err > 2);
    system("cls");
}