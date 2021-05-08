#include "game.h"
int const ENEMY_SHE = 200;
int const SHEILD_CON = 100;
int const LIFE_CON = 200;
void Game::Init() {
  screen_.AddMouseEventListener(*this);
  screen_.AddAnimationEventListener(*this);
  player_.SetX(10);
  player_.SetY(screen_.GetHeight() - 75);
}

void Game::CreateOpponents() {
  for (int i = 0; i < 4; i++) {
    opponent_.push_back(std::make_unique<Opponent>(50 * i + 15, 50));
  }
}
int Game::HasLife() { return life; }
int Game::GetScore() { return game_score_; }

bool Game::HasLost() { return Lost; }

void Game::UpdateScreen() {
  screen_.DrawRectangle(0, 0, 800, 600, 255, 255, 255);
  for (int i = 0; i < opponent_.size(); i++) {
    if (opponent_[i]->GetIsActive() == true) {
      if (Lost == false) {
        if (game_score_ > oppo_update) {
          opponent_sheild++;
          oppo_update += ENEMY_SHE;
        }
        opponent_[i]->Draw(screen_);
      }
    }
  }
  for (int i = 0; i < opponent_pro.size(); i++) {
    if (opponent_pro[i]->GetIsActive() == true) {
      if (Lost == false) {
        opponent_pro[i]->Draw(screen_);
      }
    }
  }
  for (int i = 0; i < player_pro.size(); i++) {
    if (player_pro[i]->GetIsActive() == true) {
      if (Lost == false) {
        player_pro[i]->Draw(screen_);
      }
    }
  }
  if (player_.GetIsActive() == true || life > 0) {
    if (Lost == false) {
      if (game_score_ > sheildupdate) {
        sheild++;
        sheildupdate = sheildupdate + SHEILD_CON;
      }
      if (game_score_ > lifeupdate) {
        life++;
        lifeupdate += LIFE_CON;
      }
      player_.Draw(screen_);
    }
  } else {
    std::string addition;
    addition = player_lose + std::to_string(game_score_);
    screen_.DrawText(300, 250, addition, 50, 0, 0, 0);
  }
  std::string s_dis, l_dis;
  s_dis = sheild_display + std::to_string(sheild);
  l_dis = life_display + std::to_string(life);
  screen_.DrawText(670, 10, s_dis, 35, 0, 0, 255);
  screen_.DrawText(700, 550, l_dis, 35, 255, 0, 0);
  screen_.DrawText(10, 10, std::to_string(game_score_), 35, 0, 0, 0);
}

void Game::MoveGameElements() {
  for (int i = 0; i < player_pro.size(); i++) {
    player_pro[i]->Move(screen_);
  }
  for (int i = 0; i < opponent_pro.size(); i++) {
    opponent_pro[i]->Move(screen_);
  }
  for (int i = 0; i < opponent_.size(); i++) {
    opponent_[i]->Move(screen_);
  }
}

void Game::FilterIntersections() {
  for (int j = 0; j < opponent_.size(); j++) {
    if (player_.IntersectsWith(opponent_[j].get()) == true) {
      if(opponent_sheild >= 1) {
        opponent_sheild--;
      } else if (opponent_sheild == 0){
        opponent_[j]->SetIsActive(false);
      }
      if (life > 0) {
        if (sheild > 0) {
          sheild--;
        } else {
          life--;
        }
      } else {
        player_.SetIsActive(false);
      }
    }
  }
  for (int i = 0; i < opponent_pro.size(); i++) {
    if (opponent_pro[i]->IntersectsWith(&player_) == true) {
      opponent_pro[i]->SetIsActive(false);
      if (life > 0) {
        if (sheild > 0) {
          sheild--;
        } else {
          life--;
        }
      } else {
        player_.SetIsActive(false);
        Lost = true;
      }
    }
  }
  for (int a = 0; a < player_pro.size(); a++) {
    for (int b = 0; b < opponent_.size(); b++) {
      if (player_pro[a]->IntersectsWith(opponent_[b].get()) == true) {
        player_pro[a]->SetIsActive(false);
        if (opponent_sheild >= 1) {
          opponent_sheild--;
        } else if(opponent_sheild == 0){
          opponent_[b]->SetIsActive(false);
        }
        if (player_.GetIsActive() == true) {
          game_score_++;
        }
      }
    }
  }
  for (int a = 0; a < player_pro.size(); a++) {
    for (int b = 0; b < opponent_pro.size(); b++) {
      if (player_pro[a]->IntersectsWith(opponent_pro[b].get()) == true) {
        player_pro[a]->SetIsActive(false);
        opponent_pro[b]->SetIsActive(false);
      }
    }
  }
}
void Game::OnAnimationStep() {
    if (Lost == false) {
      if (opponent_.size() == 0) {
      CreateOpponents();
    }
  }
  MoveGameElements();
  LaunchProjectiles();
  FilterIntersections();
  RemoveInactive();
  UpdateScreen();
  screen_.Flush();
}

void Game::LaunchProjectiles() {
  for (int i = 0; i < opponent_.size(); i++) {
    if (opponent_[i]->LaunchProjectile() != nullptr) {
      std::unique_ptr<OpponentProjectile> pro;
      pro = std::make_unique<OpponentProjectile>();
      pro->SetX(opponent_[i]->GetX());
      pro->SetY(opponent_[i]->GetY());
      opponent_pro.push_back(std::move(pro));
    }
  }
}

void Game::OnMouseEvent(const graphics::MouseEvent &mouse) {
  if (mouse.GetX() > 0 && mouse.GetY() > 0 &&
      mouse.GetX() < screen_.GetWidth() && mouse.GetY() < screen_.GetHeight()) {
    player_.SetX(mouse.GetX() - 25);
    player_.SetY(mouse.GetY() - 25);
  }
  if (mouse.GetMouseAction() == graphics::MouseAction::kPressed) {
    std::unique_ptr<PlayerProjectile> playpro =
        std::make_unique<PlayerProjectile>(mouse.GetX(), mouse.GetY() + 25);
    player_pro.push_back(std::move(playpro));
  }
  if (mouse.GetMouseAction() == graphics::MouseAction::kDragged) {
    std::unique_ptr<PlayerProjectile> playpro =
        std::make_unique<PlayerProjectile>(mouse.GetX(), mouse.GetY() + 25);
    player_pro.push_back(std::move(playpro));
  }
}

void Game::RemoveInactive() {
  for (int i = player_pro.size() - 1; i >= 0; i--) {
    if (player_pro[i]->GetIsActive() == false) {
      player_pro.erase(player_pro.begin() + i);
    }
  }
  for (int i = opponent_pro.size() - 1; i >= 0; i--) {
    if (opponent_pro[i]->GetIsActive() == false) {
      opponent_pro.erase(opponent_pro.begin() + i);
    }
  }
  for (int i = opponent_.size() - 1; i >= 0; i--) {
    if (!opponent_[i]->GetIsActive()) {
      opponent_.erase(opponent_.begin() + i);
    }
  }
}
