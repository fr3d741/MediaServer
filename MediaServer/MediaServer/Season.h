#pragma once

#include <JsonNode.h>
#include <map>

class Season
{
    JsonNode::Ptr _json_data;
    std::vector<JsonNode::Ptr> _episodes_json;
    std::map<std::string, JsonNode::Ptr> _episodes;

    size_t _episode_count;
    int _season_nr;
public:
    Season() = default;
    Season(JsonNode::Ptr json);

    size_t Size();

    int SeasonNumber();

    JsonNode::Ptr operator[](size_t index);
};

class Seasons {
    std::map<int, Season> _seasons;
public:

    Seasons() = default;

    void Add(Season season);
    size_t Size();
    Season& operator[](int index);
};

