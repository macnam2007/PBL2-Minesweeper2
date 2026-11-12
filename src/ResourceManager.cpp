#include "../headers/ResourceManager.hpp"
#include <iostream>
#include <algorithm>

bool ResourceManager::LoadImagesGlobal()
{
    /*Load ảnh của nút Sound và Music*/
    if (
        !this->images[ImageID::All_BtnSoundOn].loadFromFile("../assets/images/H_BtnSoundOn.png")||
        !this->images[ImageID::All_BtnSoundOff].loadFromFile("../assets/images/H_BtnSoundOff.png")||
        !this->images[ImageID::All_BtnMusicOn].loadFromFile("../assets/images/H_BtnMusicOn.png")||
        !this->images[ImageID::All_BtnMusicOff].loadFromFile("../assets/images/H_BtnMusicOff.png")
    ) {std::cerr<<"Error: load image Button Sound and Button Music\n"; return false;}

    /*Load ảnh của trang Home*/
    if (
        !this->images[ImageID::H_Bg].loadFromFile("../assets/images/H_Bg.png") ||
        !this->images[ImageID::H_IconFlag].loadFromFile("../assets/images/H_IconFlag.png")||
        !this->images[ImageID::H_IconBomb].loadFromFile("../assets/images/H_IconBomb.png")||
        !this->images[ImageID::H_BtnPlay1].loadFromFile("../assets/images/H_BtnPlay1.png")||
        !this->images[ImageID::H_BtnPlay2].loadFromFile("../assets/images/H_BtnPlay2.png")||
        !this->images[ImageID::H_BtnPlay3].loadFromFile("../assets/images/H_BtnPlay3.png")||
        !this->images[ImageID::H_BtnExit1].loadFromFile("../assets/images/H_BtnExit1.png")||
        !this->images[ImageID::H_BtnExit2].loadFromFile("../assets/images/H_BtnExit2.png")||
        !this->images[ImageID::H_BtnExit3].loadFromFile("../assets/images/H_BtnExit3.png")
    ) {std::cerr<<"Error: load image HomScreen\n"; return false;}
    
    /*Load ảnh của trang SelectMap*/
    if (
        !this->images[ImageID::SMap_Bg].loadFromFile("../assets/images/SMap_Bg.png")||
        !this->images[ImageID::SMap_BtnBack].loadFromFile("../assets/images/SMap_BtnBack.png")||
        !this->images[ImageID::SMap_BtnBasic].loadFromFile("../assets/images/SMap_BtnBasic.png")||
        !this->images[ImageID::SMap_BtnDesert].loadFromFile("../assets/images/SMap_BtnDesert.png")||
        !this->images[ImageID::SMap_BtnJungle].loadFromFile("../assets/images/SMap_BtnJungle.png")||
        !this->images[ImageID::SMap_BtnBeeHive].loadFromFile("../assets/images/SMap_BtnBeeHive.png")
    ) {std::cerr<<"Error: load image SelectMap\n"; return false;}

    /*Load ảnh của trang SelectMode*/
    if (
        !this->images[ImageID::SMode_Bg].loadFromFile("../assets/images/SMode_Bg.png")||
        !this->images[ImageID::SMode_BoxInfo].loadFromFile("../assets/images/SMode_BoxInfo.png")||
        !this->images[ImageID::SMode_BtnEasy].loadFromFile("../assets/images/SMode_BtnEasy.png")||
        !this->images[ImageID::SMode_BtnNormal].loadFromFile("../assets/images/SMode_BtnNormal.png")||
        !this->images[ImageID::SMode_BtnHard].loadFromFile("../assets/images/SMode_BtnHard.png")||
        !this->images[ImageID::SMode_BtnExplore].loadFromFile("../assets/images/SMode_BtnExplore.png")||
        !this->images[ImageID::SMode_BtnStart].loadFromFile("../assets/images/SMode_BtnStart.png")||
        !this->images[ImageID::SMode_BtnBack].loadFromFile("../assets/images/SMode_BtnBack.png")
    ) {std::cerr<<"Error: load image SelectMode\n"; return false;}

    /*Load ảnh của Overlay*/
    if (
        !this->images[ImageID::OL_Bg].loadFromFile("../assets/images/OL_Bg.png")||
        !this->images[ImageID::OL_BtnContinue].loadFromFile("../assets/images/OL_BtnContinue.png")||
        !this->images[ImageID::OL_BtnBackToHome].loadFromFile("../assets/images/OL_BtnBackToHome.png")
    ) {std::cerr<<"Error: load image Overlay\n"; return false;}
    
    // !this->images[ImageID::].loadFromFile("../assets/images/.png");
    return true;
}

bool ResourceManager::LoadImagesBoard(MapID map)
{
    /*Load ảnh chung*/
    if (
        !this->images[ImageID::G_BtnResetLosed].loadFromFile("../assets/images/G_Basic_BtnResetLosed.png") ||
        !this->images[ImageID::G_BtnResetNormal].loadFromFile("../assets/images/G_Basic_BtnResetNormal.png") ||
        !this->images[ImageID::G_BtnResetWon].loadFromFile("../assets/images/G_Basic_BtnResetWon.png") ||
        !this->images[ImageID::G_Frame].loadFromFile("../assets/images/G_Basic_Frame.png") ||
        !this->images[ImageID::G_BtnPause].loadFromFile("../assets/images/G_Basic_BtnPause.png")
    ) {std::cerr<<"Error: load image Nút Pause, Frame và Nút Reset\n"; return false;}

    /*Load ảnh riêng*/
    switch(map)
    {
        case MapID::Basic:
        {
            //Load background
            if (
                !this->images[ImageID::G_Bg].loadFromFile("../assets/images/G_Basic_Bg.png")
            ) {std::cerr<<"Error: load image BackGround\n"; return false;}
            
            //Load các ảnh của game
            if (
                !this->images[ImageID::G_Hidded].loadFromFile("../assets/images/G_Basic_Hidded.png") ||
                !this->images[ImageID::G_Number1].loadFromFile("../assets/images/G_Basic_Number1.png") ||
                !this->images[ImageID::G_Number2].loadFromFile("../assets/images/G_Basic_Number2.png") ||
                !this->images[ImageID::G_Number3].loadFromFile("../assets/images/G_Basic_Number3.png") ||
                !this->images[ImageID::G_Number4].loadFromFile("../assets/images/G_Basic_Number4.png") ||
                !this->images[ImageID::G_Number5].loadFromFile("../assets/images/G_Basic_Number5.png") ||
                !this->images[ImageID::G_Number6].loadFromFile("../assets/images/G_Basic_Number6.png") ||
                !this->images[ImageID::G_Number7].loadFromFile("../assets/images/G_Basic_Number7.png") ||
                !this->images[ImageID::G_Number8].loadFromFile("../assets/images/G_Basic_Number8.png") ||
                !this->images[ImageID::G_Flag].loadFromFile("../assets/images/G_Basic_Flag.png") ||
                !this->images[ImageID::G_Bomb].loadFromFile("../assets/images/G_Basic_Bomb.png") ||
                !this->images[ImageID::G_Openned].loadFromFile("../assets/images/G_Basic_Openned.png") ||
                !this->images[ImageID::G_Question].loadFromFile("../assets/images/G_Basic_Question.png") ||
                !this->images[ImageID::G_Dead].loadFromFile("../assets/images/G_Basic_Dead.png") ||
                !this->images[ImageID::G_Wrong].loadFromFile("../assets/images/G_Basic_Wrong.png") ||
                !this->images[ImageID::G_Panel].loadFromFile("../assets/images/G_Basic_Panel.png")
            ) {std::cerr<<"Error: load image CoreGame\n"; return false;}
            break;
        }
        case MapID::Desert:
        {
            //Load background
            if (
                !this->images[ImageID::G_Bg].loadFromFile("../assets/images/G_Desert_Bg.png")
            ) {std::cerr<<"Error: load image BackGround\n"; return false;}
            
            //Load các ảnh của game
            if (
                !this->images[ImageID::G_Hidded].loadFromFile( "../assets/images/G_Desert_Hidded.png") ||
                !this->images[ImageID::G_Number1].loadFromFile("../assets/images/G_Desert_Number1.png") ||
                !this->images[ImageID::G_Number2].loadFromFile("../assets/images/G_Desert_Number2.png") ||
                !this->images[ImageID::G_Number3].loadFromFile("../assets/images/G_Desert_Number3.png") ||
                !this->images[ImageID::G_Number4].loadFromFile("../assets/images/G_Desert_Number4.png") ||
                !this->images[ImageID::G_Number5].loadFromFile("../assets/images/G_Desert_Number5.png") ||
                !this->images[ImageID::G_Number6].loadFromFile("../assets/images/G_Desert_Number6.png") ||
                !this->images[ImageID::G_Number7].loadFromFile("../assets/images/G_Desert_Number7.png") ||
                !this->images[ImageID::G_Number8].loadFromFile("../assets/images/G_Desert_Number8.png") ||
                !this->images[ImageID::G_Flag].loadFromFile("../assets/images/G_Basic_Flag.png") || // từ vị trí này trở xuống sài tạm ảnh của Basic
                !this->images[ImageID::G_Bomb].loadFromFile("../assets/images/G_Basic_Bomb.png") ||
                !this->images[ImageID::G_Openned].loadFromFile("../assets/images/G_Basic_Openned.png") ||
                !this->images[ImageID::G_Question].loadFromFile("../assets/images/G_Basic_Question.png") ||
                !this->images[ImageID::G_Dead].loadFromFile("../assets/images/G_Basic_Dead.png") ||
                !this->images[ImageID::G_Wrong].loadFromFile("../assets/images/G_Basic_Wrong.png") ||
                !this->images[ImageID::G_Panel].loadFromFile("../assets/images/G_Basic_Panel.png") ||
                !this->images[ImageID::G_Sand].loadFromFile("../assets/images/G_Basic_Wrong.png") // này chưa có nên dùng tạm Wrong của Basic
            ) {std::cerr<<"Error: load image CoreGame\n"; return false;}
            break;
        }
        case MapID::Jungle:
        {
            //Load background
            if (
                !this->images[ImageID::G_Bg].loadFromFile("../assets/images/G_Jungle_Bg.png")
            ) {std::cerr<<"Error: load image BackGround\n"; return false;}
            
            //Load các ảnh của game
            if (
                !this->images[ImageID::G_Hidded].loadFromFile("../assets/images/G_Jungle_Hidded.png") ||
                !this->images[ImageID::G_Openned].loadFromFile("../assets/images/G_Jungle_Openned.png") ||
                !this->images[ImageID::G_Number1].loadFromFile("../assets/images/G_Jungle_Number1.png") ||
                !this->images[ImageID::G_Number2].loadFromFile("../assets/images/G_Jungle_Number2.png") ||
                !this->images[ImageID::G_Number3].loadFromFile("../assets/images/G_Jungle_Number3.png") ||
                !this->images[ImageID::G_Number4].loadFromFile("../assets/images/G_Jungle_Number4.png") ||
                !this->images[ImageID::G_Number5].loadFromFile("../assets/images/G_Jungle_Number5.png") ||
                !this->images[ImageID::G_Number6].loadFromFile("../assets/images/G_Jungle_Number6.png") ||
                !this->images[ImageID::G_Number7].loadFromFile("../assets/images/G_Jungle_Number7.png") ||
                !this->images[ImageID::G_Number8].loadFromFile("../assets/images/G_Jungle_Number8.png") ||
                !this->images[ImageID::G_Flag].loadFromFile("../assets/images/G_Basic_Flag.png") || // từ chỗ này xuoogns sài ké Basic
                !this->images[ImageID::G_Bomb].loadFromFile("../assets/images/G_Basic_Bomb.png") ||
                !this->images[ImageID::G_Question].loadFromFile("../assets/images/G_Basic_Question.png") ||
                !this->images[ImageID::G_Dead].loadFromFile("../assets/images/G_Basic_Dead.png") ||
                !this->images[ImageID::G_Wrong].loadFromFile("../assets/images/G_Basic_Wrong.png") ||
                !this->images[ImageID::G_Panel].loadFromFile("../assets/images/G_Basic_Panel.png") ||
                !this->images[ImageID::G_Root].loadFromFile("../assets/images/G_Jungle_Root.png") // cái này của Jungle chứ k phải sài ké
            ) {std::cerr<<"Error: load image CoreGame\n"; return false;}
            break;
        }
        case MapID::BeeHive:
        {
            // từ từ rồi tính
            break;
        }
    }
    return true;
}

bool ResourceManager::LoadSounds()
{
    if (!this->soundBuffers[SoundID::BtnReleased].loadFromFile("../assets/sounds/BtnReleased.wav"))
    {std::cerr<<"Error: load sound Button Released"; return false;}
    return true;
}

bool ResourceManager::LoadMusic()
{
    if (!this->music.openFromFile("../assets/musics/chill.mp3"))
    {std::cerr<<"Error: open Music"; return false;}
    this->music.setLooping(true);
    return true;
}


bool ResourceManager::LoadFont()
{
    if (
        !this->fonts[FontID::PressStart2P].openFromFile("../assets/fonts/PressStart2P-Regular.ttf") ||
        !this->fonts[FontID::DigitalNumber].openFromFile("../assets/fonts/DigitalNumbers-Regular.ttf")
    ) {std::cerr<<"Error: open Font"; return false;}
    return true;
}

const sf::Texture& ResourceManager::GetImage(const ImageID& id) const
{ return this->images.at(id); }

const sf::Font& ResourceManager::GetFont(const FontID& id) const
{ return this->fonts.at(id); }

void ResourceManager::PlaySound(const SoundID& id)
{
    if (!this->soundOn)
    { return; }

    // Dọn dẹp các âm thanh đã phát xong
    this->playingSounds.erase(
        std::remove_if(
            this->playingSounds.begin(),

            this->playingSounds.end(),

            // Hàm tạm thời | nếu trạng thái âm thanh Stop thì xóa ra khỏi vector
            [](const sf::Sound& s)
            { 
                return s.getStatus() == sf::Sound::Status::Stopped; 
            }
        ),
        this->playingSounds.end()
    );

    // Lấy trực tiếp SoundBuffer từ map
    // bỏ vào cuối 
    this->playingSounds.emplace_back(this->soundBuffers[id]); // giống push_back nhưng nhanh hơn
    // phát âm thanh ở cuối danh sách
    this->playingSounds.back().play();
}
void ResourceManager::PlayMusic()
{ this->music.play(); }

// bật tắt nhạc/âm thanh
bool ResourceManager::ToggleMusic()
{
    this->musicOn = !this->musicOn;
    if (!this->musicOn) this->music.pause();
    else PlayMusic();
    return this->musicOn;
}
bool ResourceManager::ToggleSound()
{
    this->soundOn = !this->soundOn;
    if (!this->soundOn) this->playingSounds.clear();
    return this->soundOn;
}

bool ResourceManager::MusicCheckOn() const
{ return this->musicOn; }

bool ResourceManager::SoundCheckOn() const
{ return this->soundOn; }