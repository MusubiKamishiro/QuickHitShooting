#pragma once

// 型が未確定のまま使える(使うときに型が決定する)
template<typename T>
struct Vector2
{
	Vector2() : x(0), y(0) {}
	Vector2(T inx, T iny) : x(inx), y(iny) {}
	T x;
	T y;

	Vector2<T> operator+(const Vector2<T>& in)
	{
		return Vector2<T>(in.x + x, in.y + y);
	}
	void operator+=(const Vector2<T>& in)
	{
		x += in.x;
		y += in.y;
	}
	void operator-=(const Vector2<T>& in)
	{
		x -= in.x;
		y -= in.y;
	}
	void operator*=(const T& in)
	{
		x *= in;
		y *= in;
	}
	void operator/=(const T& in)
	{
		x /= in;
		y /= in;
	}
	void operator/=(const Vector2<T>& in)
	{
		x /= in.x;
		y /= in.y;
	}

	bool operator==(const Vector2<T>& in)const
	{
		return ((x == in.x) && (y == in.y));
	}
	bool operator!=(const Vector2<T>& in)const
	{
		return ((x != in.x) || (y != in.y));
	}
	bool operator|=(const Vector2<T>& in)const
	{
		return ((x == in.x) || (y == in.y));
	}
};

/// サイズを表す構造体
struct Size {
	Size() : width(0), height(0) {}
	Size(int inw, int inh) : width(inw), height(inh) {}
	int width;	// 幅
	int height;	// 高さ
};

/// 矩形を表現する構造体
struct Rect {
	Rect() : center(0, 0), size(0, 0) {}
	Rect(int x, int y, int inw, int inh) : center(x, y), size(inw, inh) {}
	Rect(const Vector2<int>& inc, const Size& ins) : center(inc), size(ins) {}

	Vector2<int> center;	// 中心
	Size size;				// 幅と高さ


	const int Left()const
	{
		return center.x - size.width / 2;
	}
	const int Top()const
	{
		return center.y - size.height / 2;
	}
	const int Right()const
	{
		return center.x + size.width / 2;
	}
	const int Bottom()const
	{
		return center.y + size.height / 2;
	}
	const int Width()const
	{
		return size.width;
	}
	const int Height()const
	{
		return size.height;
	}
};

class Geometry
{
public:
	Geometry();
	~Geometry();
};

