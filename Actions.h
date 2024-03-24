#ifndef BMS_ACTIONS_H
#define BMS_ACTIONS_H

class Action
{
public:
    virtual string getLabel() { return ""; }

    virtual void execute(User *&user) {}
};

class Login : public Action
{
    string getLabel() override
    {
        return "Login";
    }

    void execute(User *&user) override
    {
        while (true)
        {
            string username, password;
            cout << "Enter Username: ";
            cout << BR_CYAN;
            cin >> username;
            cout << RESET;
            cout << "Enter Password: ";
            cout << BR_CYAN;
            getline(cin >> ws, password);
            cout << RESET;

            User *foundUser = UserData::getUser(username);
            if (foundUser != nullptr && foundUser->getPassword() == password)
            {
                user = foundUser; // logged in
                return;
            }
            else
            {
                cout << CLI::red("Incorrect username or password") << endl;

                bool quit = CLI::ask();
                CLI::clear();
                if (quit)
                    return;
            }
        }
    }
};

class Register : public Action
{
    string getLabel() override
    {
        return "Register";
    }

    void execute(User *&user) override
    {
        User newUser{true};
        newUser.setUserInfo();
        UserData::addUser(newUser);
        cout << GREEN;
        cout << "   v Account No - " << newUser.getAccountNo() << endl;
        cout << "   v Account Created Successfully " << endl;
        cout << RESET;

        CLI::ask("Make sure you get it.\nYou can now login.\n> Press q to go back.", 'q', true);
        CLI::clear();
    }
};

class About : public Action
{
    string getLabel() override
    {
        return "About";
    }

    void execute(User *&user) override
    {
        cout << CLI::underline(" # Banking System") << endl;
        cout << "   Is a simple file based bank system" << endl
             << "   implementation program based on C++." << endl
             << endl;

        cout << CLI::underline(" # Developers") << endl;
        cout << BR_CYAN;

        string names[] = {
            "Beyene Bekele       UGR/3182/15",
            "Enyew Yirga         UGR/4624/15",
            "Osama Ibrahim       UGR/8819/15",
            "Samuel Mesfin       UGR/5939/15",
            "Yitbarek Daniel     UGR/2620/15",
            "Yonathan Tesfaye    UGR/2549/15",
        };

        for (int i = 0; i < 6; i++)
        {
            cout << "   " << i + 1 << ". " << names[i] << endl;
        }
        cout << RESET << endl;
        cout << CLI::underline(" (c) 2016EC Banking System") << endl << endl;

        CLI::ask("> Press q to confirm", 'q', true);
        CLI::clear();
    }
};

class Exit : public Action
{
    string getLabel() override
    {
        return "Exit";
    }

    void execute(User *&user) override
    {
        cout << "Are you sure to exit the app?" << endl;

        if (CLI::ask("> Press c to confirm", 'c'))
            exit(0);
        CLI::clear();
    }
};

class AccountDetail : public Action
{
    string getLabel() override
    {
        return "Account Detail";
    }

    void execute(User *&user) override
    {
        cout << "v Username: " << CLI::underline(user->getUsername()) << endl;
        cout << "v Name: " << CLI::underline(user->getFirstName() + " " + user->getLastName()) << endl;
        cout << "v Account No - " << CLI::underline(to_string(user->getAccountNo())) << endl;
        cout << "v Balance - " << LINE << user->getBalance() << "ETB" << RESET << endl;

        CLI::ask("> Press q to go back", 'q', true);
        CLI::clear();
    }
};

class EditAccount : public Action
{
    string getLabel() override
    {
        return "Edit Account";
    }

    void execute(User *&user) override
    {
        user->setUserInfo();
        UserData::updateUser(*user);
        cout << GREEN;
        cout << "   v Account Modified Successfully " << endl;
        cout << RESET;

        CLI::ask("> Press q to go back.", 'q', true);
        CLI::clear();
    }
};

class Withdraw : public Action
{
    string getLabel() override
    {
        return "Withdraw Cash";
    }

    void execute(User *&user) override
    {
        while (true)
        {
            cout << "Amount: ";
            double amount = cin_double();

            if (amount <= 0)
            {
                CLI::showError("Invalid amount");
            }
            else if (amount > user->getBalance())
            {
                CLI::showError("Insufficient balance");
            }
            else
            {
                user->setBalance(user->getBalance() - amount);
                UserData::updateUser(*user);
                UserData::addHistory(user->getAccountNo(), amount, 0, false);

                cout << LINE << amount << "ETB" << RESET << " has been withdrawn" << endl;
                cout << "Your current balance is " << LINE << user->getBalance() << "ETB" << RESET << endl;
                getch();
                CLI::clear();
                return;
            }

            bool quit = CLI::ask();
            CLI::clear();
            if (quit)
                return;
        }
    }
};

class Deposit : public Action
{
    string getLabel() override
    {
        return "Deposit Cash";
    }

    void execute(User *&user) override
    {
        while (true)
        {
            cout << "Amount: ";
            double amount = cin_double();

            if (amount <= 0)
            {
                CLI::showError("Invalid amount");
            }
            else if (amount > 5000)
            {
                CLI::showError("Maximum cash deposit reached");
            }
            else
            {
                user->setBalance(user->getBalance() + amount);
                UserData::updateUser(*user);
                UserData::addHistory(user->getAccountNo(), amount, 0);

                cout << LINE << amount << "ETB" << RESET << " has been deposited" << endl;
                cout << "Your current balance is " << LINE << user->getBalance() << "ETB" << RESET << endl;
                getch();
                CLI::clear();
                return;
            }

            bool quit = CLI::ask();
            CLI::clear();
            if (quit)
                return;
        }
    }
};

class Transfer : public Action
{
    string getLabel() override
    {
        return "Transfer Money";
    }

    void execute(User *&user) override
    {
        double amount;
        while (true)
        {
            cout << "Amount: ";
            amount = cin_double();

            if (amount <= 0)
            {
                CLI::showError("Invalid amount");
            }
            else if (amount > user->getBalance())
            {
                CLI::showError("Insufficient balance");
            }
            else
            {
                CLI::clear();
                break; // amount valid
            }

            bool quit = CLI::ask();
            CLI::clear();
            if (quit)
                return;
        }

        while (true)
        {
            cout << "Amount: " << amount << endl;
            cout << "Receiver Account No: ";
            unsigned int accountNo = cin_uint();

            if (accountNo == user->getAccountNo())
            {
                CLI::showError("You can't transfer to yourself.");
            }
            else
            {
                User *receiver = UserData::getUserByAccNo(accountNo);
                if (receiver == nullptr)
                {
                    CLI::showError("Account not found");
                }
                else
                {
                    cout << "Transfer " << LINE << amount << "ETB" << RESET << " to " << receiver->getFirstName() << " "
                         << receiver->getLastName() << " (" << receiver->getAccountNo() << ")" << endl;
                    if (CLI::ask("> Press c to commit the transaction", 'c'))
                    {
                        user->setBalance(user->getBalance() - amount);
                        receiver->setBalance(receiver->getBalance() + amount);
                        UserData::updateUser(*user);
                        UserData::updateUser(*receiver);
                        UserData::addHistory(user->getAccountNo(), amount, receiver->getAccountNo());

                        cout << LINE << amount << "ETB" << RESET << " has been transferred" << endl;
                        cout << "Your current balance is " << LINE << user->getBalance() << "ETB" << RESET << endl;
                    }
                    else
                    {
                        cout << "Transaction canceled.";
                    }
                    getch();
                    CLI::clear();
                    return;
                }
            }

            bool quit = CLI::ask();
            CLI::clear();
            if (quit)
                return;
        }
    }
};

class TransactionHistory : public Action
{
    string getLabel() override
    {
        return "Transaction History";
    }

    void execute(User *&user) override
    {
        cout << UserData::getHistory(user->getAccountNo()) << endl;

        CLI::ask("> Press q to go back", 'q', true);
        CLI::clear();
    }
};

class CloseAccount : public Action
{
    string getLabel() override
    {
        return "Close Account";
    }

    void execute(User *&user) override
    {
        cout << CLI::red("WARNING: This action can't be undone.\nYour account balance will be lost permanently.") << endl;

        if (CLI::ask("> Press c to close your account", 'c'))
            if (CLI::ask("> Press c again to confirm", 'c'))
            {
                UserData::deleteUser(*user);
                user = nullptr;
            }

        CLI::clear();
    }
};

class Logout : public Action
{
    string getLabel() override
    {
        return "Logout";
    }

    void execute(User *&user) override
    {
        cout << "Are you sure to logout?" << endl;

        bool logout = CLI::ask("Press c to confirm", 'c');
        CLI::clear();
        if (logout)
            user = nullptr;
    }
};

//  Arrows
//     72
// 75 -32 77
//     80
// getch key-codes
const int ENTER = 13;
const int ARROW = -32;
const int UP = 72;
const int DOWN = 80;

int choose(Action **actions, int len)
{
    int choice = 0;
    bool restarted = false;
restart:
    if (restarted)
        CLI::clearAbove(len);
    else
        restarted = true;

    for (int i = 0; i < len; ++i)
    {
        if (choice == i)
        {
            cout << i + 1 << ". " << CLI::highlight((*actions[i]).getLabel()) << endl;
            continue;
        }
        cout << i + 1 << ". " << (*actions[i]).getLabel() << endl;
    }

    bool arrow = false;
    while (true)
    {
        char ch = getch(); // wait key

        if (arrow) // arrow keys
        {
            arrow = false; // clear
            if (int(ch) == UP)
            {
                if (choice > 0)
                {
                    choice--;
                    goto restart;
                }
            }
            else if (int(ch) == DOWN)
            {
                if (choice < len - 1)
                {
                    choice++;
                    goto restart;
                }
            }
            continue;
        }

        if ((int)ch == ARROW) // next arrow key will come
        {
            arrow = true;
            continue;
        }

        if (ch >= '1' && ch <= '1' + len - 1)
        {
            choice = ch - '1';
            return choice;
        }

        // other keys
        if (int(ch) == ENTER)
        {
            return choice;
        }
    }
}

#endif // BMS_ACTIONS_H