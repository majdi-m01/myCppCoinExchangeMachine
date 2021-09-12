#include <iostream>
#include <vector>
using namespace std;

const unsigned int numberOfCurrencies = 4;

class Currency{
private:
    int numberOfDenominations;
    vector <float> denominations;
public:
    Currency(int numberOfDenominations, const vector <float>& denominations): numberOfDenominations(numberOfDenominations), denominations(denominations)
    {}
    const vector <float>& get_denominations(){
        return denominations;
    }
};

class MoneyMachine{
private:
    Currency* CurrenciesAvailable[numberOfCurrencies];
public:
    MoneyMachine(Currency* CurrenciesAvailable[numberOfCurrencies]){
        for(unsigned int i=0; i<numberOfCurrencies; ++i){
            this->CurrenciesAvailable[i] = CurrenciesAvailable[i];
        }
    }
    float returnMinimumDenominations(int chosenCurrency, float givenAmount){
        return returnMinimumCoins(CurrenciesAvailable[chosenCurrency]->get_denominations(), givenAmount, 0);
    }
    float returnMinimumCoins(vector<float> denominations, float givenAmount, float minimum ){
        float tempMin = minimum;
        for(int i = denominations.size()-1; i>=0; i--){
            if(givenAmount>= denominations[i]){
                givenAmount = givenAmount - denominations[i];
                cout << denominations[i] <<endl;
                tempMin = returnMinimumCoins(denominations, givenAmount, tempMin);
                return ++tempMin;
            }
        }
        return tempMin;
    }
};

int main()
{
    vector<float> euroCoins{0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2};
    vector<float> dollarCoins{0.01, 0.05, 0.1, 0.25, 0.5, 1};
    vector<float> britishCoins{0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2};
    vector<float> tunisianCoins{0.001, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5};
    Currency* euro = new Currency(8, euroCoins);
    Currency* dollar = new Currency(6, dollarCoins);
    Currency* pound = new Currency(8, britishCoins);
    Currency* tunisianDinar = new Currency(11, tunisianCoins);
    Currency* currenciesUsed[numberOfCurrencies] = {euro, dollar, pound, tunisianDinar};
    MoneyMachine* moneyExchangeMachine = new MoneyMachine(currenciesUsed);
    float moneyInput;
    char answer;
    unsigned int chosenCurrency=0;

    cout << "SIMPLIFIED SIMULATION OF A MONEY EXCHANGE MACHINE" << endl
         << "========================================" << endl << endl;

    do{
        cout << "----------------------------------------" << endl
             << "MENU"                                     << endl
             << "A end of simulation"                      << endl
             << "B choose Currency (default: Euro)"        << endl
             << "C exchange desired amount"                << endl
             << "your choice: ";
        cin >> answer;
        switch(answer){
        case 'A':
            break;
        case 'B':
            cout << "Please choose a Currency: " << endl;
            cout << "0. Euro" << endl << "1. U.S. Dollar" << endl << "2. British Pound" << endl << "3. Tunisian Dianr"<< endl;
            cin >> chosenCurrency;
            if(cin.fail()){
                cin.clear(); cin.ignore();
                cerr << "Please enter correct input." << endl;
            }
            if(chosenCurrency == 0 || chosenCurrency == 1 || chosenCurrency == 2 || chosenCurrency == 3)
                break;
            else{
                chosenCurrency=0;
                cerr << "Please enter correct input." <<endl;
                break;
            }
        case 'C':
            cout << "Please enter desired amount: " << endl;
            cin >> moneyInput;
            if(cin.fail()){
                cin.clear(); cin.ignore();
                cerr << "Please enter correct input." << endl;
                break;
            }
            cout<< "For the given amount " << moneyInput << ", the given coins";
            if(chosenCurrency == 0)
                cout<<" in Euro currency are " <<endl;
            if(chosenCurrency == 1)
                cout<<" in U.S. Dollar currency are "<<endl;
            if(chosenCurrency == 2)
                cout<<" in British Pound currency are "<<endl;
            if(chosenCurrency == 3)
                cout<<" in Tunisian Dinnar currency are "<<endl;
            cout << moneyExchangeMachine->returnMinimumDenominations(chosenCurrency, moneyInput) << " (sum of coins)"<<endl;
            break;
        default:
            cerr << "Please enter correct input." <<endl;
            break;
        }
    } while(answer != 'A');
    return 0;
}