#include <Season.h>
#include <CommonDefines.h>

#include <format>
#include <sstream>
#include <iomanip>

Season::Season(JsonNode::Ptr json)
    : _json_data(json){

    _episodes_json = json->GetArray(TmdbWords(TmdbTags::episodes, MediaType::TvShow));

    _episode_count = _episodes_json.size();

    if (json->Has(TmdbWords(TmdbTags::order, MediaType::TvShow)))
        _season_nr = json->GetInt(TmdbWords(TmdbTags::order, MediaType::TvShow));
    else if (json->Has(TmdbWords(TmdbTags::season_number, MediaType::TvShow)))
        _season_nr = json->GetInt(TmdbWords(TmdbTags::season_number, MediaType::TvShow));
    else
        _season_nr = -1;

    for (auto i = 0; i < _episodes_json.size(); ++i) {

        auto episode = _episodes_json[i];
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(3) << _season_nr;
        auto season = stream.str();
        stream.seekp(std::ios_base::beg);
        stream << i;
        const std::string id = std::format("S{}E{}", season, stream.str());
        _episodes.insert({ id, episode });
    }
}

size_t 
Season::Size() {
    return _episode_count;
}

int
Season::SeasonNumber() {
    return _season_nr;
}

JsonNode::Ptr 
Season::operator[](size_t index) {

    return _episodes_json[index];
}

void
Seasons::Add(Season season) {

    _seasons.insert({ season.SeasonNumber(), season });
}

size_t 
Seasons::Size() {
    return _seasons.size();
}

Season& 
Seasons::operator[](int index) {
    return _seasons[index];
}
