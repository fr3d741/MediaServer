#include <Networking/RestApi.h>
#include <Utility/ScopedFunction.h>
#include <Utility/GeneralUtilities.h>

#include <curl/curl.h>

#include <format>
#include <regex>
#include <array>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using G = GeneralUtilities;

size_t 
write_callback(char* ptr, size_t /*size*/, size_t nmemb, void* userdata) {

    auto str = reinterpret_cast<std::string*>(userdata);
    str->append(ptr, nmemb);
    return nmemb;
}

static std::string
SendRequest(const std::string& request) {

    { 
        // Implement rate limiting: 20 req/s
        // According to TMDB API there is no such limit, but in practice there is
        static int request_count = 0;
        static std::chrono::time_point last_time = std::chrono::steady_clock::now();
        static long long last_second = 0;

        request_count++;

        const std::chrono::time_point now = std::chrono::steady_clock::now();
        const std::chrono::duration duration = now - last_time;
        const long long second = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        if (last_second != second) {
            request_count = 0;
            last_second = second;
            last_time = now;
        }
        else if (duration < 1s && 20 <= request_count) {
            std::this_thread::sleep_for(1s - duration);
        }
    }

    std::string result;
#ifndef  DISABLE_REST_API
    CURL* curl = curl_easy_init();
    if (curl == nullptr)
        return "";

    ScopedFunction fn_curl([=]() { curl_easy_cleanup(curl); });
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    res = curl_easy_perform(curl);
#endif //  DISABLE_REST_API

    return result;
}

static std::string
CreateSearchRequest(const string title, std::string_view query, const std::string& api_key) {

    const rgx regex(L"[(][0-9]{4}[)]");
    const rgx regex_year(L"[0-9]{4}");

    rgx_match match;
    if (std::regex_search(title, match, regex) == false)
        return "";

    auto title_wo_year = G::Convert(match.prefix());
    auto intermediate = match.str();
    std::regex_search(intermediate, match, regex_year);
    auto year = G::Convert(match.str());

    auto title_uri_coded = curl_easy_escape(/*discarded*/nullptr, title_wo_year.data(), static_cast<int>(title_wo_year.size()));
    ScopedFunction fn([=]() { curl_free(title_uri_coded); });

    return std::vformat(query, std::make_format_args(api_key, title_uri_coded, year));
}

std::string& RestApi::ApiKey() {
    static std::string api_key;
    return api_key;
}

std::string
RestApi::SearchMovie(const string& title){

    const std::string_view query = "https://api.themoviedb.org/3/search/movie?api_key={}&language=en-US&query={}&page=1&include_adult=false&year={}";
    auto request = CreateSearchRequest(title, query, ApiKey());
    return SendRequest(request);
}

std::string
RestApi::SearchTv(const string& title){

    const char* query = "https://api.themoviedb.org/3/search/tv?api_key={}&language=en-US&page=1&query={}&include_adult=false&first_air_date_year={}";

    auto request = CreateSearchRequest(title, query, ApiKey());
    return SendRequest(request);
}

std::string 
RestApi::MovieDetails(const std::string& id) {

    auto request = std::format("https://api.themoviedb.org/3/movie/{}?api_key={}&append_to_response=images,translations,keywords&include_image_language=en,hu,null", id, ApiKey());
    return SendRequest(request);
}

std::string 
RestApi::TvDetails(const std::string& id) {

    auto request = std::format("https://api.themoviedb.org/3/tv/{}?api_key={}&append_to_response=images,translations,keywords,credits&include_image_language=en,HU,null", id, ApiKey());
    return SendRequest(request);
}

std::string 
RestApi::EpisodeGroups(const std::string& id) {

    auto request = std::format("https://api.themoviedb.org/3/tv/{}/episode_groups?api_key={}", id, ApiKey());
    return SendRequest(request);
}

std::string 
RestApi::EpisodeGroup(const std::string& group_id) {

    auto request = std::format("https://api.themoviedb.org/3/tv/episode_group/{}?api_key={}&append_to_response=credits", group_id, ApiKey());
    return SendRequest(request);
}

std::string 
RestApi::Season(const std::string& id, int season_nr) {

    auto request = std::format("https://api.themoviedb.org/3/tv/{}/season/{}?api_key={}&append_to_response=translations", id, std::to_string(season_nr), ApiKey());
    return SendRequest(request);
}

