//joel rissanen
//hotelliteht‰v‰
//4 tai 5 pistett‰
//tein ylim‰‰r‰ist‰ tiedostoon tallentamista ja lukemista
//tallenttaa tekstitiedostoon tiedot ja haluttaessa voi uudelleen generoida huoneiden m‰‰r‰n
//valikko voisi olla ehk‰ v‰h‰n k‰ytt‰j‰yst‰v‰llisempi ja koodista alkoi tulla ep‰selv‰‰
//mielest‰ni on kuitenkin parempi ett‰ v‰‰r‰ syˆte palauttaa takaisin alkuun vaikka se on v‰h‰n kˆmpelˆ ei tarvitse tehd‰ enemp‰‰ vaihtoehto valikkoon palaamisesta


#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // pilkut
#include <random>   // mersenne twister
#include <fstream>  // tiedoston k‰sittelyyn
#include <sstream>  // stringstream

using namespace std;


struct Varaus {
    int varausNumero;
    string varaajanNimi;
    string huoneTyyppi;
    int oidenMaara;
    double loppuSumma;
};

// aliohjelma randomluvuille k‰ytt‰m‰ll‰ mersenne twisteri‰ <random> kirjastosta
int randomluku(int alaraja, int ylaraja) {
    static random_device rd;
    static mt19937 mt(rd());
    uniform_int_distribution<int> dist(alaraja, ylaraja);
    return dist(mt);
}

int arvoHuoneidenMaara() {
    return randomluku(20, 150) * 2; //tulee aina parillinen luku ei tarvitse tehd‰ tarkistusta ja uutta arpomista
}

//ˆiden m‰‰r‰*huonetyyppi*alennus
double laskeLoppuSumma(int oidenMaara, string huoneTyyppi, int alennus) {
    double hinta;
    if (huoneTyyppi == "1") {
        hinta = 100;
    }
    else {
        hinta = 150;
    }
    double summa = oidenMaara * hinta;
    return summa - (summa * alennus / 100.0);
}

//0 10 tai 20 alennus
int arvoAlennus() {
    int arpa = randomluku(0, 2);
    switch (arpa)
    {
    case 0:
        return 0;
    case 1:
        return 10;
    case 2:
        return 20;  //return arvot ovat alennusm‰‰r‰ 0-20%
    default:
        return 0;
    }
}

int arvoVarausNumero() {
    return randomluku(10000, 99999);
}

// Funktio huoneiden m‰‰r‰n lukemiseen tiedostosta
int lueHuoneidenMaara() {
    ifstream tiedosto("huoneiden_maara.txt");  // avaa tiedosto lukemiseen
    int huoneidenMaara = 0;
    if (tiedosto) {
        tiedosto >> huoneidenMaara;  // lue huoneiden m‰‰r‰
    }
    tiedosto.close();
    return huoneidenMaara;
}

// Funktio huoneiden m‰‰r‰n tallentamiseen tiedostoon
void tallennaHuoneidenMaara(int huoneidenMaara) {
    ofstream tiedosto("huoneiden_maara.txt");  // avaa tiedosto kirjoittamiseen
    if (tiedosto) {
        tiedosto << huoneidenMaara;  // kirjoittaa huoneiden m‰‰r‰n tiedostoon jotta jatkossa voi lukea tiedostosta ja n‰in ohjelman voi sulkea ja huoneidenm‰‰r‰ s‰ilyy
    }                                   // olisi voinut ottaa myˆs varaukset.txt tiedostosta suoraan
    tiedosto.close();
}

//k‰ytt‰j‰n antamalle arvolle aliohjelma
int huoneidenMaaraManuaalinen() {
    int huoneidenMaara;
    while (true) {
        cout << "Anna huoneiden m‰‰r‰ (40-300): ";
        cin >> huoneidenMaara;

        // 40 ja 300 v‰li syˆte
        if (!cin.fail() && huoneidenMaara >= 40 && huoneidenMaara <= 300) {
            break;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Virheellinen syˆte. Yrit‰ uudelleen." << endl;
    }
    return huoneidenMaara;
}

//huoneenvarausaliohjelma
void varaaHuone(vector<Varaus>& varaukset, int& yhdenHengenJaljella, int& kahdenHengenJaljella) {
    string huoneTyyppi;
    int oidenMaara;
    string varaajanNimi;

    cout << "\nValitse huonetyyppi:\n[1] YHDEN hengen huone\n";
    cout << "[2] KAHDEN hengen huone\n";
    cout << "Valitse toiminto: (1-2) ";
    cin >> huoneTyyppi;

    if (cin.fail() || huoneTyyppi != "1" && huoneTyyppi != "2") {
        cin.clear();  // Palauttaa cin:n normaaliin tilaan
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Virheellinen valinta. Yrit‰ uudelleen." << endl;
        return;
    }

    cout << "Anna ˆiden m‰‰r‰:(yksi tai enemm‰n) ";
    cin >> oidenMaara;
    if (cin.fail() || oidenMaara <= 0) {
        cin.clear();  // Palauttaa cin:n normaaliin tilaan
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Virheellinen m‰‰r‰. Yrit‰ uudelleen." << endl;
        return;
    }

    cout << "Anna varaajan nimi:";
    cin.ignore();
    getline(cin, varaajanNimi);


    // Jatketaan varausta, jos huoneita on saatavilla
    if (huoneTyyppi == "1" && yhdenHengenJaljella >= 1) {
        yhdenHengenJaljella -= oidenMaara; // v‰hennett‰‰n saatavilla olevista huoneista yksi jos arvo yli 1
    }
    else if (huoneTyyppi == "2" && kahdenHengenJaljella >= 1) {
        kahdenHengenJaljella -= oidenMaara;
    }
    else {
        cout << "Huoneita ei ole tarpeeksi saatavilla." << endl;
        return;
    }

    int alennus = arvoAlennus();
    double loppuSumma = laskeLoppuSumma(oidenMaara, huoneTyyppi, alennus);
    int varausNumero = arvoVarausNumero();

    varaukset.push_back({ varausNumero, varaajanNimi, huoneTyyppi, oidenMaara, loppuSumma }); // pushback lis‰‰ uuden varauksen

    cout << "Varaus onnistui! Varausnumero: " << varausNumero << endl;
    cout << "Loppusumma (alennus " << alennus << "%): " << fixed << setprecision(2) << loppuSumma << " euroa" << endl;
}

void tarkasteleVarauksia(const vector<Varaus>& varaukset) {
    string varaussyote;
    cout << "Anna varaajan nimi tai varausnumero: ";
    cin >> varaussyote;

    bool sama = false;

    // tarkistetaan, onko syˆte varaajan nimen tai varausnumeron kanssa
    for (const auto& varaus : varaukset) {
        if (varaussyote == varaus.varaajanNimi || varaussyote == to_string(varaus.varausNumero)) {
            // jos nimi tai numero vastaa syˆtteen tulostus
            cout << "Varausnumero: " << varaus.varausNumero << ", Nimi: " << varaus.varaajanNimi
                << ", Huone: " << varaus.huoneTyyppi << ", Yˆt: " << varaus.oidenMaara
                << ", Loppusumma: " << varaus.loppuSumma << " euroa" << endl;
            sama = true;
        }
    }

    // ei varausta
    if (!sama) {
        cout << "Varausta ei lˆytynyt." << endl;
    }
}

void tallennaVarauksetTiedostoon(const vector<Varaus>& varaukset) {
    ofstream tiedosto("varaukset.txt");  // Avaa tiedoston kirjoittamista varten
    if (tiedosto) {
        for (const auto& varaus : varaukset) {
            tiedosto << varaus.varausNumero << ","
                << varaus.varaajanNimi << ","
                << varaus.huoneTyyppi << ","
                << varaus.oidenMaara << ","
                << fixed << setprecision(2) << varaus.loppuSumma << endl;
        }
    }
    else {
        cout << "Virhe kirjoitettaessa tiedostoon!" << endl;
    }
    tiedosto.close();
}

void lueVarauksetTiedostosta(vector<Varaus>& varaukset) {
    ifstream tiedosto("varaukset.txt");  // Avaa tiedoston lukemista varten
    if (tiedosto) {
        string rivi;
        while (getline(tiedosto, rivi)) {  // Lue rivi tiedostosta
            stringstream ss(rivi);  // Luodaan stringstream
            Varaus varaus;
            string loppuSummaStr;


            string varausNumeroStr;
            getline(ss, varausNumeroStr, ','); //erotuspilkulla
            varaus.varausNumero = stoi(varausNumeroStr);  // muunot integeriksi

            getline(ss, varaus.varaajanNimi, ',');
            getline(ss, varaus.huoneTyyppi, ',');

            string oidenMaaraStr;
            getline(ss, oidenMaaraStr, ',');
            varaus.oidenMaara = stoi(oidenMaaraStr);  // muunto integeriksi

            getline(ss, loppuSummaStr);  // Loppusumma
            varaus.loppuSumma = stod(loppuSummaStr);  // muunto doubleksi

            varaukset.push_back(varaus);
        }
    }
    tiedosto.close();
}

////void pyyhiTiedosto() {
//    // Tiedoston nimi
//    string tiedostonNimi = "varaukset.txt";
//
//    // Avaa tiedosto kirjoitustilassa ja tyhjent‰‰ sen
//    ofstream tiedosto(tiedostonNimi, ios::trunc);
//
//    // Sulje tiedosto
//    tiedosto.close();
//    
//}

void paivitaHuoneidenMaara(int huoneidenMaara, int& yhdenVaratut, int& kahdenVaratut) {
    ifstream file("varaukset.txt");

    yhdenVaratut = 0;       // molemmat arvot nollasta koska aluksi ei varauksia
    kahdenVaratut = 0;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        int sarake = 0;
        int kolmasnumero = 0;

        while (getline(ss, temp, ',')) {
            sarake++;
            if (sarake == 3) { // kolmas rivi eli huonetyyppi 1hh tai 2hh
                kolmasnumero = stoi(temp); //sarakkeen arvo kokonaisluvuksi
                break;
            }
        }

        // jos kolmas numero 1 eli 1hh lis‰t‰‰n 1varattuihin, jos 2 eli 2hh list‰‰n 2varattuihin
        if (kolmasnumero == 1) {
            yhdenVaratut++;
        }
        else if (kolmasnumero == 2) {
            kahdenVaratut++;
        }
    }

    file.close();
}




int main() {
    setlocale(LC_ALL, "fi_FI");

    // varaukset tiedostosta
    vector<Varaus> varaukset;


    // Huoneiden m‰‰r‰ k‰sittely kuten 
    int huoneidenMaara = lueHuoneidenMaara();
    while (true) {
        cout << "[1] Ohjelmaan\n";
        cout << "[2] Arvo huoneet uudelleen (poistaa varaukset) \n";
        cout << "[3] Anna huoneiden m‰‰r‰ manuaalisesti (poistaa varaukset)\n";
        cout << "Valitse toiminto: (1-3)";
        int valikkoValinta;
        cin >> valikkoValinta; cout << endl;
        if (cin.fail() || (valikkoValinta != 1 && valikkoValinta != 2 && valikkoValinta != 3)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tarkista syˆte NUMEROT(1-3)" << endl;
            continue;
        }
        // valikko vaihtoehdot 
        // 1 arpoo hotellihuoneen numerot jos ei ole tekstitiedostossa huoneiden_maara.txt mit‰‰n jos siell‰ on tietoa ohjelma ei arvo uudestaan
        // 2 tyhjent‰‰ huoneiden_maara.txt ja arpoo uudelleen
        // 3 tyhjent‰‰ huoneiden_maara.txt ja manuaalisesti k‰ytt‰j‰ antaa huoneiden m‰‰r‰n
        else if (huoneidenMaara == 0) {
            huoneidenMaara = arvoHuoneidenMaara();
            tallennaHuoneidenMaara(huoneidenMaara);
        }
        else if (valikkoValinta == 2) {
            huoneidenMaara = arvoHuoneidenMaara();
            tallennaHuoneidenMaara(huoneidenMaara);
            string tiedostonNimi = "varaukset.txt";

            // avaa tiedoston kirjoitustilassa poistaa tiedot
            ofstream tiedosto(tiedostonNimi, ios::trunc);
            tiedosto.close();;
        }
        else if (valikkoValinta == 3) {
            string tiedostonNimi = "varaukset.txt";
            ofstream tiedosto(tiedostonNimi, ios::trunc);
            tiedosto.close();

            huoneidenMaara = huoneidenMaaraManuaalinen();
        }


        lueVarauksetTiedostosta(varaukset);  // lukee varaukset.txt tiedoston ensimm‰isen valikon j‰lkeen
        int ylimaarainenHuone = huoneidenMaara % 2;      // laskee jakoj‰‰nnˆksen, voi tulla k‰ytt‰j‰n syˆtteest‰ lis‰‰t‰‰n yhdenhuoneisiin
        int yhdenVaratut = 0;
        int kahdenVaratut = 0;
        paivitaHuoneidenMaara(huoneidenMaara, yhdenVaratut, kahdenVaratut);
        int yhdenHengenJaljella = huoneidenMaara / 2 + ylimaarainenHuone - yhdenVaratut;
        int kahdenHengenJaljella = huoneidenMaara / 2 - kahdenVaratut;
        int vapaatHuoneet = yhdenHengenJaljella + kahdenHengenJaljella;


        cout << "Vapaiden huoneiden m‰‰r‰: " << vapaatHuoneet << endl;
        cout << "Kahden hengen huoneita: " << kahdenHengenJaljella << endl;
        cout << "Yhden hengen huoneita: " << yhdenHengenJaljella << endl;

        while (true) {
            cout << "\n[1] Varaa huone\n[2] Tarkastele varauksia\n[3] Lopeta\nValitse toiminto:(1-3) ";
            int valinta;
            cin >> valinta;

            if (cin.fail() || (valinta != 1 && valinta != 2 && valinta != 3)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Tarkista syˆte NUMEROT(1-3)" << endl;
            }
            else if (valinta == 1) {
                varaaHuone(varaukset, yhdenHengenJaljella, kahdenHengenJaljella);
                tallennaVarauksetTiedostoon(varaukset);
                cout << kahdenVaratut;
            }
            else if (valinta == 2) {
                tarkasteleVarauksia(varaukset);
            }
            else if (valinta == 3) {
                cout << "Ohjelma lopetetaan." << endl;
                tallennaVarauksetTiedostoon(varaukset);
                break;
            }
        }

        return 0;
    }
}