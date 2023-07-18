#include <cstdio>
#include <cstring>

const int _Size = 16;

class io_r {
private:
	static char buf[];
	static int p;
	FILE* f;
	inline void flush() { fread(buf, sizeof(char), 1 << _Size, f), p = 0; }

public:
	inline io_r& operator>>(char& ch) {
		if (p == (1 << _Size)) flush();
		ch = buf[p++];
		return *this;
	}
	template <typename T>
	inline io_r& operator>>(T& x) {
		x = 0;
		short w = 1;
		char ch = 0;
		do w = ch == '-' ? -1 : w, *this >> ch;
		while (ch < '0' || ch > '9');
		do x = (x << 3) + (x << 1) + ch - '0', *this >> ch;
		while ('0' <= ch && ch <= '9');
		x *= w;
		return *this;
	}
	io_r(FILE* f = stdin) : f(f) {}
};

class io_w {
private:
	static char buf[];
	static int p;
	FILE* f;
	inline void flush() { fwrite(buf, p, 1, f), p = 0; }

public:
	inline io_w& operator<<(const char c) {
		if (p == (1 << _Size)) flush();
		buf[p++] = c;
		return *this;
	}
	inline io_w& operator<<(const char* c) {
		int len = strlen(c);
		for (int i = 0; i < len; ++i) *this << c[i];
		return *this;
	}
	template <typename T>
	inline io_w& operator<<(T x) {
		if (x < 0) *this << '-', x = -x;
		static int s[50], d = 0;
		do s[++d] = x % 10, x /= 10;
		while (x);
		do *this << (char)(s[d--] + '0');
		while (d);
		return *this;
	}
	~io_w() { flush(); }
	io_w(FILE* f = stdout) : f(f) {}
};

char io_r::buf[1 << _Size];
int io_r::p = 0;
char io_w::buf[1 << _Size];
int io_w::p = 0;
