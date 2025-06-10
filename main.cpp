#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

const char uzorakX[9] = {
    'X', ' ', 'X',
    ' ', 'X', ' ',
    'X', ' ', 'X'};

const char uzorakO[9] = {
    'O', 'O', 'O',
    'O', ' ', 'O',
    'O', 'O', 'O'};

void prikaziLeaderboard();
bool ucitajPlocu(char ploca[3][3][9], char velikePloce[3][3],
                 string &igrac1, string &igrac2,
                 const string &filename = "ploca.bin");

int prikaziIzbornik(string &igrac1, string &igrac2, char ploca[3][3][9], char velikePloce[3][3])
{

    cout << "\033[1;36m __   __  ___      _______  ___   __   __  _______  _______  _______    _______  ___   _______    _______  _______  _______    _______  _______  _______" << endl;
    cout << "|  | |  ||   |    |       ||   | |  |_|  ||   _   ||       ||       |  |       ||   | |       |  |       ||   _   ||       |  |       ||       ||       |" << endl;
    cout << "|  | |  ||   |    |_     _||   | |       ||  |_|  ||_     _||    ___|  |_     _||   | |       |  |_     _||  |_|  ||       |  |_     _||   _   ||    ___|" << endl;
    cout << "|  |_|  ||   |      |   |  |   | |       ||       |  |   |  |   |___     |   |  |   | |       |    |   |  |       ||       |    |   |  |  | |  ||   |___ " << endl;
    cout << "|       ||   |___   |   |  |   | |       ||       |  |   |  |    ___|    |   |  |   | |      _|    |   |  |       ||      _|    |   |  |  |_|  ||    ___|" << endl;
    cout << "|       ||       |  |   |  |   | | ||_|| ||   _   |  |   |  |   |___     |   |  |   | |     |_     |   |  |   _   ||     |_     |   |  |       ||   |___ " << endl;
    cout << "|_______||_______|  |___|  |___| |_|   |_||__| |__|  |___|  |_______|    |___|  |___| |_______|    |___|  |__| |__||_______|    |___|  |_______||_______|" << endl;
    cout << "\033[0m" << endl;
    int izbor;
    while (true)
    {
        cout << "\033[31mPritisni 1 za \033[1mpočetak igre\033[0m\033[31m\033[0m" << endl;
        cout << "\033[32mPritisni 2 za \033[1mpravila\033[0m\033[32m\033[0m" << endl;
        cout << "\033[34mPritisni 3 za \033[1mleaderboard\033[0m\033[34m\033[0m" << endl;
        cout << "\033[36mPritisni 4 za \033[1mučitavanje igre\033[0m\033[36m\033[0m" << endl;
        cout << "\033[30mPritisni 5 za izlaz\033[0m" << endl;
        cout << "Unesi svoj izbor: ";
        cin >> izbor;
        cout << endl;
        if (izbor == 1)
        {
            cout << "Unesi ime prvog igraca (X): ";
            cin.ignore();
            getline(cin, igrac1);
            cout << "Unesi ime drugog igraca (O): ";
            getline(cin, igrac2);
            return 1;
        }
        else if (izbor == 2)
        {
            cout << "\033[32m                         ===PRAVILA IGRE===\033[0m" << endl
                 << endl;
            cout << "Ploča se sastoji od 9 manjih 3x3 ploča, raspoređenih u veliku 3x3 mrežu" << endl;
            cout << "Svaki mali kvadrat na velikoj ploči je zapravo zasebna igra Križića-kružića." << endl;
            cout << "Igrači naizmjenično stavljaju svoje znakove (X ili O)" << endl;
            cout << "Odabirom malog polja određuje se polje u velikoj mreži u koje protivnik mora igrati." << endl;
            cout << "Ako je ploča u koju je igrač poslan već dovršena protivnik može igrati bilo gdje." << endl;
            cout << "Igra završava kada jedan igrač spoji tri polja u velikoj mreži ili kada dođe do neriješenog ishoda." << endl
                 << endl;
            cout << "Pritisni Enter za povratak na izbornik." << endl;
            cin.ignore();
            cin.get();
        }
        else if (izbor == 3)
        {
            prikaziLeaderboard();
        }
        else if (izbor == 4)
        {
            if (ucitajPlocu(ploca, velikePloce, igrac1, igrac2))
            {
                cout << "Igra je uspješno učitana. Nastavi igru!" << endl;
                return 2;
            }
            else
            {
                cout << "Nije moguće učitati igru." << endl;
            }
        }
        else if (izbor == 5)
        {
            cout << "Izlaz iz igre. Hvala na igranju!" << endl;
            return 0;
        }
        else
        {
            cout << "Nepoznat izbor. Pokušaj ponovno." << endl;
        }
    }
}

void ispisiPlocu(char ploca[3][3][9], char velikePloce[3][3])
{
    cout << "       A  B  C    D  E  F    G  H  I" << endl;
    for (int velikiRed = 0; velikiRed < 3; ++velikiRed)
    {
        for (int maliRed = 0; maliRed < 3; ++maliRed)
        {
            cout << "  " << (velikiRed * 3 + maliRed + 1) << "   ";
            for (int velikiStupac = 0; velikiStupac < 3; ++velikiStupac)
            {
                // Ako je mala ploča osvojena, koristi uzorak
                const char *uzorak = nullptr;
                if (velikePloce[velikiRed][velikiStupac] == 'X')
                    uzorak = uzorakX;
                else if (velikePloce[velikiRed][velikiStupac] == 'O')
                    uzorak = uzorakO;

                int pocetakIdx = maliRed * 3;
                for (int maliStupac = 0; maliStupac < 3; ++maliStupac)
                {
                    char znak;
                    if (uzorak)
                        znak = uzorak[pocetakIdx + maliStupac];
                    else
                        znak = ploca[velikiRed][velikiStupac][pocetakIdx + maliStupac];

                    cout << " ";
                    if (znak == 'X')
                        cout << "\033[34mX\033[0m ";
                    else if (znak == 'O')
                        cout << "\033[31mO\033[0m ";
                    else
                        cout << ". ";
                }
                if (velikiStupac < 2)
                    cout << "||";
            }
            cout << endl;
        }
        if (velikiRed < 2)
            cout << "      ===============================" << endl;
    }
}

char provjeriPobjeduMalaPloca(char polje[9])
{
    int pobjednickeKombinacije[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, 
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8}, 
        {0, 4, 8},
        {2, 4, 6} 
    };
    for (int i = 0; i < 8; ++i)
    {
        char prvi = polje[pobjednickeKombinacije[i][0]];
        if (prvi != '.' &&
            prvi == polje[pobjednickeKombinacije[i][1]] &&
            prvi == polje[pobjednickeKombinacije[i][2]])
            return prvi;
    }
    return '.';
}
-
char provjeriPobjeduVelikaPloca(char velikePloce[3][3])
{
    // Redovi i stupci
    for (int i = 0; i < 3; ++i)
    {
        if (velikePloce[i][0] != '.' && velikePloce[i][0] == velikePloce[i][1] && velikePloce[i][1] == velikePloce[i][2])
            return velikePloce[i][0];
        if (velikePloce[0][i] != '.' && velikePloce[0][i] == velikePloce[1][i] && velikePloce[1][i] == velikePloce[2][i])
            return velikePloce[0][i];
    }
    // Dijagonale
    if (velikePloce[0][0] != '.' && velikePloce[0][0] == velikePloce[1][1] && velikePloce[1][1] == velikePloce[2][2])
        return velikePloce[0][0];
    if (velikePloce[0][2] != '.' && velikePloce[0][2] == velikePloce[1][1] && velikePloce[1][1] == velikePloce[2][0])
        return velikePloce[0][2];
    return '.';
}

void spremiLeaderboard(const string &pobjednik)
{
    ofstream dat("leaderboard.txt", ios::app);
    if (dat.is_open())
    {
        dat << pobjednik << endl;
        dat.close();
    }
}

void prikaziLeaderboard()
{
    ifstream dat("leaderboard.txt");
    cout << "=== Leaderboard ===" << endl;
    if (!dat.is_open())
    {
        cout << "Nema rezultata." << endl;
        return;
    }
    map<string, int> pobjede;
    string linija;
    while (getline(dat, linija))
    {
        if (!linija.empty())
            pobjede[linija]++;
    }
    dat.close();

    vector<pair<string, int>> lista(pobjede.begin(), pobjede.end());
    sort(lista.begin(), lista.end(), [](const auto &a, const auto &b)
         { return b.second < a.second; });

    int broj = 1;
    for (const auto &par : lista)
    {
        cout << broj++ << ". " << par.first << " - " << par.second << " pobjeda" << endl;
    }
    cout << "Pritisni Enter za povratak na izbornik." << endl;
    cin.ignore();
    cin.get();
}

void spremiPlocu(const char ploca[3][3][9], const char velikePloce[3][3],
                 const string &igrac1, const string &igrac2,
                 const string &filename = "ploca.bin")
{
    ofstream dat(filename, ios::binary);
    if (dat.is_open())
    {
        size_t len1 = igrac1.size();
        size_t len2 = igrac2.size();
        dat.write(reinterpret_cast<const char *>(&len1), sizeof(len1));
        dat.write(igrac1.c_str(), len1);
        dat.write(reinterpret_cast<const char *>(&len2), sizeof(len2));
        dat.write(igrac2.c_str(), len2);
        dat.write(reinterpret_cast<const char *>(ploca), sizeof(char) * 3 * 3 * 9);
        dat.write(reinterpret_cast<const char *>(velikePloce), sizeof(char) * 3 * 3);
        dat.close();
        cout << "Stanje ploče i imena spremljeni" << endl;
    }
}

bool ucitajPlocu(char ploca[3][3][9], char velikePloce[3][3],
                 string &igrac1, string &igrac2,
                 const string &filename)
{
    ifstream dat(filename, ios::binary);
    if (dat.is_open())
    {
        size_t len1, len2;
        dat.read(reinterpret_cast<char *>(&len1), sizeof(len1));
        igrac1.resize(len1);
        dat.read(&igrac1[0], len1);
        dat.read(reinterpret_cast<char *>(&len2), sizeof(len2));
        igrac2.resize(len2);
        dat.read(&igrac2[0], len2);
        dat.read(reinterpret_cast<char *>(ploca), sizeof(char) * 3 * 3 * 9);
        dat.read(reinterpret_cast<char *>(velikePloce), sizeof(char) * 3 * 3);
        dat.close();
        cout << "Stanje ploče i imena učitani" << endl;
        return true;
    }
    else
    {
        cout << "Greška pri učitavanju ploče!" << endl;
        return false;
    }
}

int main()
{
    string igrac1, igrac2;
    char ploca[3][3][9];
    char velikePloce[3][3];
    // Inicijalizacija
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            velikePloce[i][j] = '.';
            for (int k = 0; k < 9; ++k)
                ploca[i][j][k] = '.';
        }

    int pokreni = prikaziIzbornik(igrac1, igrac2, ploca, velikePloce);
    if (pokreni == 0)
        return 0;

    int trenutniIgrac = 0;
    int dozvoljenaMalaPloca = -1;

    while (true)
    {
        ispisiPlocu(ploca, velikePloce);

        cout << (trenutniIgrac == 0 ? igrac1 : igrac2) << " na redu (" << (trenutniIgrac == 0 ? "X" : "O") << ")." << endl;

        if (dozvoljenaMalaPloca == -1)
            cout << "Možeš igrati u bilo kojoj maloj ploči." << endl;
        else
            cout << "Moraš igrati u maloj ploči broj " << dozvoljenaMalaPloca + 1 << "." << endl;

        cout << "Unesi potez (redak 1-9 i stupac A-I), 'save' za spremanje, 'load' za učitavanje, ili -1 za izlaz: ";
        string potez;
        cin >> potez;
        if (potez == "-1")
        {
            cout << "Izlaz iz igre. Hvala na igranju!" << endl;
            return 0;
        }
        if (potez == "save")
        {
            // Spremanje:
            spremiPlocu(ploca, velikePloce, igrac1, igrac2);
            continue;
        }
        if (potez == "load")
        {
            // Učitavanje:
            ucitajPlocu(ploca, velikePloce, igrac1, igrac2);
            ispisiPlocu(ploca, velikePloce);
            continue;
        }

        // Parsiraj unos
        if (potez.length() != 2 ||
            potez[0] < '1' || potez[0] > '9' ||
            (toupper(potez[1]) < 'A' || toupper(potez[1]) > 'I'))
        {
            cout << "Neispravan unos!" << endl;
            continue;
        }
        int red = potez[0] - '1';
        int stupac = toupper(potez[1]) - 'A';

        int velikiRed = red / 3;
        int velikiStupac = stupac / 3;
        int malaRed = red % 3;
        int maliStupac = stupac % 3;
        int idx = malaRed * 3 + maliStupac;

        // Provjeri je li potez u dozvoljenoj ploči
        int trenutnaMalaPloca = velikiRed * 3 + velikiStupac;
        if (dozvoljenaMalaPloca != -1 && trenutnaMalaPloca != dozvoljenaMalaPloca)
        {
            cout << "Moraš igrati u maloj ploči broj " << dozvoljenaMalaPloca + 1 << "!" << endl;
            continue;
        }

        // Provjeri je li mala ploča već osvojena ili neriješena
        if (velikePloce[velikiRed][velikiStupac] != '.')
        {
            cout << "Ta mala ploča je već osvojena ili neriješena, ne možeš igrati u njoj!" << endl;
            continue;
        }

        // Provjeri je li polje slobodno
        if (ploca[velikiRed][velikiStupac][idx] != '.')
        {
            cout << "To polje je već zauzeto!" << endl;
            continue;
        }

        // Upiši potez
        ploca[velikiRed][velikiStupac][idx] = (trenutniIgrac == 0 ? 'X' : 'O');

        // Provjeri pobjedu na maloj ploči
        char pobjednikMale = provjeriPobjeduMalaPloca(ploca[velikiRed][velikiStupac]);
        if (pobjednikMale != '.' && velikePloce[velikiRed][velikiStupac] == '.')
        {
            velikePloce[velikiRed][velikiStupac] = pobjednikMale;
        }

        // Odredi malu ploču za sljedećeg igrača
        dozvoljenaMalaPloca = idx;
        int sljRed = dozvoljenaMalaPloca / 3;
        int sljStupac = dozvoljenaMalaPloca % 3;
        // Ako je ta mala ploča već osvojena ili puna, dozvoli bilo gdje
        bool punaIliOsvojena = (velikePloce[sljRed][sljStupac] != '.');
        if (!punaIliOsvojena)
        {
            bool imaPrazno = false;
            for (int i = 0; i < 9; ++i)
                if (ploca[sljRed][sljStupac][i] == '.')
                    imaPrazno = true;
            if (!imaPrazno)
                punaIliOsvojena = true;
        }
        if (punaIliOsvojena)
            dozvoljenaMalaPloca = -1;

        // Zamijeni igrača
        trenutniIgrac = 1 - trenutniIgrac;

        // Provjeri pobjedu na velikoj ploči
        char pobjednik = provjeriPobjeduVelikaPloca(velikePloce);
        if (pobjednik != '.')
        {
            ispisiPlocu(ploca, velikePloce); // Prvo prikaži ažuriranu ploču
            cout << "\033[1;33mPobjednik je " << (pobjednik == 'X' ? igrac1 : igrac2) << "!\033[0m" << endl;
            spremiLeaderboard(pobjednik == 'X' ? igrac1 : igrac2);
            break;
        }

        // Provjeri neriješeni ishod
        bool nerijeseno = true;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (velikePloce[i][j] == '.')
                    nerijeseno = false;
        if (nerijeseno)
        {
            cout << "Igra je završila neriješeno!" << endl;
            break;
        }
    }

    return 0;
}