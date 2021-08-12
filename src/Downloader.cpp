//
// Created by cysiu on 08.08.2019.
//

#include "../include/Downloader.h"

Downloader::Downloader() {
    this->endOnError = true;
}

Downloader::Downloader(string url, string savePath) {
    this->url = url;
    this->savePath = savePath;
    this->endOnError = true;
}

void Downloader::setUrl(string url) {
    this->url = url;
}

void Downloader::setSavePath(string savePath) {
    this->savePath = savePath;
}

void Downloader::setEndOnError(bool end) {
    this->endOnError = end;
}

bool Downloader::checkPrepared() {
    return !(savePath.empty() || url.empty());
}

bool Downloader::download() {
    responseCode = -1;
    if (this->checkPrepared()) {
        CURL *curl = curl_easy_init();
        if (curl) {
            if (!this->openDFile()) {
                throw Exception("Can't open file");
            }

            if (!getQuery.empty())
                url = url + "?" + getQuery;

            curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

            if (postQuery.empty())
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postQuery.c_str());

            try {
                curlErrCode = curl_easy_perform(curl);
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            } catch (Exception &exp) {
                curl_easy_cleanup(curl);
                if (endOnError) {
                    throw Exception(exp.msg());
                } else {
                    return false;
                }
            }

            if (curlErrCode != 0) {
                curl_easy_cleanup(curl);
                if (endOnError) {
                    throw Exception("Error Curl " + to_string(curlErrCode));
                } else {
                    dfstream.close();
                    return false;
                }
            }

            //Cleaning
            curl_easy_cleanup(curl);
            dfstream.close();
            return true;
        }


        curl_easy_cleanup(curl);
        return false;
    } else {
        return false;
    }
}

size_t Downloader::write_data(void *ptr, size_t size, size_t nmemb, void *f) {
    size_t nbytes = size * nmemb;
    static_cast<Downloader *>(f)->writeDataFun(ptr, size, nmemb);
    return nbytes;
}

void Downloader::writeDataFun(void *ptr, size_t size, size_t nmemb) {
    size_t nbytes = size * nmemb;
    if (dfstream.is_open()) {
        dfstream.write((char *) ptr, nbytes);
    } else {
        if (this->reopenFile()) {
            dfstream.write((char *) ptr, nbytes);
        } else {
            throw Exception("Can't open file");
        }
    }
}

//File functions
bool Downloader::openDFile() {
    this->dfstream.open(this->savePath.c_str(), ofstream::binary | ofstream::out);
    return this->dfstream.is_open();
}

bool Downloader::reopenFile() {
    this->dfstream.open(this->savePath.c_str(), ofstream::app | ofstream::binary | ofstream::out);
    return this->dfstream.is_open();
}

string Downloader::getCurlErrorString() {
    if (curlErrCode == 0)
        return "OK";
    return curl_easy_strerror(curlErrCode);
}

bool Downloader::existNewerVersion(time_t fileTime) {
    //TODO Throw errors
    if (this->checkPrepared()) {
        CURL *curl;
        long ftimeLong;
        time_t tServerFile;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->fake_write_data);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
            curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
            curlErrCode = curl_easy_perform(curl);
            if (curlErrCode == CURLE_OK) {
                curlErrCode = curl_easy_getinfo(curl, CURLINFO_FILETIME, &ftimeLong);
                if (curlErrCode == CURLE_OK && ftimeLong >= 0) {
                    tServerFile = (time_t) ftimeLong;

                    //cleaning
                    curl_easy_cleanup(curl);

                    return fileTime <= tServerFile;
                }
            }
        }
        //Cleaning
        curl_easy_cleanup(curl);
        return false;
    } else {
        return false;
    }
}

size_t Downloader::fake_write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    return size * nmemb;
}

void Downloader::getAdd(const string &param, const string &value) {
    if (!getQuery.empty()) {
        getQuery += '&';
    }
    getQuery = getQuery + param + "=" + value;
}

void Downloader::postAdd(const string &param, const string &value) {
    if (!postQuery.empty()) {
        postQuery += '&';
    }
    postQuery = postQuery + param + "=" + value;
}

void Downloader::getClear() {
    getQuery = "";
}

void Downloader::postClear() {
    postQuery = "";
}

string Downloader::getFullUrl() {
    if (!getQuery.empty())
        return url + "?" + getQuery;
    else
        return url;
}

long Downloader::getResponseCode() {
    return responseCode;
}
