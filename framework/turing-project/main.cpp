#include <TuringMachine.h>
#include <unistd.h>
#include <getopt.h>
#include <cstdlib>
using namespace std;

static option longOpts[] = {
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {NULL, no_argument, NULL, '\0'}
};

int main(int argc, char* argv[]){
    
    // Disable automatic errors
    // opterr = 0;

    int opt;
    int verbose = 0;
    while ((opt = getopt_long(argc, argv, "hv", longOpts, NULL)) != -1) {
        switch (opt)
        {
        case '?':
            fprintf(stderr, "usage: %s [-v|--verbose] [-h|--help] <tm> <input>\n", argv[0]);
            return -1;
        case 'h':
            fprintf(stdout, "usage: %s [-v|--verbose] [-h|--help] <tm> <input>\n", argv[0]);
            if (argc > 2) {
                fprintf(stderr, "Warning: redundant arguments\n");
                return -1;
            }
            return 0;
        case 'v':
            verbose = 1;
            break;
        default:
            break;
        }
    }

    if (argc < optind + 1) {
        fprintf(stderr, "Turing machine file not specified!\nusage: %s [-v|--verbose] [-h|--help] <tm> <input>\n", argv[0]);
        return -1;
    }
    if (argc < optind + 2) {
        fprintf(stderr, "input string not specified!\nusage: %s [-v|--verbose] [-h|--help] <tm> <input>\n", argv[0]);
        return -1;
    }
        
    if (argc > optind + 2) {
        fprintf(stderr, "Warning: redundant arguments\nusage: %s [-v|--verbose] [-h|--help] <tm> <input>\n", argv[0]);
        return -1;
    }

    char *tm_str = argv[optind];
    char *input_str = argv[optind + 1];
    if (access(tm_str, 4) != 0 && access(tm_str, 6) != 0) {
        fprintf(stderr, "Turing Machine file not readable!\n");
        return -1;
    }
    auto tm = TuringMachine(tm_str);
    tm.Run(input_str, verbose);

    return 0;
}