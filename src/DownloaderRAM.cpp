//
// Created by krzysiu on 11.07.2020.
//

#include "../include/DownloaderRAM.h"

bool DownloaderRAM::checkPrepared() {
    return !(url.empty());
}

void DownloaderRAM::setMaxDownloadedStreamSize(unsigned int size) {
    maxStreamSize = size * 1024 * 1024;
}

int DownloaderRAM::getDownloadedDataSize() {
    downloadedData.seekg(0, ios::end);
    return downloadedData.tellg();
}

void DownloaderRAM::clearDownloadedData() {
    downloadedData.str(string());
}

stringstream * DownloaderRAM::getDownloadedData() {
    return &downloadedData;
}

bool DownloaderRAM::downloadToRam() {
    if (this->checkPrepared()) {
        CURL *curl = curl_easy_init();
        if (curl) {
            if(!getQuery.empty())
                url = url + "?" + getQuery;

            curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

            if(postQuery.empty())
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postQuery.c_str());
            curlErrCode = curl_easy_perform(curl);

            if (curlErrCode != 0) {
                if (endOnError) {
                    cout << "Error curl!!" << endl;
                    throw ("Error Curl " + to_string(curlErrCode)).c_str();
                } else {
                    curl_easy_cleanup(curl);
                    return false;
                }
            }

            //Cleaning
            curl_easy_cleanup(curl);
            return true;
        }


        curl_easy_cleanup(curl);
        return false;
    } else {
        return false;
    }
}

size_t DownloaderRAM::write_data(void *ptr, size_t size, size_t nmemb, void *f) {
    size_t nbytes = size * nmemb;
    static_cast<DownloaderRAM *>(f)->writeDataFun(ptr, size, nmemb);
    return nbytes;
}

void DownloaderRAM::writeDataFun(void *ptr, size_t size, size_t nmemb) {
    size_t nbytes = size * nmemb;

    if (getDownloadedDataSize() < maxStreamSize) {
        downloadedData.write((char *) ptr, nbytes);
    } else {
        throw ("Trying download too big file. Max download file size was set to " +
        to_string(maxStreamSize / 1024 / 1024) + "MB").c_str();
    }
}
