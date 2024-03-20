#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <ctime>

using namespace std;

#include "Utils.h"
#include "User.h"
#include "UserData.h"
#include "Actions.h"

int main()
{
    CLI::clear(); // also enables ANSI escape codes
    User *user = nullptr;
homePage:
    CLI::setTitle("Welcome to Banking System");
    Action *actions[] = {new Login{}, new Register{}, new About{}, new Exit{}};
    int i = choose(actions, size(actions));
    CLI::clear();
    Action *action = actions[i];
    CLI::setTitle(action->getLabel());
    action->execute(user);

    if (user == nullptr) // not logged in
        goto homePage;

    CLI::clear();
userPage:
    CLI::setTitle("My Account");
    cout << "Welcome " << user->getFirstName() << " " << user->getLastName() << endl;
    cout << "Account No: " << CLI::underline(to_string(user->getAccountNo())) << endl;
    cout << "Balance: " << LINE << user->getBalance() << "ETB" << RESET << endl;

    Action *user_actions[] = {new AccountDetail{}, new EditAccount{}, new Deposit{}, new Withdraw{}, new Transfer{},
                              new TransactionHistory{}, new CloseAccount{}, new Logout{}};
    i = choose(user_actions, size(user_actions));
    CLI::clear();
    Action *user_action = user_actions[i];
    CLI::setTitle(user_action->getLabel());
    user_action->execute(user);

    if (user == nullptr)
        goto homePage;
    else
        goto userPage;
}