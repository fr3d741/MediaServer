#include <RestApi.h>
#include <ScopedFunction.h>

#include <curl/curl.h>

#include <format>
#include <regex>
#include <array>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {

    auto str = reinterpret_cast<std::string*>(userdata);
    str->append(ptr, nmemb);
    return nmemb;
}

static std::string
SendRequest(const std::string& request) {

    { 
        // Implement rate limiting: 20 req/s
        // According to TMDB API there is no such limit, but in practice there is
        static std::array<int, 60> _requests = {};
        std::chrono::hh_mm_ss hh(std::chrono::steady_clock::now().time_since_epoch());

        auto sec = hh.seconds().count();
        _requests[sec]++;
        _requests[(sec+1)%60] = 0;
        if (_requests[sec] == 20) {
            std::cout << "Limit requests" << std::endl;
            std::this_thread::sleep_for(500ms);
        }
    }

    CURL* curl = curl_easy_init();
    std::string result;
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

    return result;
}

static std::string
CreateSearchRequest(const std::string title, std::string_view query, const std::string& api_key) {

    const std::regex regex("[(][0-9]{4}[)]");

    std::smatch match;
    if (std::regex_search(title, match, regex) == false)
        return "";

    const std::string title_wo_year = std::regex_replace(title, regex, "");
    std::string year = match.str().substr(1, 4);

    auto title_uri_coded = curl_easy_escape(/*discarded*/nullptr, title_wo_year.c_str(), static_cast<int>(title_wo_year.size()));
    ScopedFunction fn([=]() { curl_free(title_uri_coded); });

    return std::vformat(query, std::make_format_args(api_key, title_uri_coded, year));
}

std::string& RestApi::ApiKey() {
    static std::string api_key;
    return api_key;
}

std::string
RestApi::SearchMovie(const std::string& title){

    const std::string_view query = "https://api.themoviedb.org/3/search/movie?api_key={}&language=en-US&query={}&page=1&include_adult=false&year={}";
    auto request = CreateSearchRequest(title, query, ApiKey());
    return SendRequest(request);
}

std::string
RestApi::SearchTv(const std::string& title){

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

