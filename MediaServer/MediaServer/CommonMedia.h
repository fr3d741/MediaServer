#pragma once

#include <CommonDefines.h>
#include <XmlNode.h>
#include <JsonNode.h>
#include <ILogger.h>

#include <filesystem>

namespace Media {

    class CommonMedia
    {
    protected:
        Logging::ILogger::Ptr _logger;
        const std::wstring _title;
        const std::filesystem::directory_entry _entry;
        ::MediaType _media_type;
        JsonNode::Ptr _details;
    public:
        CommonMedia(Logging::ILogger::Ptr logger, const std::filesystem::directory_entry& entry, ::MediaType media_type = ::MediaType::Movie);

        enum Property {
            title,
            originaltitle,
            year,
            ratings,
            userrating,
            set,
            plot,
            outline,
            tagline,
            runtime,
            poster,
            fanart,
            mpaa,
            certification,
            id,
            country,
            premiered,
            genre,
            studio,
            credits,
            tag,
            actor,
            languages,
            dateadded,
        };

        virtual void Fill(Logging::ILogger::Ptr logger, XmlNode& root, JsonNode::Ptr json, const std::vector<Property>& properties);
        virtual bool Init();
        virtual std::map<std::string, XmlNode> CreateXml();

        static bool IsMediaFile(const std::filesystem::path& path);

    protected:
        virtual const char* Tmdb(TmdbTags key);
        virtual std::string GetDetails(const std::string& id);
        virtual std::string GetFileName();
        JsonNode::Ptr GetDetails(JsonNode::Ptr json_ptr);
    };
}
