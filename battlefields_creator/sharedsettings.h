#ifndef SHAREDSETTINGS_H
#define SHAREDSETTINGS_H


#include <QPoint>
#include <numeric>


using RectId = unsigned int;

static constexpr RectId INVALID_RECT_ID = std::numeric_limits<RectId>::max();


static constexpr int SCENE_WIDTH = 1025;
static constexpr int SCENE_HEIGHT = 1025;


static constexpr float SCENE_MIN_OFFSET = -128;
static constexpr float SCENE_MAX_OFFSET = 127;


struct RectParam
{
    QPointF pos{0, 0};
    float width = 0;
    float height = 0;
    float rotation = 0.0f;
    int offset = 0;
    int renderOrder = -1;
};


static constexpr RectParam DEFAULT_RECT{QPoint{512, 512}, 200, 200, 0, 50};


enum TeamsSize
{
    TEAMS_SIZE_1x1 = 0,
    TEAMS_SIZE_2x2,
    TEAMS_SIZE_3x3,

    TEAMS_SIZE_COUNT
};


static const char* SETTINGS_FILE = "settings.ini";

static const char* SETTINGS_KEY_INSTALL_MOD = "install_mod";
static const char* SETTINGS_KEY_REMOVE_TMP_FOLDER = "remove_tmp_folder";
static const char* SETTINGS_KEY_GAME_FOLDER = "game_folder";
static const char* SETTINGS_KEY_PACK_MANAGER_FOLDER = "pack_fanager_folder";
static const char* SETTINGS_KEY_TMP_FOLDER = "tmp_folder";
static const char* SETTINGS_KEY_SAVE_FOLDER = "save_folder";

static const char* TMP_FOLDER_NAME = "tmp";
static const char* PACK_FILE_NAME = "custom_battlefield.pack";
static const char* IMAGE_FILE_NAME = "image.png";

#endif // SHAREDSETTINGS_H
