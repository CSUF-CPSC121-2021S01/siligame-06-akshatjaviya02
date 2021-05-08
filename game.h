#include "cpputils/graphics/image_event.h"
#include "opponent.h"
#include "player.h"

#ifndef GAME_H
#define GAME_H

class Game : public graphics::AnimationEventListener,
             public graphics::MouseEventListener {
 public:
  Game() : screen_(800, 600) {}
  Game(int width, int height) : screen_(width, height) {}
  graphics::Image &GetGameScreen() { return screen_; }
  std::vector<std::unique_ptr<PlayerProjectile>> &GetPlayerProjectiles() {
    return player_pro;
  }
  std::vector<std::unique_ptr<OpponentProjectile>> &GetOpponentProjectiles() {
    return opponent_pro;
  }
  std::vector<std::unique_ptr<Opponent>> &GetOpponents() { return opponent_; }
  Player &GetPlayer() { return player_; }
  void CreateOpponents();
  void Init();
  void UpdateScreen();
  void Start(graphics::Image &game) { game.ShowUntilClosed(); }
  void MoveGameElements();
  void FilterIntersections();
  void OnAnimationStep();
  void OnMouseEvent(const graphics::MouseEvent &mouseObject);
  void LaunchProjectiles();
  void RemoveInactive();
  int GetScore();
  bool HasLost();
  int HasLife();

 private:
  int oppo_life = 0;
  int oppo_update = 200;
  int opponent_sheild = 1;
  std::string sheild_display = "Sheild: ";
  std::string life_display = "Life: ";
  int sheildupdate = 100;
  int sheild = 1;
  int lifeupdate = 200;
  int life = 3;
  std::string player_lose = "Player has lost\n score: ";
  graphics::Image screen_;
  std::vector<std::unique_ptr<PlayerProjectile>> player_pro;
  std::vector<std::unique_ptr<OpponentProjectile>> opponent_pro;
  std::vector<std::unique_ptr<Opponent>> opponent_;
  Player player_;
  int game_score_ = 0;
  bool Lost = false;
};
#endif
