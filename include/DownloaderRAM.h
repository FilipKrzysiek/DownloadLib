//
// Created by krzysiu on 11.07.2020.
//

#ifndef TEST1_DOWNLOADERRAM_H
#define TEST1_DOWNLOADERRAM_H


#include "Downloader.h"

class DownloaderRAM: public Downloader {
private:
    stringstream downloadedData;
    unsigned maxStreamSize = 250 * 1024 * 1024;

    int getDownloadedDataSize();
    bool checkPrepared();

    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *f);

    void writeDataFun(void *ptr, size_t size, size_t nmemb);

public:
    /**
     * @brief Seting max downloaded file size if is downloaded to ram (stringstream)
     * @param size size in MB, default 250MB
     */
    void setMaxDownloadedStreamSize(unsigned size);
    /**
     * @brief Download file to RAM (to stringStream)
     * @return 1 - download succesfully; 0 - error on download
     */
    bool downloadToRam();
    /**
     * @brief Return downloaded data to RAM
     * @return string with data
     */
    stringstream * getDownloadedData();

    /**
     * @brief Delete downloaded data (clear string stream)
     */
    void clearDownloadedData();

    virtual ~DownloaderRAM();
};


#endif //TEST1_DOWNLOADERRAM_H
