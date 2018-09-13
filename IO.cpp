
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
template <class T> inline void writeInt( T x, char end = 0 );
inline void writeChar( int x );
inline void writeWord( const char *s );
template <class T> inline void writeUInt( T x, int len = -1 );
static const int buf_size = 4096;
static int write_pos = 0;
static int write_buf_pos = 0;
static char write_buf[buf_size];
static char buf[buf_size];
static int buf_len = 0, buf_pos = 0;
//-- read
template <class T = int> inline T readInt();
inline double readDouble();
inline int readUInt();
inline int readChar();
inline void readWord( char *s );
inline bool readLine( char *s ); // do not save '\n'
inline bool isEof();
inline int peekChar();
inline bool seekEof();
inline void flush();
//------
struct Flusher {
    ~Flusher() {
        if (write_pos)
            fwrite(write_buf, 1, write_pos, stdout), write_pos = 0;
    }
} flusher;
inline void flush() {
    if (write_buf_pos)
        fwrite(write_buf, 1, write_buf_pos, stdout), write_buf_pos = 0;
}
//__________________________
class TOutputStream
{
public:
    int precision;
    TOutputStream()
    {
        
    }
    TOutputStream(int new_prec)
    {
        precision = new_prec;
    }
    ~TOutputStream()
    {
        flush();
    }
};
//--------------------------
class InputStream
{
public:
    int precision;
    InputStream()
    {
        
    }
    InputStream(int new_prec)
    {
        precision = new_prec;
    }
    ~InputStream()
    {
        
    }
};
//--------------------------
inline bool isEof() {
    if (buf_pos == buf_len) {
        buf_pos = 0, buf_len = fread(buf, 1, buf_size, stdin);
        if (buf_pos == buf_len)
            return 1;
    }
    return 0;
}

inline int getChar() {
    return isEof() ? -1 : buf[buf_pos++];
}

inline int peekChar() {
    return isEof() ? -1 : buf[buf_pos];
}

inline bool seekEof() {
    int c;
    while ((c = peekChar()) != -1 && c <= 32)
        buf_pos++;
    return c == -1;
}

inline int readChar() {
    int c = getChar();
    while (c != -1 && c <= 32)
        c = getChar();
    return c;
}

inline int readUInt() {
    int c = readChar(), x = 0;
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return x;
}

template <class T>
inline T readInt() {
    int s = 1, c = readChar();
    T x = 0;
    if (c == '-')
        s = -1, c = getChar();
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return s == 1 ? x : -x;
}

inline double readDouble() {
    int s = 1, c = readChar();
    double x = 0;
    if (c == '-')
        s = -1, c = getChar();
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    if (c == '.') {
        c = getChar();
        double coef = 1;
        while ('0' <= c && c <= '9')
            x += (c - '0') * (coef *= 1e-1), c = getChar();
    }
    return s == 1 ? x : -x;
}

inline void readWord( char *s ) {
    int c = readChar();
    while (c > 32)
        *s++ = c, c = getChar();
    *s = 0;
}

inline bool readLine( char *s ) {
    int c = getChar();
    while (c != '\n' && c != -1)
        *s++ = c, c = getChar();
    *s = 0;
    return c != -1;
}
//--------------------------
inline void writeChar( int x ) {
    if (write_pos == buf_size)
        fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
    write_buf[write_pos++] = x;
}

template <class T>
inline void writeInt( T x, char end ) {
    if (x < 0)
        writeChar('-'), x = -x;
    
    char s[24];
    int n = 0;
    while (x || !n)
        s[n++] = '0' + x % 10, x /= 10;
    while (n--)
        writeChar(s[n]);
    if (end)
        writeChar(end);
}

template <class T>
inline void writeUInt( T x, int output_len ) {
    char s[24];
    int n = 0;
    while (x || !n)
        s[n++] = '0' + x % 10, x /= 10;
    while (n < output_len)
        s[n++] = '0';
    while (n--)
        writeChar(s[n]);
}
inline void writeWord( const char *s ) {
    while (*s)
        writeChar(*s++);
}

TOutputStream& operator<< (TOutputStream& larg ,bool rarg)
{
    if(rarg)
        writeChar('1');
    else
        writeChar('0');
    return larg;
}
//TOutputStream& operator<< (TOutputStream& larg ,char rarg)
//{
//    writeChar(rarg);
//    return larg;
//}
TOutputStream& operator<< (TOutputStream& larg ,int rarg)
{
    writeInt(rarg);
    return larg;
}
TOutputStream& operator<< (TOutputStream& larg ,const char* rarg)
{
    writeWord(rarg);
    return larg;
}
TOutputStream& operator<< (TOutputStream& larg ,long long rarg)
{
    writeInt(rarg);
    return larg;
}
TOutputStream& operator<< (TOutputStream& larg ,unsigned long long rarg)
{
    writeUInt(rarg);
    return larg;
}
TOutputStream& operator<< (TOutputStream& larg ,unsigned int rarg)
{
    writeUInt(rarg);
    return larg;
}
TOutputStream& operator<< (TOutputStream& larg , const std::string rarg)
{
    writeWord(rarg.c_str());
    return larg;
}
//--------------------------------
InputStream& operator>> (InputStream& larg ,char &rarg)
{
    rarg = readChar();
    return larg;
}
InputStream& operator>> (InputStream& larg ,int &rarg)
{
    rarg = readInt();
    return larg;
}
// ??
InputStream& operator>> (InputStream& larg , char* rarg)
{
    readWord(rarg);
    // rarg = copy;
    return larg;
}
InputStream& operator>> (InputStream& larg , long long &rarg)
{
    rarg = readInt();
    return larg;
}
InputStream& operator>> (InputStream& larg ,unsigned long long &rarg)
{
    rarg = readUInt();
    return larg;
}
InputStream& operator>> (InputStream& larg ,unsigned int &rarg)
{
    rarg = readUInt();
    return larg;
}

//-----------------------
const int MAXN = 10002;
char p[MAXN];
char s[MAXN];
bool d[MAXN];
bool t[MAXN];
std:: string x;
int main()
{
    
    //freopen("patterns.out", "w", stdout);
    InputStream a;
    a >> p;
    x = p;
    TOutputStream b;
    char *copy;
    //    while (*copy != 0) {
    //        b << *copy;
    //        copy++;
    //    }
    b << x;
}
