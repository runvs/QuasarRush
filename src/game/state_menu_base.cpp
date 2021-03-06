#include "state_menu_base.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "game_state.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "tween_alpha.hpp"

void StateMenuBase::create(std::shared_ptr<jt::renderTarget> rt, jt::GameState& state, bool en)
{
    m_input = state.getGame()->input();
    m_background = jt::dh::createRectShape(GP::GetScreenSize(), GP::PaletteBackground());

    m_quasarImage = std::make_shared<jt::Sprite>();
    m_quasarImage->loadSprite("assets/quasar_orig.png");
    m_quasarImage->setScale(jt::Vector2 { 1.0f, 1.0f });
    m_quasarImage->setPosition(jt::Vector2 { 60.0f, 0.0f });
    m_quasarImage->update(0.1f);

    m_titleLogo = std::make_shared<jt::Sprite>();
    if (en) {
        m_titleLogo->loadSprite("assets/qr_logo_en.png");
    }
    else
    {
        m_titleLogo->loadSprite("assets/qr_logo_jp.png");
        m_titleLogo->setScale(jt::Vector2{0.4f,0.4f});
    }
    m_titleLogo->setPosition(jt::Vector2{10,10});



    m_text_Subtitle = jt::dh::createText(rt, "", 14U, GP::PaletteFontFront());
    m_text_Subtitle->setPosition({ GP::GetScreenSize().x() / 2.0f, 170.0f });
    m_text_Subtitle->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 2, 2 });

    auto tween = jt::TweenAlpha<jt::DrawableInterface>::create(m_titleLogo, 0.55f, 0, 255);
    tween->setStartDelay(0.2f);
    tween->setSkipFrames();
    state.add(tween);

    m_vignette = jt::dh::createVignette(GP::GetScreenSize());
    m_vignette->setColor({ 255, 255, 255, 110 });

    m_overlay = jt::dh::createRectShape(GP::GetScreenSize(), jt::colors::Black);

    auto tweenAlpha = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.75f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tweenAlpha->setSkipFrames();
    state.add(tweenAlpha);
}

void StateMenuBase::update(float elapsed)
{
    auto const mp = m_input->mouse()->getMousePositionWorld();

    jt::Vector2 const screenCenter = GP::GetScreenSize() / 2.0f;

    auto dif = screenCenter - mp;

    float const sxf = dif.x() / (GP::GetScreenSize().x() / 2.0f);
    float const syf = dif.y() / (GP::GetScreenSize().y() / 2.0f);


    m_text_Subtitle->setShadowOffset(jt::Vector2{3.0f* sxf, 3.0f*syf} );

    m_background->update(elapsed);
    m_quasarImage->update(elapsed);
    m_titleLogo->update(elapsed);

    m_text_Subtitle->update(elapsed);

    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateMenuBase::draw(std::shared_ptr<jt::renderTarget> rt)
{
    m_background->draw(rt);
    m_quasarImage->draw(rt);
    m_titleLogo->draw(rt);
    m_text_Subtitle->draw(rt);
}
void StateMenuBase::drawOverlay(std::shared_ptr<jt::renderTarget> rt)
{
    m_vignette->draw(rt);
    m_overlay->draw(rt);
}
void StateMenuBase::startFadeOut(std::function<void(void)> callback, jt::GameState& state)
{
    auto tw = jt::TweenAlpha<jt::DrawableInterface>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback(callback);
    state.add(tw);
}

void StateMenuBase::setSubtitleText(std::string const& str) { m_text_Subtitle->setText(str); }
