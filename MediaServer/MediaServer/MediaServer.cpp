#include <MediaServer.h>
#include <CommonDefines.h>
#include <JsonNode.h>
#include <CommonMedia.h>
#include <TvShow.h>

#include <chrono>
#include <filesystem>
#include <fstream>

using namespace std::chrono_literals;

static std::string
GetFileName(const std::filesystem::directory_entry& dir, MediaType media_type) {

    const std::set<std::string> common_extensions = { ".mkv", ".avi", ".mpeg", ".mov", ".wmv", ".mp4", ".m4p", ".m4v" };

    if (media_type == MediaType::TvShow)
        return "tvshow.nfo";

    for (auto const& dir_entry : std::filesystem::directory_iterator{ dir })
    {
        if (dir_entry.is_regular_file() == false)
            continue;

        auto ext = dir_entry.path().extension().string();
        if (common_extensions.count(ext) == 0)
            break;

        return dir_entry.path().filename().string() + ".nfo";
    }

    return "movie.nfo";
}

static std::multimap<int, std::string> 
MergeMessages(std::list<std::string>& local_storage, Logging::ILogger::Ptr logger) {

    std::multimap<int, std::string> messages;
    for (auto msg : local_storage) {

        auto root = JsonNode::Parse(logger, msg);
        if (root->Has(KeyWords(Keys::NodeType)) == false)
        {
            logger->LogMessage("Invalid node type: " + msg);
            continue;
        }

        int type;
        if (root->Get(KeyWords(Keys::NodeType), type) == false)
        {
            logger->LogMessage("Invalid node type: " + msg);
            continue;
        }

        if (messages.contains(type))
        {
            auto it_pair = messages.equal_range(type);
            bool is_found = false;
            for (auto it = it_pair.first; it != it_pair.second && !is_found; ++it) {
                if (it->second.compare(msg) == 0)
                    is_found = true;
            }

            if (!is_found)
                messages.insert({ type, msg });
        }
        else
            messages.insert({ type, msg });
    }

    return messages;
}

MediaServer::MediaServer(Logging::ILogger::Ptr logger, IMessageQueue::Ptr queue, IStreamWriter::Ptr writer)
    : _logger(logger)
    , _queue(queue)
    , _writer(writer) {

}

void 
MediaServer::Start()
{
    std::list<std::string> local_storage;
    std::chrono::steady_clock::time_point clock;
    bool count_started = false;

    while (true) {

        if (_queue->HasMessage()) {
            auto msg = _queue->Pop();
            local_storage.push_back(msg);
            clock = std::chrono::steady_clock::now();
            count_started = true;
        }

        if (count_started) {

            auto now = std::chrono::steady_clock::now();
            if (10s <= (now - clock)) {

                count_started = false;

                auto messages = MergeMessages(local_storage, _logger);
                local_storage.clear();

                for (auto item : messages) {
                    processMessage(item.second);
                }
            }
        }

        std::this_thread::sleep_for(100ms);
    }
}

void
MediaServer::RegisterPath(const std::string& path, int type) {
    _path_types[path] = type;
}

void 
MediaServer::processMessage(std::string& message) {

    auto root = JsonNode::Parse(_logger, message);
    if (root->Has(KeyWords(Keys::NodeType)) == false)
    {
        _logger->LogMessage("Invalid node type: " + message);
        return;
    }

    int type;
    if (root->Get(KeyWords(Keys::NodeType), type) == false)
    {
        _logger->LogMessage("Invalid node type: " + message);
        return;
    }

    switch (static_cast<Tags>(type)) {
        case Tags::Path_Update: {
            std::string path;
            root->Get(TagWords(Tags::Path_Update), path);
            auto media_type = (MediaType)_path_types[path];

            std::filesystem::path dir(path);
            for (auto const& dir_entry : std::filesystem::directory_iterator{ dir }) {

                if (!dir_entry.is_directory())
                    continue;

#if true
                // TODO: remove
                { // do not parse until cache cleared
                    auto string_path = dir_entry.path().string();
                    if (_cache.contains(string_path))
                        continue;
                    _cache.insert(string_path);
                }
#endif

                std::shared_ptr<Media::CommonMedia> media;
                switch (media_type)
                {
                    case Movie:
                        media = std::make_shared<Media::CommonMedia>(_logger, dir_entry);
                        break;
                    case TvShow:
                        media = std::make_shared<Media::TvShow>(_logger, dir_entry);
                        break;
                    default:
                        break;
                }

                if (media->Init() == false)
                    continue;

                auto nodes = media->CreateXml();

                for (auto&& it : nodes) {

                    _writer->Write(it.first, it.second.Dump());
                }
            }
        }
        break;
        case Tags::Last:
        default:
            break;
    }

}
