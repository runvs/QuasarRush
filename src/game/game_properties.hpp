#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "color.hpp"
#include "vector.hpp"
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "Quasar Rush"; }
    static std::string AuthorName() { return "Thunraz & adkiem & Laguna_999"; }
    static std::string JamName() { return "AGBIC"; }
    static std::string JamDate() { return "Aug 2021"; }

    static jt::Vector2 GetWindowSize() { return jt::Vector2 { 800, 600 }; }
    static float GetZoom() { return 2.0f; }
    static jt::Vector2 GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::PaletteColor6(); }
    static jt::Color PaletteFontFront() { return GP::PaletteColor4(); }
    static jt::Color PaletteFontShadow() { return GP::PaletteColor7(); }

    static jt::Color PaletteColor1() { return jt::Color { 5, 64, 71 }; }
    static jt::Color PaletteColor2() { return jt::Color { 32, 142, 68 }; }
    static jt::Color PaletteColor3() { return jt::Color { 133, 225, 66 }; }
    static jt::Color PaletteColor4() { return jt::Color { 255, 255, 255 }; }
    static jt::Color PaletteColor5() { return jt::Color { 251, 114, 48 }; }
    static jt::Color PaletteColor6() { return jt::Color { 0, 0, 0 }; }
    static jt::Color PaletteColor7() { return jt::Color { 64, 64, 64 }; }
    static jt::Color PaletteColor8() { return jt::Color { 157, 64, 43 }; }

    static float PlayerRotationSpeed() { return 100.0; }
    static float PlayerAcceleration() { return 3.5; }
    static float PlayerAccelerationBoostFactor() { return 2.0f; }

    static float PlayerShootTimerMg() { return 0.09f; }
    static float PlayerShootTimerMissile() { return 5.0f; }

    static float ShotSpeed() { return 55.0f; }

    static int PlayerHalfSize() { return 9; }
    static int PlanetHalfSize() { return 16; }

    static float EnemyShootTimer() { return 2.5f; }
    static int EnemyHalfSize() { return 8; }
    static int WeaponMgMagazineSize() { return 20; }
    static float WeaponMgReloadTimer() {return 2.0f;}
};

#endif
