#include "state_menu_base.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "game_state.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "tween_alpha.hpp"

void StateMenuBase::create(std::shared_ptr<jt::renderTarget> rt, jt::GameState& state)
{
    m_background = jt::dh::createRectShape(GP::GetScreenSize(), GP::PaletteBackground());

    m_quasarImage = std::make_shared<jt::Sprite>();
    m_quasarImage->loadSprite("assets/quasar_orig.png");
    m_quasarImage->setScale(jt::Vector2 { 0.4f, 0.4f });
    m_quasarImage->setPosition(jt::Vector2 { 100.0f, 0.0f });

    m_quasarImage->update(0.1f);

    m_text_Title = jt::dh::createText(rt, GP::GameName(), 20U, GP::PaletteFontFront());
    m_text_Title->setPosition({ 10, 6 });
    m_text_Title->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Title->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 3, 3 });

    auto tween = jt::TweenAlpha<jt::Text>::create(m_text_Title, 0.55f, 0, 255);
    tween->setStartDelay(0.2f);
    tween->setSkipFrames();
    state.add(tween);

    m_vignette = jt::dh::createVignette(GP::GetScreenSize());
    m_vignette->setColor({ 255, 255, 255, 110 });

    m_overlay = jt::dh::createRectShape(GP::GetScreenSize(), jt::colors::Black);

    auto tweenAlpha = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 1.25f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tweenAlpha->setSkipFrames();
    state.add(tweenAlpha);
}

void StateMenuBase::update(float elapsed)
{
    m_background->update(elapsed);
    m_quasarImage->update(elapsed);
    m_text_Title->update(elapsed);

    auto sc = m_text_Title->getShadowColor();
    sc.a() = m_text_Title->getColor().a();
    m_text_Title->setShadowColor(sc);

    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateMenuBase::draw(std::shared_ptr<jt::renderTarget> rt)
{
    m_background->draw(rt);
    m_quasarImage->draw(rt);
    m_text_Title->draw(rt);
}
void StateMenuBase::drawOverlay(std::shared_ptr<jt::renderTarget> rt) {
    m_vignette->draw(rt);
    m_overlay->draw(rt);

}
void StateMenuBase::startFadeOut(std::function<void(void)> callback, jt::GameState& state)
{
    auto tw = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback(callback);
    state.add(tw);
}

