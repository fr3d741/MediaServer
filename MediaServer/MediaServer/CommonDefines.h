#pragma once

#include <array>

enum class Keys {
    NodeType,
    Last
};

enum class Tags {
    Path_Update,
    Last
};

enum MediaType {
    Movie = 0,
    TvShow = 1
};

enum class TmdbTags {
    original_title,
    id,
    imdb_id,
    genres,
    images,
    posters,
    iso_639_1,
    file_path,
    translations,
    data,
    title,
    release_date,
    belongs_to_collection,
    name,
    overview,
    runtime,
    keywords,
    cast,
    credits,
    crew,
    character,
    profile_path,
    seasons,
    season_number,
    episode_count,
    results,
    number_of_seasons,
    group_count,
    groups,
    episodes,
    episode_number,
    order,
    Last
};

enum class KodiTags {
    title = 0,
    unique_id,
    original_title,
    genre,
    year,
    original_filename,
    fanart,
    thumb,
    poster,
    aspect,
    set,
    name,
    overview,
    plot,
    outline,
    runtime,
    tag,
    actor,
    role,
    profile,
    tmdbid,
    premiered,
    season,
    episode,
    Last
};

const char* KeyWords(Keys key);
const char* TagWords(Tags key);
const char* TmdbWords(TmdbTags key, int media_type = 0);
const char* KodiWords(KodiTags key);
const char* XmlTagFor(MediaType media_type);
