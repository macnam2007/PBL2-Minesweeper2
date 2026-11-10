#pragma once

// kết quả khi mở ô
enum class OpenResult
{
    Safe,  // ô mở k phải mìn 
    Lose,  // ô mở là mìn (chế độ != Bee's Hive)
    LostLife, // ô mở là ong (chế độ Bee's Hive)
    Blocked, // ô đó không mở đc
    Win,
};