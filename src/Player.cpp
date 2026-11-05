#include "../headers/Player.hpp"

Player::Player(int lives) : lives(lives) {}

void Player::LoseLife()
{ if (lives > 0) this->lives--; }

bool Player::IsDead()
{ return (this->lives == 0); }

void Player::OnFlagPlaced(bool isMide)
{
    if (isMide) this->correctFlags++;
    else this->wrongFlags++;
}

void Player::OnFlagRemoved(bool isMide)
{
    if (isMide) this->correctFlags--;
    else this->wrongFlags--;
}

// 1 đúng = 3 đieemr
int Player::GetScore()
{ return (this->correctFlags-this->wrongFlags)*3; }


