#pragma once

#include <Model/CommonMedia.h>
#include <CommonDefines.h>

#include <filesystem>
#include <map>

namespace Media {

    class TvShow : public CommonMedia
    {
    public:
        TvShow(Logging::ILogger::Ptr logger, const std::filesystem::directory_entry& entry);

        bool Init() override;
        std::map<std::wstring, XmlNode> CreateXml() override;

    protected:
        const char* Tmdb(TmdbTags key) override;
        std::wstring GetFileName() override;
        std::string GetDetails(const std::string& id) override;
    public:
        void CreateEpisodeNfos(std::map<std::wstring, XmlNode>& dir_entry);
    };

}
