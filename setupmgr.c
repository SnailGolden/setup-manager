#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <systemd/sd-daemon.h>
int main() {
    char *home = getenv("HOME");
    //char cfg[512];
    char line[256];
    //snprintf(cfg, sizeof(cfg), "%s/Documents/startup-config/config", home);
    
    char path[512];
    snprintf(path, sizeof(path), "%s/Documents/startup-config/config", home);
    char *cstart; // command start or command ofset in line
    char *cmd; //command buffer
    int cmdc; //command counter
    int cmdcs; //command success counter
    int prgcs; //success counter for program
    int prgc; //program counter
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        system("echo setupmgr: couldn't load config file! Got NULL from fopen, check logs for more info.'");
    }
    while (fgets(line, sizeof(line), file)) {
        
        if (line[0] == "#") continue; // Skip comments
        if (line[0] == "\n") continue; // Skip empty lines
        if (strncmp(line, "command", 7) == 0) {
            cmdc++;
            line[strcspn(line, "\n")] = 0; // Remove newline character
            cstart = strchr(line, '=');
            cmd = cstart + 1;
            system(cmd); // Execute the command
            cmdcs++;
            continue;
        } else if (strncmp(line, "program", 7) == 0) {
            prgc++;
            line[strcspn(line, "\n")] = 0; // Remove newline character
            cstart = strchr(line, '=');
            cmd = cstart + 1;
            system(cmd); // Execute the command
            prgcs++;
            continue;
        }
    }
    char endcmd[256];
    if (cmdcs != 0 || prgcs != 0) {
        sd_notify(0, "READY=1");
    }
        
    sprintf(endcmd, "echo setupmgr: found %d commands, %d programs. Executed %d commands, %d programs",
            cmdc, prgc, cmdcs, prgcs);

    system(endcmd);
    return 0;
}


