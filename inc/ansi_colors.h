/* This is a reused code */
#ifndef __ANSI_COLORS_H_
#define __ANSI_COLORS_H_

#include <string>
#include <sstream>
#include <fstream>

namespace TermAnsiColor
{

// enabling or disabling coloring
static bool no_color 
#ifndef _WIN32
 __attribute__((unused)) = false;
#else
 = true;
#endif

// represents term color, can output to std::ostream or using as std::string
class AnsiColor
{
public:
	template<class T>
	class BaseColor
	{
	public:
		inline operator int() const { return static_cast<const T*>(this)->val; }
		inline bool operator!() const { return static_cast<const T*>(this)->val == static_cast<const T*>(this)->NIL; }
		virtual ~BaseColor() {}
	};

	class Style : public AnsiColor::BaseColor<AnsiColor::Style>
	{
	public:
		enum type {NIL = -1, CLEAR = 0, BOLD = 1, DARK = 2, ITALIC = 3, UNDERLINE = 4, BLINK = 5, INVERSE = 7} val;
		inline Style(AnsiColor::Style::type v = AnsiColor::Style::NIL) : val(v) {}
	};

	class Foreground : public AnsiColor::BaseColor<AnsiColor::Foreground>
	{
	public:
		enum type {NIL = -1, BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37, DEFAULT = 39} val;
		inline Foreground(AnsiColor::Foreground::type v = AnsiColor::Foreground::NIL) : val(v) {}
	};

	class Background : public AnsiColor::BaseColor<AnsiColor::Background>
	{
	public:
		enum type {NIL = -1, BLACK = 40, RED = 41, GREEN = 42, YELLOW = 43, BLUE = 44, MAGENTA = 45, CYAN = 46, WHITE = 47, DEFAULT = 49} val;
		inline Background(AnsiColor::Background::type v = AnsiColor::Background::NIL) : val(v) {}
	};

public:
	inline AnsiColor(AnsiColor::Foreground fg = AnsiColor::Foreground(AnsiColor::Foreground::DEFAULT)) :
		_style(), _fg(fg), _bg()
	{}
	inline AnsiColor(AnsiColor::Foreground fg, AnsiColor::Background bg) :
		_style(), _fg(fg), _bg(bg)
	{}
	inline AnsiColor(AnsiColor::Foreground fg, AnsiColor::Style style) :
		_style(style), _fg(fg), _bg()
	{}
	inline AnsiColor(AnsiColor::Style style) :
		_style(style), _fg(), _bg()
	{}
	inline AnsiColor(AnsiColor::Background bg) :
		_style(), _fg(), _bg(bg)
	{}

	inline AnsiColor::Style style() const
	{
		return _style;
	}

	inline AnsiColor::Foreground fg() const
	{
		return _fg;
	}

	inline AnsiColor::Background bg() const
	{
		return _bg;
	}

	inline std::string to_string() const
	{
		if (no_color)
			return "";
		std::string r;
		if (!!_style)
			r += esc_wrap(_style);
		if (!!_fg)
			r += esc_wrap(_fg);
		if (!!_bg)
			r += esc_wrap(_bg);
		return r;
	}

	inline operator std::string() const
	{
		return to_string();
	}

	inline AnsiColor operator()() const
	{
		return *this;
	}

	inline static AnsiColor reset()
	{
		return AnsiColor(Foreground::DEFAULT, Style::CLEAR);
	}

protected:
	inline static std::string esc_wrap(int v)
	{
		std::ostringstream ss;
		ss << "\033[" << v << "m";
		return ss.str();
	}

private:
	Style _style;
	Foreground _fg;
	Background _bg;
};

// DEFAULT COLOR AND STYLE
static const AnsiColor DEFAULT = AnsiColor(AnsiColor::Foreground::DEFAULT, AnsiColor::Style::CLEAR);
// STYLES
static const AnsiColor CLEAR = AnsiColor(AnsiColor::Style::CLEAR);
static const AnsiColor BOLD = AnsiColor(AnsiColor::Style::BOLD);
static const AnsiColor ITALIC = AnsiColor(AnsiColor::Style::ITALIC);
static const AnsiColor UNDERLINE = AnsiColor(AnsiColor::Style::UNDERLINE);
static const AnsiColor BLINK = AnsiColor(AnsiColor::Style::BLINK);
// FOREGROUND COLORS
static const AnsiColor BLACK = AnsiColor(AnsiColor::Foreground::BLACK);
static const AnsiColor RED = AnsiColor(AnsiColor::Foreground::RED);
static const AnsiColor GREEN = AnsiColor(AnsiColor::Foreground::GREEN);
static const AnsiColor YELLOW = AnsiColor(AnsiColor::Foreground::YELLOW);
static const AnsiColor BLUE = AnsiColor(AnsiColor::Foreground::BLUE);
static const AnsiColor MAGENTA = AnsiColor(AnsiColor::Foreground::MAGENTA);
static const AnsiColor CYAN = AnsiColor(AnsiColor::Foreground::CYAN);
static const AnsiColor WHITE = AnsiColor(AnsiColor::Foreground::WHITE);
// BACKGROUND COLORS
static const AnsiColor ON_BLACK = AnsiColor(AnsiColor::Background::BLACK);
static const AnsiColor ON_RED = AnsiColor(AnsiColor::Background::RED);
static const AnsiColor ON_GREEN = AnsiColor(AnsiColor::Background::GREEN);
static const AnsiColor ON_YELLOW = AnsiColor(AnsiColor::Background::YELLOW);
static const AnsiColor ON_BLUE = AnsiColor(AnsiColor::Background::BLUE);
static const AnsiColor ON_MAGENTA = AnsiColor(AnsiColor::Background::MAGENTA);
static const AnsiColor ON_CYAN = AnsiColor(AnsiColor::Background::CYAN);
static const AnsiColor ON_WHITE = AnsiColor(AnsiColor::Background::WHITE);

// allows construct STYLE+FOREGROUND, FOREGROUND+BACKGROUND
// no commutative operation, c1+c2 is not always equal c2+c1
// c1 color is prefer than c2 color
inline AnsiColor operator+(const AnsiColor& c1, const AnsiColor& c2)
{
	const AnsiColor::Style style = !c1.style() ? c2.style() : c1.style();
	const AnsiColor::Foreground fg = !c1.fg() ? c2.fg() : c1.fg();
	const AnsiColor::Background bg = !c1.bg() ? c2.bg() : c1.bg();
	if (!style)
		return AnsiColor(fg, bg);
	else
		return AnsiColor(fg, style);
}

inline std::ostream& operator<<(std::ostream& os, const AnsiColor& color)
{
	os << color.to_string();
	return os;
}

inline AnsiColor reset()
{
	return AnsiColor::reset();
}

// wrap string in color, implies autoreset color and style at end
template<class COLOR>
inline std::string wrap(const std::string& str, const COLOR& color)
{
	return std::string(color()) + str + std::string(reset());
}

inline AnsiColor red() { return AnsiColor(AnsiColor::Foreground::RED); }
inline AnsiColor green() { return AnsiColor(AnsiColor::Foreground::GREEN); }
inline AnsiColor yellow() { return AnsiColor(AnsiColor::Foreground::YELLOW); }
inline AnsiColor blue() { return AnsiColor(AnsiColor::Foreground::BLUE); }

inline AnsiColor red_on_yellow() { return AnsiColor(AnsiColor::Foreground::RED, AnsiColor::Background::YELLOW); }
inline AnsiColor green_on_blue() { return AnsiColor(AnsiColor::Foreground::GREEN, AnsiColor::Background::BLUE); }
inline AnsiColor bold_red() { return AnsiColor(AnsiColor::Foreground::RED, AnsiColor::Style::BOLD); }
inline AnsiColor bold_green() { return AnsiColor(AnsiColor::Foreground::GREEN, AnsiColor::Style::BOLD); }
inline AnsiColor bold_blue() { return AnsiColor(AnsiColor::Foreground::BLUE, AnsiColor::Style::BOLD); }

}

#endif /* __ANSI_COLORS_H_ */

