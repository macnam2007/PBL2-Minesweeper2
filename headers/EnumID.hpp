#pragma once

enum class SoundID {
    /* VÙNG GLOBAL (UI - Không bao giờ xóa) */
    BtnReleased,

    /* VÙNG LÕI GAME (Core Gameplay) */

    /* VÙNG CỦA MOD SA MẠC (Chỉ load khi chơi sa mạc) */

    /* VÙNG CỦA MOD RỪNG RẬM */
};

enum class ImageID {
    /* VÙNG GLOBAL (UI - Không bao giờ xóa) */
    // Nút tồn tại vĩnh viễn
    All_BtnSoundOn,
    All_BtnSoundOff,
    All_BtnMusicOn,
    All_BtnMusicOff,
    // Trang Home 
    H_Bg,
    H_IconFlag,
    H_IconBomb,
    H_BtnPlay1,
    H_BtnPlay2,
    H_BtnPlay3,
    H_BtnExit1,
    H_BtnExit2,
    H_BtnExit3,
    // Trang SelectMap  <Các nút hiện không có effect>
    SMap_Bg,
    SMap_BtnBack,
    SMap_BtnBasic,
    SMap_BtnDesert,
    SMap_BtnJungle,
    SMap_BtnBeeHive,
    // Trang SelectMode  <Các nút hiện không có effect>
    SMode_Bg,
    SMode_BoxInfo,
    SMode_BtnEasy,
    SMode_BtnNormal,
    SMode_BtnHard,
    SMode_BtnExplore,
    SMode_BtnStart,
    SMode_BtnBack,
    // Trang Game <Các nút hiện không có effect>
    G_Bg,
    G_BtnPause,
    G_BtnReset,
    // Trang Overlay <Các nút hiện không có effect>
    OL_Bg,
    OL_BtnContinue,
    OL_BtnBackToHome,


    /* VÙNG LÕI GAME (Core Gameplay) */

    /* VÙNG CỦA MOD SA MẠC (Chỉ load khi chơi sa mạc) */

    /* VÙNG CỦA MOD RỪNG RẬM */

};

enum class MapID {
    Basic,
    Desert,
    Jungle,
    BeeHive
};

enum class ModeID {
    Easy,
    Normal,
    Hard,
    Explore
};