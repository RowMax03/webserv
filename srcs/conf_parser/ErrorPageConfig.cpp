#include "ErrorPageConfig.hpp"

namespace Config {

    ErrorPage::ErrorPage() : status(501), url("/fallbackError.html") {}

    ErrorPage::ErrorPage(int status, const std::string &url = "") : status(status), url(url) {
        if (this->url.empty()) {
            char buffer[50];
            sprintf(buffer, "default_%d.html", this->status);
            this->url = buffer;
        }
    }
    ErrorPage::ErrorPage(const ErrorPage &other) : confBase(other), status(other.status), url(other.url) {}

    ErrorPage::~ErrorPage() {}

    void ErrorPage::print() const {
        std::cout << "\t\tError Page: \n\t\t\tstatus=" << status << ", \n\t\t\turl=" << url << std::endl;
    }

    void ErrorPage::validate() {
    }

}