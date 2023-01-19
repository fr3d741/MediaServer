#pragma once

#include <string>

class RestApi
{
public:
    static std::string& ApiKey();
    static std::string SearchMovie(const std::string& title);
    static std::string SearchTv(const std::string& title);

    static std::string MovieDetails(const std::string& id);
    static std::string TvDetails(const std::string& id);
    static std::string EpisodeGroups(const std::string& id);
    static std::string EpisodeGroup(const std::string& group_id);
    static std::string Season(const std::string& id, int season_nr);
};

