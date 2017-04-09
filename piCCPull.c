//
// Created by shojo1 on 4/4/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>

#define numChamb 3

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
    int airData[numChamb] = { 0 };
    int newAirData[numChamb] = { 0 };
    char* data;
    //char*fakeData ="{\"id_user\":\"2\",\"ac1\":\"58\",\"ac2\":\"0\",\"ac3\":\"0\",\"ac4\":\"0\",\"ac5\":\"0\",\"ac6\":\"0\",\"ac7\":\"0\",\"ac8\":\"0\",\"ac9\":\"0\",\"ac10\":\"0\",\"ac11\":\"0\",\"ac12\":\"0\",\"ac13\":\"0\",\"ac14\":\"0\",\"ac15\":\"0\",\"ac16\":\"0\",\"ac17\":\"0\",\"ac18\":\"0\",\"bin\":\"1\",\"piOrSever\":\"1\"}";

    int running = 1;
    while(running) {
        //curl the results to bedbot interface
        data = handle_url("http://ec2-54-204-185-188.compute-1.amazonaws.com/api/getAirData.php");

        //skip user id
        data = strstr(data, ":\"");
        data=data+2;
        char *result2= strstr(data, "\"");
        memcpy( &data, &result2, sizeof(&data) );
        //convert data char * to int array
        for(int i=0; i<numChamb; i++) {
            data = strstr(data, ":\"");
            data=data+2;
            char *result2= strstr(data, "\"");
            int length = result2 - data;
            char subbuff[length+1];
            memcpy( subbuff, data, length );
            subbuff[length] = '\0';
            sscanf(subbuff, "%d", &newAirData[i]);
            printf("air chamber %d: %d \n", i+1, newAirData[i]);
            memcpy( &data, &result2, sizeof(&data) );
        }


        if(data) {
            if (memcmp(airData, newAirData, sizeof(airData)) != 0){
                //the air data changed
                memcpy(airData, newAirData, sizeof(airData));
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