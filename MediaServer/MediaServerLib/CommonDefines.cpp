#include <CommonDefines.h>

const char* 
KeyWords(Keys key) {
    constexpr std::array<const char*, static_cast<size_t>(Keys::Last)> words = { 
        "NodeType" 
    };

    return words[static_cast<int>(key)];
}

const char* 
TagWords(Tags key) {
    constexpr std::array<const char*, static_cast<size_t>(Tags::Last)> words = {
        "PathUpdate" 
    };

    return words[static_cast<int>(key)];
}


const char* 
TmdbWords(TmdbTags key, int media_type) {

    typedef std::array<const char*, static_cast<size_t>(TmdbTags::Last)> wordsArray;
    
    constexpr wordsArray movies = { "original_title", "id", "imdb_id", "genres","images", "posters", "iso_639_1", "file_path", "translations", "data", "title", "release_date", "belongs_to_collection", "name",
        "overview", "runtime", "keywords", "cast", "credits", "crew", "character", "profile_path", "","","", "results", "", "", "", "", "",""};
    constexpr wordsArray tvshows = { "original_name", "id", "imdb_id", "genres","images", "posters", "iso_639_1", "file_path", "translations", "data", "name", "release_date", "belongs_to_collection", "name",
        "overview", "runtime", "keywords", "cast", "credits", "crew", "character", "profile_path", "seasons", "season_number", "episode_count", "results", "number_of_seasons", "group_count", "groups", "episodes", 
        "episode_number","order"};

    constexpr std::array<wordsArray, 2> words = { movies, tvshows };
    return words[media_type][static_cast<int>(key)];
}

const char*
KodiWords(KodiTags key) {

    constexpr std::array<const char*, static_cast<size_t>(KodiTags::Last)> words = {
        "title", "uniqueid", "originaltitle","genre","year","original_filename", "fanart","thumb", "poster", "aspect", "set", "name", "overview",
        "plot", "outline", "runtime", "tag", "actor", "role", "profile", "tmdbid","premiered", "season", "episode"
    };

    return words[static_cast<int>(key)];
}

const char*
XmlTagFor(MediaType media_type) {

    constexpr std::array<const char*, static_cast<size_t>(KodiTags::Last)> words = {
        "movie", "tvshow"
    };

    return words[static_cast<int>(media_type)];
}

