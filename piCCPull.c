//
// Created by shojo1 on 4/4/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>

struct url_data {
    size_t size;
    char* data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char* url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

int main() {
    //number of air chambers
    int const numChamb=3;
    int airData[numChamb] = { 0 };
    int newAirData[numChamb] = { 0 };
    char* data;

    int running = 1;
    while(running) {
        //curl the results to bedbot interface
        data = handle_url("http://ec2-54-204-185-188.compute-1.amazonaws.com/api/getAirData.php");

        //convert data char * to int array
        for(int i; i<numChamb; i++) {
            char *pch = NULL;
            pch = strtok(data, ", ");
            size_t size;
            char *endFirstLine;
            size = strlen(pch);
            sscanf(pch, "%d", &newAirData[i]);
        }
        char myarray[5] = {'-', '1', '2', '3', '\0'};
        int i;


        if(data) {
            if (memcmp(airData, data, sizeof(airData)) != 0){
                //the air data changed
                airData=data;
            }
            free(data);
        }
        //sleep 5 seconds and
        sleep(5);
//        else{
//            running=false;
//        }
    }
    curl_global_cleanup();
    return 0;
}