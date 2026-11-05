#pragma once

class Player
{
    private:
        int lives;
        int correctFlags = 0 , wrongFlags = 0;
    
    public:
        Player(int = 1);

        void LoseLife(); // trừ 1 mạng 
        bool IsDead(); // kiểm tra hết mạng 
        void OnFlagPlaced(bool isMine); // tăng cờ đặt cho correctFlags nếu đặt đúng vào mìn, wrongFlags nếu đặt vào ô k phải mìn 
        void OnFlagRemoved(bool isMine); // giảm cờ đặt cho correctFlags hoặc wrongFlags như trên
        int GetScore(); // chỉ dùng khi người chơi thua 
};