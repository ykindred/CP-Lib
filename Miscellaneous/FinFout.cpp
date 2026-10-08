static const int WBUF_SIZE = 1 << 20; 
int wptr = 0;
char wbuf[WBUF_SIZE];

inline void flushOut() {
    if (wptr) {
        fwrite(wbuf, 1, wptr, stdout);
        wptr = 0;
    }
}

inline void writeChar(char c) {
    if (wptr == WBUF_SIZE) flushOut();
    wbuf[wptr++] = c;
}

inline void writeInt(__int128 x) {
    if (x == 0) {
        writeChar('0');
        return;
    }
    if (x < 0) {
        writeChar('-');
        x = -x;
    }
    int t = 0;
    char s[45]; 
    while (x > 0) {
        s[t++] = (char)('0' + (x % 10));
        x /= 10;
    }
    while (t > 0) {
        writeChar(s[--t]);
    }
}
static const int RBUF_SIZE = 1 << 20; 
char rbuf[RBUF_SIZE];
int ridx = 0, rlen = 0;

static inline int getCharRaw() {
    if (ridx == rlen) {
        rlen = (int)fread(rbuf, 1, RBUF_SIZE, stdin);
        ridx = 0;
        if (rlen == 0) return EOF;
    }
    return (unsigned char)rbuf[ridx++];
}

static inline char readChar() {
    int c;
    do {
        c = getCharRaw();
    } while (c <= ' ' && c != EOF);
    return (char)c;
}
static inline char readCharRaw() {
    return (char)getCharRaw();
}
static inline __int128 readInt() {
    int c = getCharRaw();
    while (c <= ' ' && c != EOF) c = getCharRaw();

    if (c == EOF) return 0;

    int sign = 1;
    if (c == '-') {
        sign = -1;
        c = getCharRaw();
    } else if (c == '+') {
        c = getCharRaw();
    }

    __int128 x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getCharRaw();
    }
    return sign * x;
}
