#ifndef BMS_USERDATA_H
#define BMS_USERDATA_H

const string USER_LIST_FILE = "users.txt";
const string USER_LIST_FILE_TMP = "users_tmp.txt";
const string USER_DIR = "users";
const string HIS_DIR = "history";

class File
{
public:
    static bool exists(string path)
    {
        ofstream file(path + "/.txt");
        if (file.is_open())
        {
            file.close();
            remove((path + "/.txt").c_str());
            return true;
        }
        return false;
    }

    static bool mkdir(string dirname)
    {
        return system(("mkdir " + dirname).c_str()) == 0;
    }

    static string getUserPath(string username, unsigned int accountNo)
    {
        if (!File::exists(USER_DIR))
        {
            if (!File::mkdir(USER_DIR))
            {
                showException("Can't create user directory, getUserPath");
            }
        }

        return USER_DIR + "/" + username + "_" + to_string(accountNo) + ".txt";
    }

    static string getHistoryPath(unsigned int account_no)
    {
        if (!File::exists(HIS_DIR))
        {
            if (!File::mkdir(HIS_DIR))
            {
                showException("Can't create history directory, getHistoryPath");
            }
        }

        return HIS_DIR + "/" + to_string(account_no) + ".txt";
    }
};

class UserList
{
public:
    static void addUserList(string username, unsigned int accountNo)
    {
        ofstream users_file(USER_LIST_FILE, ios::app);
        if (!users_file.is_open())
            showException("Can't open user list file, addUserList");

        users_file << username << " " << accountNo << endl;
        users_file.close();
    }

    static void deleteUserList(string username)
    {
        ifstream file(USER_LIST_FILE);
        if (!file.is_open())
            showException("Can't open user list file, deleteUserList");
        ofstream file_tmp(USER_LIST_FILE_TMP);
        if (!file_tmp.is_open())
        {
            file.close();
            showException("Can't open user list temp file, deleteUserList");
        }

        while (!file.eof())
        {
            string found_username, found_account;
            file >> found_username;
            file >> found_account;

            if (!found_username.empty() && !found_account.empty() && username != found_username)
            {
                file_tmp << found_username << " " << found_account << endl;
            }
        }

        file.close();
        file_tmp.close();

        remove(USER_LIST_FILE.c_str());
        rename(USER_LIST_FILE_TMP.c_str(), USER_LIST_FILE.c_str());
    }
};

class UserData
{
public:
    static void deleteUser(User &user)
    {
        UserList::deleteUserList(user.getUsername());

        string filePath = File::getUserPath(user.getUsername(), user.getAccountNo());
        string newFilePath = File::getUserPath("." + user.getUsername(), user.getAccountNo());
        rename(filePath.c_str(), newFilePath.c_str());
    }

    static void addUser(User &user)
    {
        UserList::addUserList(user.getUsername(), user.getAccountNo());
        saveUserData(user);
    }

    static void updateUser(User &user)
    {
        if (user.usernameChanged())
        {
            string username = user.getUsername();
            string oldUsername = user.getOldUsername();
            unsigned int accountNo = user.getAccountNo();

            UserList::deleteUserList(oldUsername);
            UserList::addUserList(username, accountNo);

            rename(File::getUserPath(oldUsername, accountNo).c_str(),
                   File::getUserPath(username, accountNo).c_str());
        }

        saveUserData(user);
    }

    static void saveUserData(User &user)
    {
        ofstream file(File::getUserPath(user.getUsername(), user.getAccountNo()));
        if (!file.is_open())
            showException("Can't open user data file, saveUserData");

        file << user.getUsername() << endl;
        file << user.getPassword() << endl;
        file << user.getFirstName() << endl;
        file << user.getLastName() << endl;
        file << user.getAccountNo() << endl;
        file << user.getBalance() << endl;

        file.close();
    }

    static User *getUser(string username)
    {
        unsigned int accountNo = getAccountNoByUsername(username);
        string userDataPath = File::getUserPath(username, accountNo);
        if (userDataPath.empty())
            return nullptr;

        ifstream file(userDataPath);
        if (!file.is_open())
            return nullptr;

        string value, password, firstName, lastName;
        double balance;

        getline(file, value); // username
        getline(file, password);
        getline(file, firstName);
        getline(file, lastName);
        getline(file, value);
        getline(file, value); // account no
        balance = _stod(value);

        User *user = new User{
            false,
            username,
            password,
            firstName,
            lastName,
            accountNo,
            balance};
        return user;
    }

    static bool userExists(string username)
    {
        return getAccountNoByUsername(username) != 0;
    }

    static User *getUserByAccNo(unsigned int accountNo)
    {
        ifstream file(USER_LIST_FILE);
        if (file.is_open())
        {
            string found_username, found_account;

            while (!file.eof())
            {
                file >> found_username;
                file >> found_account;

                if (accountNo == stoul(found_account))
                {
                    return getUser(found_username);
                }
            }
            file.close();
        }

        return nullptr;
    }

    static unsigned int getAccountNoByUsername(string username)
    {
        ifstream file(USER_LIST_FILE);
        if (file.is_open())
        {
            string found_username, found_account;

            while (!file.eof())
            {
                file >> found_username;
                file >> found_account;

                if (strcasecmp(username.c_str(), found_username.c_str()) == 0)
                {
                    return stoi(found_account);
                }
            }
            file.close();
        }

        return 0;
    }

    /*
     * Registers account balance history to the file system database
     *
     * `account_from`   - Target account who sends, deposits or withdraws the money
     * `amount`         - The amount of money to be sent, deposited or withdrawn.
     * `account_to`     - If omitted or the same as account_from the action will be
     *                    deposition or withdrawal based on the `deposit` parameter
     *                    otherwise transfer
     */
    static void addHistory(unsigned int account_from, double amount, unsigned int account_to = 0, bool deposit = true)
    {
        time_t timestamp = time(nullptr);

        if (account_to == 0 || account_from == account_to)
        {
            // no receiver
            ofstream file(File::getHistoryPath(account_from), ios::app);
            if (!file.is_open())
                showException("Can't open history file, addHistory");

            if (deposit)
            {
                // deposit
                file << "Deposit " << amount << " " << timestamp << endl;
            }
            else if (amount > 0)
            {
                // withdraw
                file << "Withdraw " << amount << " " << timestamp << endl;
            }

            file.close();
        }
        else
        {
            // transfer
            ofstream sender_file(File::getHistoryPath(account_from), ios::app);
            if (!sender_file.is_open())
                showException("Can't open sender's file, addHistory");

            ofstream receiver_file(File::getHistoryPath(account_to), ios::app);
            if (!receiver_file.is_open())
            {
                sender_file.close();
                showException("Can't open receiver's file, addHistory");
            }

            sender_file << "To " << account_to << " " << amount << " " << timestamp << endl;
            receiver_file << "From " << account_from << " " << amount << " " << timestamp << endl;

            sender_file.close();
            receiver_file.close();
        }
    }

    static string getHistory(unsigned int account_no)
    {
        string history;

        int cnt = 0;
        ifstream file(File::getHistoryPath(account_no));
        if (file.is_open())
        {
            double balance = 0;
            while (!file.eof())
            {
                string type, amount, other_user, timestamp;

                file >> type;
                if (type != "From" && type != "To" && type != "Deposit" && type != "Withdraw")
                    break;

                if (type == "To" || type == "From")
                    file >> other_user; // sender or receiver
                file >> amount;
                file >> timestamp;

                auto timestamp_t = static_cast<time_t>(stol(timestamp));
                timestamp = ctime(&timestamp_t);
                timestamp.pop_back(); // try to remove /n

                history.append(CLI::br_cyan(timestamp) + " ");

                string sign = type == "To" || type == "Withdraw" ? "-" : "+";
                amount.insert(0, sign);
                balance += _stod(amount);

                history.append(sign == "-" ? CLI::red(amount) : CLI::green(amount));
                history.append(" " + CLI::underline(to_string(balance)) + " ");

                if (type == "To" || type == "From")
                {
                    history.append(" Transfer " + type + " " + other_user);
                }
                else
                {
                    history.append(" " + type);
                }
                history.append("\n");
                cnt++;
            }
            file.close();
        }

        if (cnt == 0)
            history = "Your transaction history is empty";
        return history;
    }
};

bool User::userExists(string username)
{
    return UserData::userExists(username);
}

#endif // BMS_USERDATA_H