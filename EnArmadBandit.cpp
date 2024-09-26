

#include <iostream>
#include <limits>
#include <random>


// första funktionen skriver ut välkomstmeddelandet, void används för att det inte returneras ngt "värde"
void DisplayWelcomeMessage() {
    std::cout << "\nWelcome to the game of one-armed bandit! \n"
            "\nRules of the game: \n"
            "\nYou will soon be able to put in money to bet with, 100, 300 or 500 kr. \n"
            "\nYou will then choose a bet. \n"
            "\nThe game will then start and the one-armed bandit will randomize 3 different symbols in 9 fields. \n"
            "\nThe goal of the game is to obtain as many rows, columns and diagonals as possible of the same symbol. \n"
            "\n 1 row = 2 * bet \n"
            "\n 2 row = 3 * bet \n"
            "\n 3 row = 4 * bet \n"
            "\n 4 row = 5 * bet \n"
            "\n 5 row = 7 * bet \n"
            "\n Full = 10 * bet \n"
             "\n\nGet ready.... Set..... GOOOO!!! \n" << std::endl;
}
// funktion som frågar användaren ifall den vill sätta in pengar på kontot
int DoYouWantToPutInMoney(int Saldo, int (*PutInMoney)()) {
    int ContinueInMoney;
    bool ValidInput = false;
    while (!ValidInput) {
        std::cout << "\n\nDo you want to put in money to play with (to your saldo)?";
        std::cout << "\n1 = Yes \n2 = No \n3 = Quit ";
        std::cout << "\nEnter your choice: ";
        std::cin >> ContinueInMoney;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input, please enter a valid choice (1, 2, or 3)." << std::endl;
            ValidInput = false;
        }else if (ContinueInMoney == 1) {
            int InMoney = PutInMoney();
            Saldo += InMoney;
            ValidInput = true;
        } else if (ContinueInMoney == 2 && Saldo <= 0) {
            std::cout << "\nYou currently have no saldo, you have to put in money in order to play one-armed bandit!";
            int InMoney = PutInMoney();
            Saldo += InMoney;
            ValidInput = true;
        } else if (ContinueInMoney == 2) {
            std::cout << "\nYou have chosen to not put in money, therefore the money you have to bet with is: " << Saldo << std::endl;
            ValidInput = true;
        } else if (ContinueInMoney == 3) {
            std::cout << "\nThank you for playing! You've ended up with: " << Saldo << " kr." << std::endl;
            return 0;
        } else {
            std::cout << "\nInvalid input, please enter a valid choice (1, 2, or 3)." << std::endl;
            ValidInput = false;
        }
    }
    return Saldo;
}

// funktionen returnerar hur mycket anv. vill ladda kontot med (100, 300, lr 500)
int PutInMoney(){
    int InputPutInMoney;
    // while loop som körs till giltig input angivits
    while (true) {
        std::cout << "\nPut in money inorder to bet please! (100, 300, 500 kr): ";
        std::cin >> InputPutInMoney;
        //cin.fail() kollar ifall input = ogiltigt och blir isf true
        // om inputen skiljer sig från de godkända värdena 100, 300, 500
        if (std::cin.fail() || (InputPutInMoney != 100 && InputPutInMoney != 300 && InputPutInMoney != 500 )) {
            std::cout << "\nInvalid bet. Try again!" << std::endl;
            // cin.clear() rensar felstatus på cin
            std::cin.clear();
            // cin.ignore() för att bli av med ogiltig input som användaren angav
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // om inputen = giltig bryts loopen
        } else {
            break;
        }
    }
    // returnerar den giltiga inputen dvs 100,300 eller 500 kr
    return InputPutInMoney;
}


// funktion som returnerar användarens insats
int PutInBet(int Saldo) {
    int InputPutInBet;
    while (true) {
        std::cout << "\nPut in your bet please: ";
        std::cin >> InputPutInBet;
        // om inputen är större än det totala saldot eller <= 0 (eller annat än int då)
        if (std::cin.fail() || InputPutInBet > Saldo || InputPutInBet <= 0 ){
            std::cout << "\nInvalid bet. Try again!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    // returnerar den giltiga insatsen
    return InputPutInBet;
}

// funktion för att fylla varje position i 2D array av char
void RandomSymbols(char Grid[3][3]) {
    std::random_device rd;
    std::mt19937 generator(rd());
    // Array med 3 möjliga symboler
    const char ArraySymbols[] = {'A', 'O', 'X'};
    std::discrete_distribution<int> distribution({2, 1, 1});
    // itererr över raderna i spelfältet, där i = index för raden
    for (int i = 0; i < 3; ++i) {
        // itererar över kolumnerna i spelfältet, där j = index för kolumnen
        for (int j = 0; j < 3; ++j) {
            //generara ett random index med symbol
            Grid[i][j] = ArraySymbols[distribution(generator)];
        }
    }
}

// Funktion för att skriva ut den fyllda 2D array
void DisplayGrid(const char Grid[3][3]) {
    std::cout << "\nOne-armed Bandit: \n";
    // loopar igenom varje rad i spelfältet
    for (int i = 0; i < 3; ++i) {
        std::cout << " | ";
        // loopar igenom varje symbol på raden
        for (int j = 0; j < 3; ++j) {
            // skriver ut symbol med |
            std::cout << Grid[i][j] << " | ";
        }
        // skriver ut ny rad efter alla symboler i raden skrivits ut
        std::cout << std::endl;
        std::cout << "  --- --- ---\n";
    }
}

// funktion för beräkna vinst baserat på hur många rader, kolumner, diagonaler som innehåller likadan symbol
int CalculateWinnings(const char Grid[3][3], int InputPutInBet){
    // initierar variabler för att räkna likadana symboler
    int Rows = 0;
    int Columns = 0;
    int Diagonals = 0;
    // loop som går igenom varje rad i spelfältet
    for (int i = 0; i < 3; ++i) {
        // om alla 3 symboler i raden är likadana
        if (Grid[i][0] == Grid[i][1] && Grid[i][1] == Grid[i][2]) {
            ++Rows;
        }
    }
    // loop som går igenom varje kolumn i spelfältet
    for (int column = 0; column < 3; ++column) {
        // om alla 3 symboler i kolumnen är likadana
        if (Grid[0][column] == Grid[1][column] && Grid[1][column] == Grid[2][column]) {
            ++Columns;
        }
    }
    // kollar diagonalerna i spelfältet
    // om alla 3 symboler i diagonalen är likadana (++Diagonals;)
    if (Grid[0][0] == Grid[1][1] && Grid[1][1] == Grid[2][2]) {
        ++Diagonals;
    }
    if (Grid[0][2] == Grid[1][1] && Grid[1][1] == Grid[2][0]) {
        ++Diagonals;
    }
    // alla rader, kolumner och diagonaler som har samma symbol läggs i Total variabeln
    int Total = Rows + Columns + Diagonals;
    int Win = 0;
    // beräknar vinst beroende på Total
    switch (Total) {
        // 1 rad likadan, vinst blir 2 * InputPutInBet
        case 1:
            Win = 2 * InputPutInBet;
        break;
        // 2 rad likadan, vinst blir 3 * InputPutInBet
        case 2:
            Win = 3 * InputPutInBet;
        break;
        //...
        case 3:
            Win = 4 * InputPutInBet;
        break;
        case 4:
            Win = 5 * InputPutInBet;
        break;
        case 5:
            Win = 7 * InputPutInBet;
        break;
        case 6:
            Win = 10 * InputPutInBet;
        break;
        // om ej vinst
        default:
            Win = 0;
        break;
    }
    // returnerar vinsten
    return Win;
}
// Funktion om man vill fortsätta spelet eller ej
int ContinueGame(int saldo) {
    bool Continue = false;
    char InputContinue;
    while(!Continue) {
        std::cout << "\nDo you want to play again? (y/n): ";
        std::cin >> InputContinue;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input, please enter a valid choice (y or n).";
            Continue = false;
        } else if (InputContinue == 'n' || InputContinue == 'N') {
            std::cout << "\nThank you for playing! You've ended up with: " << saldo << " kr.";
            return 0;
        } else if (InputContinue == 'y' || InputContinue == 'Y') {
            Continue = true;
        } else {
            std::cout << "\nInvalid input, please enter a valid choice (y or n).";
            Continue = false;
        }
    }
    return 1;
}


//här kommer huvud-funktionen som kallar på de ovanstående funktionerna
int main() {
    DisplayWelcomeMessage();
    int Saldo = 0;
    while (true) {
        std::cout << "\nYour saldo: " << Saldo;

        int DoYouWant = DoYouWantToPutInMoney(Saldo, PutInMoney);
        if (DoYouWant <= 0) {
            break;
        }
        Saldo = DoYouWant;
        // ber användaren lägga sin insatts
        int InBet = PutInBet(Saldo);
        // Initierar ett 3x3 spelfält med char
        char Grid[3][3];
        // Spelfältet fylls med slumpmässiga symboler
        RandomSymbols(Grid);

        //Spelfältet visas
        DisplayGrid(Grid);
        //beräknar vinsten
        int Win = CalculateWinnings(Grid, InBet) - InBet;
        //baserat på vinst eller förlust uppdateras saldot
        if (Win > 0) {
            Saldo += Win;
            std::cout << "\nYou won: " << Win << std::endl;
        } else if(Win <= 0) {
            Saldo -= InBet;
            std::cout << "\n You lost, therefor you have now lost your bet: " << InBet << std::endl;
        }
        // saldots visas
        std::cout << "\nYour current balance is: " << Saldo << " kr." << std::endl;

        if (Saldo >0) {
            int Continue = ContinueGame(Saldo);
            if (Continue == 0) {
                break;
            }
        }else {
            std::cout << "\nYou have run out of money. Put in more money in order to continue playing the game." << std::endl;
        }
    }
    return 0;
}


