#ifndef BMS_UTILS_H
#define BMS_UTILS_H

double _stod(string s)
{
    try
    {
        double n = stod(s);
        return n;
    }
    catch (exception &ex)
    {
        return 0;
    }
}

double cin_double()
{
    string s;
    cin >> s;
    return _stod(s);
}

unsigned int _stoui(string s)
{
    try
    {
        unsigned int n = stoi(s);
        return n;
    }
    catch (exception &ex)
    {
        return 0;
    }
}

unsigned int cin_uint()
{
    string s;
    cin >> s;
    return _stoui(s);
}

// #source https://en.wikipedia.org/wiki/ANSI_escape_code
const string RESET = "\033[0m";
const string LINE = "\033[4m";

// normal color
const string BLACK = "\033[30m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";

// bright color
const string BR_CYAN = "\033[96m";

// background color
const string BG_WHITE = "\033[47m";

class CLI
{
public:
    static string highlight(string text)
    {
        return BLACK + BG_WHITE + text + RESET;
    }

    static string underline(string text)
    {
        return LINE + text + RESET;
    }

    static string red(string text)
    {
        return RED + text + RESET;
    }

    static string green(string text)
    {
        return GREEN + text + RESET;
    }

    static string br_cyan(string text)
    {
        return BR_CYAN + text + RESET;
    }

    static void moveUp(int lines)
    {
        cout << "\033[" << lines << "A";
    }

    static void clearLine()
    {
        cout << "\033[2K\r";
    }

    static void clearAbove(int lines)
    {
        for (int i = 1; i <= lines; i++)
        {
            moveUp(1);
            clearLine();
        }
    }

    static void clear()
    {
        system("cls");
    }

    static bool ask(string message = "> Press q to go back", char quiteKey = 'q', bool waitForever = false)
    {
        cout << message << endl;

    wait:
        if (getch() == quiteKey)
        {
            return true;
        }

        if (waitForever)
            goto wait;

        return false;
    }

    static void showError(string message)
    {
        CLI::clearAbove(1);
        cerr << CLI::red(message) << endl;
    }

    static void setTitle(string title)
    {
        system(("title " + title).c_str());
    }
};

void showException(string message)
{
    CLI::setTitle("Runtime Exception");
    cout << "Error: " << CLI::red(message) << endl;
    cout << "Something unexpected has happened." << endl
         << ":( Sorry, we're stopping now." << endl
         << "   > Press any key to exit";
    getch();
    exit(911);
}

#endif // BMS_UTILS_H