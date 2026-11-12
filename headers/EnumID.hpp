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
    // Trang Overlay <Các nút hiện không có effect>
    OL_Bg,
    OL_BtnContinue,
    OL_BtnBackToHome,

    // Trang Game <Các nút hiện không có effect>
    G_Bg,
    G_BtnPause,
    G_Frame,
    /* VÙNG LÕI GAME (Core Gameplay) */
    G_Number0, // điểm gốc để load lúc vẽ
    G_Number1,
    G_Number2,
    G_Number3,
    G_Number4,
    G_Number5,
    G_Number6,
    G_Number7,
    G_Number8,
    G_Flag,
    G_Bomb,
    G_Hidded,
    G_Openned,
    G_Question,
    G_BtnResetNormal,
    G_BtnResetLosed,
    G_BtnResetWon,
    G_Dead,
    G_Wrong,
    G_Panel,

    /* VÙNG CỦA MOD SA MẠC (Ghi đè lên lõi game) */
    G_Sand,
    /* VÙNG CỦA MOD RỪNG RẬM (Ghi đè lên lõi game)*/
    G_Root,
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

enum class FontID {
    DigitalNumber,
    PressStart2P,
};