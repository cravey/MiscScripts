/*
Takes either an ip or subnet in / notation as the first argument. Takes a subnet in slash notation as the second argument.
returns 0 for 1 is member of 2. returns 1 if 1 is NOT a member of 2.
Thx ChatGPT
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

int is_subnet_member(char* ip_or_subnet, char* subnet) {
    struct in_addr ip, subnet_ip, subnet_mask;

    if (inet_pton(AF_INET, ip_or_subnet, &ip) <= 0) {
        // Failed to parse IP address, assume it's a subnet
        if (inet_pton(AF_INET, strtok(ip_or_subnet, "/"), &subnet_ip) <= 0) {
            fprintf(stderr, "Invalid IP or subnet: %s\n", ip_or_subnet);
            return -1;
        }

        int prefix_length = atoi(strtok(NULL, "/"));
        if (prefix_length < 0 || prefix_length > 32) {
            fprintf(stderr, "Invalid subnet prefix length: %d\n", prefix_length);
            return -1;
        }

        // Create a subnet mask from the prefix length
        subnet_mask.s_addr = htonl(~((1 << (32 - prefix_length)) - 1));
    } else {
        // It's an IP address, convert it to a /32 subnet
        subnet_ip = ip;
        subnet_mask.s_addr = 0xFFFFFFFF;
    }

    // Parse the second argument as a subnet
    if (inet_pton(AF_INET, strtok(subnet, "/"), &subnet_ip) <= 0) {
        fprintf(stderr, "Invalid subnet: %s\n", subnet);
        return -1;
    }

    int prefix_length = atoi(strtok(NULL, "/"));
    if (prefix_length < 0 || prefix_length > 32) {
        fprintf(stderr, "Invalid subnet prefix length: %d\n", prefix_length);
        return -1;
    }

    // Create a subnet mask from the prefix length
    subnet_mask.s_addr = htonl(~((1 << (32 - prefix_length)) - 1));

    // Check if the IP/subnet is a member of the given subnet
    return ((ip.s_addr & subnet_mask.s_addr) == (subnet_ip.s_addr & subnet_mask.s_addr)) ? 0 : 1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP or subnet> <subnet>\n", argv[0]);
        return 1;
    }

    int result = is_subnet_member(argv[1], argv[2]);
    if (result == 0) {
        printf("The IP/subnet is a member of the given subnet.\n");
    } else if (result == 1) {
        printf("The IP/subnet is not a member of the given subnet.\n");
    } else {
        return 1; // Error occurred
    }

    return 0;
}
