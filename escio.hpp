#include <cstdio>
#include <cstring>

const size_t BUFLEN = (1 << 16);

struct io_read {
	static char buf[];
	static int p;
	inline char bit() {
		if (p == BUFLEN) flush();
		return buf[p++];
	}
	inline void flush() {
		if (!p) return;
		fread(buf, sizeof(char), BUFLEN, stdin);
		p = 0;
	}
	io_read() { flush(); }
} __io_read;

char io_read::buf[BUFLEN];
int io_read::p = 0;

template <typename T>
inline void read(T& x) {
	x = 0;
	int w = 1;
	char ch = 0;
	do {
		w = ch == '-' ? -1 : w;
		ch = __io_read.bit();
	} while (!isdigit(ch));
	do {
		x = x * 10 + ch - '0';
		ch = __io_read.bit();
	} while (isdigit(ch));
	x *= w;
}

template <typename T, typename... _T>
inline void read(T& x, _T&... args) {
	read(x), read(args...);
}

struct io_print {
	static char buf[];
	static int p;
	io_print& operator<<(char c) {
		if (p == BUFLEN) flush();
		buf[p++] = c;
		return *this;
	}
	inline void flush() {
		if (!p) return;
		fwrite(buf, p, 1, stdout);
		p = 0;
	}
	~io_print() { flush(); }
} __io_print;

char io_print::buf[BUFLEN];
int io_print::p = 0;

inline void print(const char a[]) {
	int l = strlen(a);
	for (int i = 0; i < l; ++i) __io_print << a[i];
}

inline void print(const char s) { __io_print << s; }

template <typename T>
inline void print(T x) {
	if (x < 0) __io_print << '-', x = -x;
	static char s[45];
	int d = 0;
	do {
		s[++d] = (char)((x % 10) + '0'), x /= 10;
	} while (x);
	do {
		__io_print << s[d--];
	} while (d);
}

template <typename T, typename... _T>
inline void print(T x, _T... args) {
	print(x), print(args...);
}
