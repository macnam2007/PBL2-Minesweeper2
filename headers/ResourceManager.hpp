#pragma once

#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <vector>
#include <map>
#include "EnumID.hpp"

class ResourceManager
{
    private:

        std::map<ImageID, sf::Texture> images;
        std::map<SoundID, sf::SoundBuffer> soundBuffers;
        std::map<FontID,sf::Font> fonts;
        std::vector<sf::Sound> playingSounds; // giúp nhiều âm thanh hoạt động cùng lúc
        sf::Music music;

        bool musicOn = true;
        bool soundOn = true;

    public:
        ResourceManager(){};
        ~ResourceManager(){};

        bool LoadImagesGlobal();
        bool LoadImagesBoard(MapID map);
        bool LoadFont();
        bool LoadSounds();
        bool LoadMusic();

        const sf::Texture& GetImage(const ImageID& id) const;
        const sf::Font& GetFont(const FontID& id) const;

        void PlayMusic(); // Dùng khi vừa mở game 
        void PlaySound(const SoundID& id);

        // bật tắt nhạc/âm thanh
        bool ToggleMusic();
        bool ToggleSound();

        bool MusicCheckOn() const;
        bool SoundCheckOn() const;
};