#include <Model/TvShow.h>
#include <Networking/RestApi.h>
#include <Configuration/IConfiguration.h>
#include <Utility/GeneralUtilities.h>

#include <regex>
#include <algorithm>

using namespace Media;
using G = GeneralUtilities;

static XmlNode
CreateEpisodeXml(JsonNode::Ptr json) {

    XmlNode root("episodedetails");
    root.AddChild(KodiWords(KodiTags::title), json->GetString(TmdbWords(TmdbTags::name, MediaType::TvShow)));
    root.AddChild(KodiWords(KodiTags::season), json->GetAsString(TmdbWords(TmdbTags::season_number, MediaType::TvShow)));
    root.AddChild(KodiWords(KodiTags::episode), json->GetAsString(TmdbWords(TmdbTags::episode_number, MediaType::TvShow)));
    root.AddChild(KodiWords(KodiTags::plot), json->GetString(TmdbWords(TmdbTags::overview, MediaType::TvShow)));

    return root;
}

static std::string 
Format(int season, int episode) {

    std::string season_str;
    std::string episode_str;
    {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(3) << season;
        season_str = stream.str();
    }
    {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(3) << episode;
        episode_str = stream.str();
    }
    return "S" + season_str + "E" + episode_str;
}

static std::string
ConvertToCommonFormat(const string& str) {

    const rgx number_filter(L"[0-9]{1,3}");
    auto view = string_view(str);
    auto it = std::regex_iterator<string_view::iterator>{ view.cbegin(), view.cend(), number_filter };
    
    auto season = stoi(G::Convert(it->str()));
    ++it;
    auto episode = stoi(G::Convert(it->str()));
    return Format(season, episode);
}

static void
GatherEpisodesFromFS(Logging::ILogger::Ptr logger, const std::filesystem::directory_entry& dir_entry, std::map<std::string, string>& season_episodes) {

    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    for (auto const& season_dir : std::filesystem::directory_iterator{ dir_entry }) {

        if (season_dir.is_directory() == false)
            continue;

        const std::regex season_filter("season.[0-9]{1,3}", std::regex_constants::icase);
        std::smatch match;
        auto dir_name = season_dir.path().filename().string();
        if (std::regex_search(dir_name, match, season_filter) == false)
            continue;

        int episode_count = 0;

        for (auto const& file_entry : std::filesystem::directory_iterator{ season_dir }) {
            // Looking for SxxExx pattern to identify episodes

            if (file_entry.is_directory())
                continue;

            if (CommonMedia::IsMediaFile(file_entry.path()) == false)
                continue;

            const rgx episode_filter(L"S[0-9]{1,3}E[0-9]{1,3}", std::regex_constants::icase);
            rgx_match file_match;
            auto file_name = file_entry.path().filename().generic_wstring();
            if (std::regex_search(file_name, file_match, episode_filter) == false)
            {
                XmlNode root("Incorrect file pattern");
                root.AddAttribute("file", file_entry.path().generic_wstring());
                logger->LogMessage(root.Dump());
                continue;
            }

            episode_count++;
            season_episodes.insert({ ConvertToCommonFormat(file_match.str()), file_entry.path().generic_wstring() });
        }
    }
}

static void
GatherEpisodesFromJson(JsonNode::Ptr json, std::map<std::string, JsonNode::Ptr>& episodes_map) {

    auto episodes_json = json->GetArray(TmdbWords(TmdbTags::episodes, MediaType::TvShow));
    auto season_nr = -1;

    if (json->Has(TmdbWords(TmdbTags::order, MediaType::TvShow)))
        season_nr = json->GetInt(TmdbWords(TmdbTags::order, MediaType::TvShow));
    else if (json->Has(TmdbWords(TmdbTags::season_number, MediaType::TvShow)))
        season_nr = json->GetInt(TmdbWords(TmdbTags::season_number, MediaType::TvShow));
    else
        return;

    for (auto i = 0; i < episodes_json.size(); ++i) {

        const std::string id = Format(season_nr, i+1);
        episodes_map.insert({ id, episodes_json[i] });
    }
}

static bool 
operator<(const std::pair<const std::string, JsonNode::Ptr>& a, const std::pair<const std::string, string>& b) {
    return a.first < b.first;
}

static bool 
operator<(const std::pair<const std::string, string>& a, const std::pair<const std::string, JsonNode::Ptr>& b) {
    return a.first < b.first;
}

using InputIt1 = std::map<std::string, JsonNode::Ptr>::iterator;
using InputIt2 = std::map<std::string, string>::const_iterator;

static bool 
is_includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {//custom specialization of std::includes
    for (; first2 != last2; ++first1)
    {
        if (first1 == last1 || *first2 < *first1)
            return false;
        if (!(*first1 < *first2))
            ++first2;
    }
    return true;
}

static int 
count_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    int counter = 0;
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
            ++first1;
        else
        {
            if (!(*first2 < *first1))
                ++counter; // *first1 and *first2 are equivalent.
            ++first2;
        }
    }
    return counter;
}

static std::map<std::string, JsonNode::Ptr>
GetAllSeasonConfigurations(Logging::ILogger::Ptr logger, const int nr_of_seasons, const std::string& show_id, const std::map<std::string, string>& reference) {

    std::map<std::string, JsonNode::Ptr> to_return;
    int ret_count = 0;
    {
        std::map<std::string, JsonNode::Ptr> episodes_map;
        // Seasons stored with TvShow details
        for (auto i = 0; i <= nr_of_seasons; ++i) {
            auto response_json = JsonNode::Parse(logger, RestApi::Season(show_id, i));
            if (response_json->Has(TmdbWords(TmdbTags::episodes, MediaType::TvShow)) == false)
                continue;
            GatherEpisodesFromJson(response_json, episodes_map);
        }

        if (is_includes(episodes_map.begin(), episodes_map.end(), reference.begin(), reference.end()))
            return episodes_map;

        int count = count_intersection(episodes_map.begin(), episodes_map.end(), reference.begin(), reference.end());
        if (ret_count < count) {
            to_return = std::move(episodes_map);
        }
    }

    {
        // check for alternate episode groups
        auto episode_groups_json_string = RestApi::EpisodeGroups(show_id);
        auto ep_group_json = JsonNode::Parse(logger, episode_groups_json_string);
        auto group_results_array = ep_group_json->GetArray(TmdbWords(TmdbTags::results, MediaType::TvShow));
        if (group_results_array.empty())
            return to_return;

        for (auto it : group_results_array) {
            auto group_id = it->GetString(TmdbWords(TmdbTags::id, MediaType::TvShow));
            auto group_str = RestApi::EpisodeGroup(group_id);
            auto group_json = JsonNode::Parse(logger, group_str);

            std::map<std::string, JsonNode::Ptr> episodes_map;
            auto group_array = group_json->GetArray(TmdbWords(TmdbTags::groups, MediaType::TvShow));
            for (int i = 0; i < group_array.size(); ++i) {

                GatherEpisodesFromJson(group_array[i], episodes_map);
            }

            if (is_includes(episodes_map.begin(), episodes_map.end(), reference.begin(), reference.end()))
                return episodes_map;

            auto count = count_intersection(episodes_map.begin(), episodes_map.end(), reference.begin(), reference.end());
            if (ret_count < count) {
                to_return = std::move(episodes_map);
            }
        }
    }

    return to_return;
}

TvShow::TvShow(Logging::ILogger::Ptr logger, const std::filesystem::directory_entry& entry)
    : CommonMedia(logger, entry, MediaType::TvShow) {
}

bool
TvShow::Init() {

    auto result_in_json = RestApi::SearchTv(_title);
    if (result_in_json.empty())
    {
        auto msg = L"Empty request response for " + _title;
        _logger->LogMessage(msg);
        return false;
    }

    auto json = JsonNode::Parse(_logger, result_in_json);
    _details = CommonMedia::GetDetails(json, [&](JsonNode::Ptr pp) { return pp->GetWString(Tmdb(TmdbTags::name)); });
    if (_details == nullptr) {
        auto msg = L"Cannot find details for " + _title;
        _logger->LogMessage(msg);
        return false;
    }

    return true;
}

std::map<std::string, XmlNode> 
TvShow::CreateXml() {

    auto result_map = CommonMedia::CreateXml();
    CreateEpisodeNfos(result_map);
    return result_map;
}

const char* 
TvShow::Tmdb(TmdbTags key) {
    return TmdbWords(key, MediaType::TvShow);
}

std::string 
TvShow::GetFileName() {

    return _entry.path().string().append("\\tvshow.nfo");
}

std::string
TvShow::GetDetails(const std::string& id) {
    return RestApi::TvDetails(id);
}

void
TvShow::CreateEpisodeNfos(std::map<std::string, XmlNode>& result_map) {

    std::map<std::string, string> season_episodes;
    GatherEpisodesFromFS(_logger, _entry, season_episodes);

    auto show_id = _details->GetInt(Tmdb(TmdbTags::id));
    auto seasons = _details->GetArray(Tmdb(TmdbTags::seasons));
    auto nr_of_seasons = seasons.size();
    auto proposed_episode_mapping = GetAllSeasonConfigurations(_logger, static_cast<int>(nr_of_seasons), std::to_string(show_id), season_episodes);

    const std::filesystem::path extension(".nfo");
    for (auto&& item : season_episodes) {

        if (proposed_episode_mapping.contains(item.first) == false)
            continue;

        auto epiosde_json = proposed_episode_mapping[item.first];
        std::filesystem::path path(item.second);
        path.replace_extension(extension);
        result_map[path.string()] = CreateEpisodeXml(epiosde_json);
    }
}
