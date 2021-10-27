// etorodane.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include <vector>

#include <cstdio>
#include <fstream>
#include <ostream>
#include <streambuf>
#include <ctime> 
#include <string>

#include <sstream>

using namespace std;

double GBtoUSD = 73.06422222222222;
double DEtoUSD = 0.8635141242937853;

struct data_t
{
    string identifikator;
    string ukon;
    double castka;
    double jednotky;
    string datumot;
    string datumuz;
    double paka;
    double rozpeti;
    double zisk;
    double sazbaot;
    double sazbauz;
    double takeprof;
    double stoploss;
    double poplatkypres;
    string zkopirovano;
    string napiste;
    string ISIN;
    string poznamky;
    int type;
    double nakup;
    double prodej;
};
/*
std::istream& operator>>(std::istream& ist, data_t& data)
{
    char comma;
    ist >> data.identifikator >> comma
        >> data.ukon >> comma
        >> data.castka >> comma
        >> data.jednotky >> comma
        >> data.datumot >> comma
        >> data.datumuz >> comma
        >> data.paka >> comma
        >> data.rozpeti >> comma
        >> data.zisk >> comma
        >> data.sazbaot >> comma
        >> data.sazbauz >> comma
        >> data.takeprof >> comma
        >> data.stoploss >> comma
        >> data.zkopirovano >> comma
        >> data.napiste >> comma
        >> data.ISIN >> comma
        >> data.poznamky
        ;
    return ist;
}*/

std::vector<data_t> datavect;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void parseCSV(char* file) {
    std::ifstream data(file);
    std::string line;
    bool first = true;
    while ((std::getline(data, line))/*&&(x<15)*/)
    {
        std::string delimiter = ";";

        data_t data;
        std::stringstream lineStream(line);
        if (first)first = false;
        else
        {
            size_t pos = 0;
            std::string token;
            int index = 0;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                //std::cout << token << std::endl;
                replace(token, ",", ".");
                replace(token, "\xa0", "");
                if (token == "")token = "999999999";
                switch (index) {
                case 0:data.identifikator = token; break;
                case 1:data.ukon = token; break;
                case 2:data.castka = stod(token); break;
                case 3:data.jednotky = stod(token); break;
                case 4:data.datumot = token; break;
                case 5:data.datumuz = token; break;
                case 6:data.paka = stod(token); break;
                case 7:data.rozpeti = stod(token); break;
                case 8:data.zisk = stod(token); break;
                case 9:data.sazbaot = stod(token); break;
                case 10:data.sazbauz = stod(token); break;
                case 11:data.takeprof = stod(token); break;
                case 12:data.stoploss = stod(token); break;
                case 13:data.poplatkypres = stod(token); break;
                case 14:data.zkopirovano = token; break;
                case 15:data.napiste = token; break;
                case 16:data.ISIN = token; break;
                case 17:data.poznamky = token; break;
                }
                line.erase(0, pos + delimiter.length());
                index++;
            }

            //lineStream >> data;
            datavect.push_back(data);
        }
    }
    //datavect.pop_back();
};

double ziskar[4];
double nakupar[4];
double prodejar[4];

int main()
{
    //parseCSV((char*)"c:\\Users\\Veselý\\Downloads\\etoro-dane.csv");
    parseCSV((char*)"c:\\Users\\tomas.vesely\\Downloads\\etoro-dane.csv");

    for (int i = 0; i < 4; i++)
    {
        ziskar[i] = 0;
        nakupar[i] = 0;
        prodejar[i] = 0;
    }
    for (int posi = 0; posi < datavect.size(); posi++)
    {
        datavect[posi].type = 0;//basic
        if (datavect[posi].napiste == "CFD")
        {
            datavect[posi].type = 1;//cfd
            /*if ((datavect[posi].ukon.substr(datavect[posi].ukon.size() - 4) == " ETF") ||
                (datavect[posi].ukon.substr(datavect[posi].ukon.size() - 7) == "ETF usd"))
                datavect[posi].type = 3;//etf*/
        }
        else
        {
            if (datavect[posi].ISIN == "999999999")
                datavect[posi].type = 2;//coin        
            if ((datavect[posi].ukon.substr(datavect[posi].ukon.size() - 4) == " ETF") ||
                /*(datavect[posi].ukon.substr(datavect[posi].ukon.size() - 7) == "ETF usd")||*/
                (datavect[posi].ukon.substr(datavect[posi].ukon.size() - 4) == " SPDR"))
                datavect[posi].type = 3;//etf
        }

        datavect[posi].nakup = datavect[posi].sazbaot * datavect[posi].jednotky;
        datavect[posi].prodej = datavect[posi].sazbauz * datavect[posi].jednotky;

        cout << datavect[posi].zisk << " ";
        //datavect[posi].zisk = (datavect[posi].sazbauz - datavect[posi].sazbaot) * datavect[posi].jednotky;

        cout << datavect[posi].zisk << " ";

        if (datavect[posi].ISIN.substr(0, 2) == "GB")
        {
            //datavect[posi].zisk /= GBtoUSD;
            datavect[posi].sazbaot /= GBtoUSD;
            datavect[posi].sazbauz /= GBtoUSD;
        }
        if (datavect[posi].ISIN.substr(0, 2) == "DE")
        {
            //datavect[posi].zisk /= DEtoUSD;
            datavect[posi].sazbaot /= DEtoUSD;
            datavect[posi].sazbauz /= DEtoUSD;
        }
        /*
        if (datavect[posi].ISIN.substr(0,2) == "GB")
        {
            double comp=datavect[posi].sazbauz - datavect[posi].sazbaot;
            double comp2 = datavect[posi].zisk / comp;
            datavect[posi].sazbaot *= comp2;
            datavect[posi].sazbauz *= comp2;
        }
        else
        if (datavect[posi].ISIN.substr(0, 2) == "DE")
        {
            double comp = datavect[posi].sazbauz - datavect[posi].sazbaot;
            double comp2 = datavect[posi].zisk / comp;
            datavect[posi].sazbaot *= comp2;
            datavect[posi].sazbauz *= comp2;
        }
        else
        {
            datavect[posi].zisk = (datavect[posi].sazbauz - datavect[posi].sazbaot) * datavect[posi].jednotky;
        }
        */

        cout << datavect[posi].zisk << " t:" << datavect[posi].type << "\n";

        double zisknaakcii = datavect[posi].sazbauz - datavect[posi].sazbaot;
        double vypjednotka = 0;
        if((datavect[posi].zisk!=0)&&(zisknaakcii!=0))
            vypjednotka = datavect[posi].zisk / zisknaakcii;

        ziskar[datavect[posi].type] += datavect[posi].zisk;
        nakupar[datavect[posi].type] += datavect[posi].sazbaot * vypjednotka;
        prodejar[datavect[posi].type] += datavect[posi].sazbauz * vypjednotka;
    }

    /*std::cout << "Basic and ETF: zisk:" << ziskar[0] + ziskar[3] << " nak:" << nakupar[0] + nakupar[3] << " pro:" << prodejar[0] + prodejar[3] << "\n";
    std::cout << "CFD: zisk:" << ziskar[1] << " nak:" << nakupar[1] << " pro:" << prodejar[1] << "\n";
    std::cout << "Krypt: zisk:" << ziskar[2] << " nak:" << nakupar[2] << " pro:" << prodejar[2] << "\n";
    //std::cout << "ETF: zisk:" << ziskar[3] << " nak:" << nakupar[3] << " pro:" << prodejar[3] << "\n";
    */

    nakupar[0] = prodejar[0] - ziskar[0];
    nakupar[1] = prodejar[1] - ziskar[1];
    nakupar[2] = prodejar[2] - ziskar[2];
    nakupar[3] = prodejar[3] - ziskar[3];

    std::cout << "Basic and ETF: zisk:" << ziskar[0]+ ziskar[3] << " nak:" << nakupar[0]+ nakupar[3] << " pro:" << prodejar[0]+ prodejar[3] << "\n";
    std::cout << "CFD: zisk:" << ziskar[1] << " nak:" << nakupar[1] << " pro:" << prodejar[1] << "\n";
    std::cout << "Krypt: zisk:" << ziskar[2] << " nak:" << nakupar[2] << " pro:" << prodejar[2] << "\n";
    //std::cout << "ETF: zisk:" << ziskar[3] << " nak:" << nakupar[3] << " pro:" << prodejar[3] << "\n";


}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
