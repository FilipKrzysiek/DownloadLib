//
// Created by cysiu on 08.08.2019.
//

#ifndef TRAMSTOP_DOWNLOADER_H
#define TRAMSTOP_DOWNLOADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <curl/curl.h>
#include <sys/types.h>
#include <experimental/filesystem>
#include "Exception.h"

using namespace std;

class Downloader {
public:
    Downloader();
    /**
     * @brief Constructor setting url and save path
     * @param url - url from file be downloaded
     * @param savePath - path where file was saved
     */
    Downloader(string url, string savePath);
    /**
     * @brief Set url from file be downloaded
     * @param url
     */
    void setUrl(string url);
    /**
     * @brief Set path where download file was saved
     * @param savePath
     */
    void setSavePath(string savePath);
    /**
     * @brief Set behavior on download error
     * @param end 1 - end program on download error; 0 - continue program on download error
     */
    void setEndOnError(bool end);
    /**
     * @brief Adding data to get query
     * @param param parameter
     * @param value value of parameter
     */
    void getAdd(const string& param, const string& value);
    /**
     * @brief Clear get query
     */
    void getClear();
    /**
     * @brief Adding data to post query
     * @param param parameter
     * @param value value of parameter
     */
    void postAdd(const string& param, const string& value);
    /**
     * @brief Clear post query
     */
    void postClear();
    /**
     * @brief Download file
     * @return 1 - download succesfully; 0 - error on download
     */
    bool download();
    /**
     * @brief Checking on server, that exist newer version, than passed date
     * @param fileTime time to compare on server
     * @return 1 - exist newer version; 0 - exist older version
     */
    bool existNewerVersion(time_t fileTime);

    /**
     * @brief Getting describe CURL error
     * @return
     */
    string getCurlErrorString();
    /**
     * @brief Returning full url with get
     * @return
     */
    string getFullUrl();

    /**
     * @brief Getting resonse code of last response
     * @return Response code or -1 if nothing was called
     */
    long getResponseCode();

protected:
    string url = "";
    string savePath = "";
    string getQuery = "";
    string postQuery = "";
    CURLcode curlErrCode;
    bool endOnError = false;

    long responseCode = -1;
private:
    ofstream dfstream;
    bool openDFile();
    bool reopenFile();

    bool checkPrepared();

    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

    static size_t fake_write_data(void *ptr, size_t size, size_t nmemb, void *stream);

    void writeDataFun(void *ptr, size_t size, size_t nmemb);
};


#endif //TRAMSTOP_DOWNLOADER_H
