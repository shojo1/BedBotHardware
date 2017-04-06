//
// Created by shojo1 on 4/4/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>

int main() {
    //open file that stores bin number
    char bin[5] = "12345";
    //run temp sensor
    double temp =98.8;
    //run health sensor
    double rate =92.8;
    double oxygen = 42.3;

    //curl the results to bedbot interface
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        //rate, oxygen, temp, timeSent
        curl_easy_setopt(curl, CURLOPT_URL, "http://ec2-54-204-185-188.compute-1.amazonaws.com/api/setBioData.php");
        /* Now specify the POST data */
        time_t timeSent = time(NULL);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "bin="+bin+"&rate="+rate+"&oxygen="+oxygen+"&temp="+temp+"&timeSent="+asctime(gmtime(&timeSent)));

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}