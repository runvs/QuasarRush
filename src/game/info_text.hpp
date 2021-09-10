#ifndef QUASARRUSH_INFO_TEXT_HPP
#define QUASARRUSH_INFO_TEXT_HPP

#include "game_object.hpp"
#include "drawable_interface.hpp"

namespace jt{
class Text;
}

enum eInfoTextAlign
{
    RightDown,
    RightUp,
    LeftDown,
    LeftUp
};

class InfoText : public jt::GameObject {
public:
    InfoText(
        std::shared_ptr<jt::DrawableInterface> drawable, std::string text, eInfoTextAlign align = RightDown);

    void setColor(jt::Color const& col);
    jt::Color getColor() const;

    void setOffset(jt::Vector2 const&);

private:
    std::weak_ptr<jt::DrawableInterface> m_objectToTrack;

    std::shared_ptr<jt::Text> m_text;
    std::string m_textstring;

    eInfoTextAlign m_align;
    jt::Vector2 m_diagonalVector;
    jt::Color m_color;
    jt::Vector2 m_offset;


    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

};

#endif // QUASARRUSH_INFO_TEXT_HPP
