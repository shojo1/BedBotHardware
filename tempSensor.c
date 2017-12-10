#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * temp_raw(){
    //Read the data from sensor
    FILE * pFile;
    char * buffer = 0;
    long length;
    pFile = fopen (temp_sensor,"r");
    if (pFile!=NULL){
        fseek (pFile, 0, SEEK_END);
        length = ftell (pFile);
        fseek (pFile, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, pFile);
        }
        fclose (pFile);
    }
    return buffer;
}

double read_temp(){
    char * buffer = 0;
    buffer = temp_raw();
    //process the data to extract the temperature
    if (buffer){
        printf("The buffer %s\n", buffer);
    }

    char* pch = NULL;
    pch = strtok(buffer, "\r\n");
    size_t size;
    char *endFirstLine;
    size = strlen(pch);
    if (!size) {
        printf("this is pch: %s\n", pch);
        return -1;
    }

    endFirstLine = pch + size - 4;
    while(!strncmp(endFirstLine, "YES", 3)){
        sleep(200);
        printf("nope, endline: %s\n", endFirstLine);
        buffer=temp_raw();
        pch = strtok(buffer, "\r\n");
        size_t size;
        char *endFirstLine;
        size = strlen(pch);
        if (!size) {
            printf("this is pch: %s\n", pch);
            return -1;
        }
        endFirstLine = buffer + size - 4;
    }
    pch = strtok(NULL, "\r\n");
    printf("this is pch: %s\n", pch);
    char* pch2 = NULL;
    pch2=strchr(pch,'=');
    char temp[6];
    memcpy( temp, &pch2[1], 5 );
    temp[5] = '\0';
    printf("temperature: %s\n", temp);

    int int_temp;
    sscanf(temp, "%d", &int_temp);
    double temp_c = int_temp/1000.0;
    double temp_f = temp_c * 9.0 / 5.0 + 32.0;
    return temp_f;
}


int main() {
    //the temperature sensor uses One Wire protocol. Enable this on the pi.
    system("modprobe w1-gpio");
    system("modprobe w1-therm");

    char temp_sensor[] = "sys/bus/w1/devices/28-000005e2fdc3/w1_slave";

    //use the methods defined above to get pi data.
}
