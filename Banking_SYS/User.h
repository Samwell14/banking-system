#ifndef BMS_USER_H
#define BMS_USER_H

class User
{
protected:
    string username;
    string oldUsername;
    string password;
    string firstName;
    string lastName;
    unsigned int accountNo;
    double balance;
    bool isNewUser;

public:
    User(
        bool _isNewUser,
        string _username = "",
        string _password = "",
        string _firstName = "",
        string _lastName = "",
        unsigned int _accountNo = time(nullptr),
        double _balance = 0)
    {
        isNewUser = _isNewUser;
        oldUsername = _username;
        username = _username;
        password = _password;
        firstName = _firstName;
        lastName = _lastName;
        accountNo = _accountNo;
        balance = _balance;
    }

    static bool userExists(string);

    bool usernameChanged()
    {
        return strcasecmp(oldUsername.c_str(), username.c_str()) != 0;
    }

    string getUsername()
    {
        return username;
    };

    string getOldUsername()
    {
        return oldUsername;
    };

    string getPassword()
    {
        return password;
    };

    string getFirstName()
    {
        return firstName;
    };

    string getLastName()
    {
        return lastName;
    };

    unsigned int getAccountNo()
    {
        return accountNo;
    };

    double getBalance()
    {
        return balance;
    };

    void setUsername()
    {
    ask_user:
        string value;
        cout << "Username: ";
        cout << BR_CYAN;
        cin >> value;
        cout << RESET;

        if (strcasecmp(username.c_str(), value.c_str()) == 0)
            return;

        if (userExists(value) != 0)
        {
            CLI::showError("Username already taken");
            goto ask_user;
        }

        if (value.length() < 6)
        {
            CLI::showError("Username too short");
            goto ask_user;
        }

        for (char c : value)
        {
            if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9'))
            {
                CLI::showError("Username can't contain special characters");
                goto ask_user;
            }
        }

        username = value;
        if (isNewUser)
            oldUsername = value;
    }

    void setPassword()
    {
    ask_user:
        string value;
        cout << "Password: ";
        cout << BR_CYAN;
        getline(cin >> ws, value);
        cout << RESET;

        if (value.length() < 8)
        {
            CLI::showError("Password too short");
            goto ask_user;
        }

        password = value;
    }

    void setFirstName()
    {
    ask_user:
        string value;
        cout << "Firstname: ";
        cout << BR_CYAN;
        cin >> value;
        cout << RESET;

        if (value.length() < 3)
        {
            CLI::showError("Firstname too short");
            goto ask_user;
        }

        for (char c : value)
        {
            if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
            {
                CLI::showError("Firstname should contain only letters");
                goto ask_user;
            }
        }

        firstName = value;
    }

    void setLastName()
    {
    ask_user:
        string value;
        cout << "Lastname: ";
        cout << BR_CYAN;
        cin >> value;
        cout << RESET;

        if (value.length() < 3)
        {
            CLI::showError("Lastname too short");
            goto ask_user;
        }

        for (char c : value)
        {
            if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
            {
                CLI::showError("Lastname should contain only letters");
                goto ask_user;
            }
        }

        lastName = value;
    }

    void setBalance(double value)
    {
        balance = value;
    }

    void setUserInfo()
    {
        setUsername();
        CLI::clear();
        cout << CLI::green("v Username: " + username) << endl;

        setPassword();
        CLI::clear();
        cout << GREEN;
        cout << "v Username: " << username << endl;
        cout << "v Password: ******" << endl;
        cout << RESET;

        setFirstName();
        CLI::clear();
        cout << GREEN;
        cout << "v Username: " << username << endl;
        cout << "v Password: ******" << endl;
        cout << "v Firstname: " << firstName << endl;
        cout << RESET;

        setLastName();
        CLI::clear();
        cout << GREEN;
        cout << "v Username: " << username << endl;
        cout << "v Password: ******" << endl;
        cout << "v Name: " << firstName << " " << lastName << endl;
        cout << RESET;
    }
};

#endif // BMS_USER_H