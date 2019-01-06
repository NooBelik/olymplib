#include <string>
namespace olymplib {
    namespace {
        template <class T> void writeInt(T x);
        template <class T> void writeUInt(T x);
        void writeChar(int x);
        void writeWord(const char *s);
        void writeDouble(double x, int len = 0);
        static const int buf_size = 4096;
        static int write_pos = 0;
        static char write_buf[buf_size];
        static char buf[buf_size];
        static int buf_len = 0, buf_pos = 0;
        //-- read
        template <class T = int> T readInt();
        double readDouble();
        int readUInt();
        int readChar();
        void readWord(char *s);
        bool readLine(char *s); // do not save '\n'
        bool isEof();
        int peekChar();
        bool seekEof();
        void flush();
        //--------------------------
        bool isEof() {
            if (buf_pos == buf_len) {
                buf_pos = 0, buf_len = fread(buf, 1, buf_size, stdin);
                if (buf_pos == buf_len)
                    return 1;
            }
            return 0;
        }

        int getChar() {
            return isEof() ? -1 : buf[buf_pos++];
        }

        int peekChar() {
            return isEof() ? -1 : buf[buf_pos];
        }

        bool seekEof() {
            int c;
            while ((c = peekChar()) != -1 && c <= 32)
                buf_pos++;
            return c == -1;
        }

        int readChar() {
            int c = getChar();
            while (c != -1 && c <= 32)
                c = getChar();
            return c;
        }

        int readUInt() {
            int c = readChar(), x = 0;
            while ('0' <= c && c <= '9')
                x = x * 10 + c - '0', c = getChar();
            return x;
        }

        template <class T>
        T readInt() {
            int s = 1, c = readChar();
            T x = 0;
            if (c == '-')
                s = -1, c = getChar();
            while ('0' <= c && c <= '9')
                x = x * 10 + c - '0', c = getChar();
            return s == 1 ? x : -x;
        }

        template <class Double = double>
        double readDouble() {
            int s = 1, c = readChar();
            Double x = 0;
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

        void readWord(char *s) {
            int c = readChar();
            while (c > 32)
                *s++ = c, c = getChar();
            *s = 0;
        }

        bool readLine(char *s) {
            int c = getChar();
            while (c != '\n' && c != -1)
                *s++ = c, c = getChar();
            *s = 0;
            return c != -1;
        }

        //only supports fixed precision for now (hence huge values are not allowed)
        void writeDouble(double x, int output_len) {
            if (x < 0)
                writeChar('-'), x = -x;
            int t = (int)x;
            writeUInt(t), x -= t;
            writeChar('.');
            for (int i = output_len - 1; i > 0; i--) {
                x *= 10;
                t = 9 < (int)x ? 9 : x;
                writeChar('0' + t), x -= t;
            }
            x *= 10;
            t = 9 < (int)(x + 0.5) ? 9 : (int)(x + 0.5);
            writeChar('0' + t);
        }

        void writeChar(int x) {
            if (write_pos == buf_size)
                fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
            write_buf[write_pos++] = x;
        }

        template <class T>
        void writeUInt(T x) {
            char s[24];
            int n = 0;
            while (x || !n)
                s[n++] = '0' + x % 10, x /= 10;
            while (n--)
                writeChar(s[n]);
        }

        template<>
        void writeInt<int>(int x) {
            if (x < 0)
                writeChar('-'), x = -x;

            char s[10];
            int n = 0;
            while (x || !n)
                s[n++] = '0' + x % 10, x /= 10;
            while (n--)
                writeChar(s[n]);
        }

        template <class T>
        void writeInt(T x) {
            if (x < 0)
                writeChar('-'), x = -x;

            char s[24];
            int n = 0;
            while (x || !n)
                s[n++] = '0' + x % 10, x /= 10;
            while (n--)
                writeChar(s[n]);
        }

        void writeWord(const char *s) {
            while (*s)
                writeChar(*s++);
        }

        void flush() {
            if (write_pos) {
                fwrite(write_buf, 1, write_pos, stdout), write_pos = 0;
            }
        }
    }

    class TOutputStream
    {
        int precision_;
    public:
        void precision(int new_prec)
        {
            precision_ = new_prec;
        }

        TOutputStream& operator<< (bool arg)
        {
            writeChar('0' + arg);
            return *this;
        }

        TOutputStream& operator<< (char arg)
        {
            writeChar(arg);
            return *this;
        }

        TOutputStream& operator<< (const char* arg)
        {
            writeWord(arg);
            return *this;
        }

        TOutputStream& operator << (float arg) {
            writeDouble(arg, precision_);
            return *this;
        }

        TOutputStream& operator << (double arg) {
            writeDouble(arg, precision_);
            return *this;
        }

        TOutputStream& operator << (long double arg) {
            writeDouble(arg, precision_);
            return *this;
        }

        TOutputStream& operator<< (int arg)
        {
            writeInt(arg);
            return *this;
        }

        TOutputStream& operator<< (long long arg)
        {
            writeInt(arg);
            return *this;
        }

        TOutputStream& operator<< (const std::string& arg)
        {
            writeWord(arg.c_str());
            return *this;
        }

        void flush() {
            olymplib::flush();
        }

        typedef TOutputStream& (*MyStreamManipulator)(TOutputStream&);

        TOutputStream& operator<<(const MyStreamManipulator& manip)
        {
            return manip(*this);
        }

        ~TOutputStream() {
            flush();
        }
    };

    class TInputStream {
    public:
        TInputStream& operator>> (bool& arg)
        {
            arg = readChar() - '0';
            return *this;
        }

        TInputStream& operator>> (char& arg)
        {
            arg = readChar();
            return *this;
        }

        //please know what you're doing.
        TInputStream& operator>> (char*& arg)
        {
            readWord(arg);
            return *this;
        }

        TInputStream& operator >> (float& arg) {
            arg = readDouble<float>();
            return *this;
        }

        TInputStream& operator >> (double& arg) {
            arg = readDouble<double>();
            return *this;
        }

        TInputStream& operator >> (long double& arg) {
            arg = readDouble<long double>();
            return *this;
        }

        TInputStream& operator>> (int& arg)
        {
            arg = readInt<int>();
            return *this;
        }

        TInputStream& operator>> (long long arg)
        {
            arg = readInt<long long>();
            return *this;
        }

        TInputStream& operator>> (std::string& arg)
        {
            arg.clear();
            arg.reserve(16);
            arg.push_back(readChar());
            while (char c = peekChar()) {
                if (c == -1 || c <= 32)
                    break;
                arg.push_back(getChar());
            }
            arg.shrink_to_fit();
            return *this;
        }
    };

    static TInputStream cin;
    static TOutputStream cout;

    static TOutputStream& endl(TOutputStream& stream)
    {
        stream << '\n';
        stream.flush();
        return stream;
    }
}