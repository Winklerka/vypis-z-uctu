#include <stdio.h>

// Symbolické konstanty
#define UROKOVA_MIRA 25    // Roční úroková míra (2.5%)
#define DAN_Z_UROKU 15     // Daň z úroků (15%)
#define MIN_VKLAD 1000     // Minimální měsíční vklad
#define MAX_VKLAD 10000    // Maximální měsíční vklad
#define VYSTUPNI_SOUBOR "vypisUctu.txt" // Jméno výstupního souboru

// Funkce pro načtení celého čísla v intervalu
int nacti_cele_cislo(int min, int max) {
    int cislo;
    do {
        printf("Zadejte cislo v rozmezi od %d do %d: ", min, max);
        scanf("%d", &cislo);
        if (cislo < min || cislo > max) {
            printf("Zadane cislo je mimo platny rozsah!\n");
        }
    } while (cislo < min || cislo > max);
    return cislo;
}

// Funkce pro zjištění, zda je zadaný rok přestupný
int je_prestupny(int rok) {
    if ((rok % 4 == 0 && rok % 100 != 0) || rok % 400 == 0) {
        return 1;
    } else {
        return 0;
    }
}

// Funkce pro zjištění počtu dní v měsíci
int pocet_dni_v_mesici(int mesic, int rok) {
    switch (mesic) {
        case 1:  // Leden
        case 3:  // Březen
        case 5:  // Květen
        case 7:  // Červenec
        case 8:  // Srpen
        case 10: // Říjen
        case 12: // Prosinec
            return 31;
        case 4:  // Duben
        case 6:  // Červen
        case 9:  // Září
        case 11: // Listopad
            return 30;
        case 2: // Únor
            if (je_prestupny(rok)) {
                return 29;
            } else {
                return 28;
            }
        default:
            return -1; // Chybný měsíc
    }
}

// Funkce pro výpočet úroku z vkladu
int vypocet_uroku(int stav_ucet, int mesicni_vklad) {
    int urok = stav_ucet * (UROKOVA_MIRA / 12);
    int dan_z_uroku = urok * DAN_Z_UROKU / 100;
    return urok - dan_z_uroku;
}

// Hlavní funkce programu
int main() {
    // Načtení vstupů od uživatele
    int pocatecni_stav_ucet = nacti_cele_cislo(0, 1000000000);
    int mesicni_vklad = nacti_cele_cislo(MIN_VKLAD, MAX_VKLAD);
    int rok;

    do {
        printf("Zadejte rok: ");
        scanf("%d", &rok);
        if (rok <= 0) {
            printf("Zadany rok neni platny!\n");
        }
    } while (rok <= 0);

    // Otevření výstupního souboru
    FILE *soubor;
    soubor = fopen(VYSTUPNI_SOUBOR, "w");
    if (soubor == NULL) {
        printf("Nelze otevrit soubor pro zapis!\n");
        return 1;
    }

    // Simulace pohybu na účtu
    int stav_ucet = pocatecni_stav_ucet;
    int stav_ucet_na_zacatku_roku = stav_ucet;

    // Hlavička tabulky
    fprintf(soubor, "-------------------------------------------------------------------\n");
    fprintf(soubor, "| Mesic | Pocet dni | Zac. mesice | Urok | Dan z uroku | Kon. mesice |\n");
    fprintf(soubor, "|-------|-----------|-------------|------|-------------|-------------|\n");

    for (int mesic = 1; mesic <= 12; mesic++) {
        // Zjištění počtu dní v aktuálním měsíci
        int dnu_v_mesici = pocet_dni_v_mesici(mesic, rok);

        // Výpočet úroku a daně z úroku
        int urok = vypocet_uroku(stav_ucet, mesicni_vklad);

        // Aktualizace stavu účtu
        stav_ucet += mesicni_vklad + urok;

        // Výpis do textového souboru
        fprintf(soubor, "| %5d | %9d | %11d | %4d | %11d | %11d |\n",
                mesic, dnu_v_mesici, stav_ucet - (mesicni_vklad + urok), urok, urok * DAN_Z_UROKU / 100, stav_ucet);

    }

    // Patička tabulky
    fprintf(soubor, "-------------------------------------------------------------------\n");

    // Výpis na konci roku
    int stav_ucet_na_konci_roku = stav_ucet;
    fprintf(soubor, "Stav uctu na konci roku %d: %d Kc\n", rok, stav_ucet_na_konci_roku);
    fprintf(soubor, "Rozdil mezi zacatkem a koncem roku: %d Kc\n", stav_ucet_na_konci_roku - pocatecni_stav_ucet);

    // Uzavření výstupního souboru
    fclose(soubor);

    printf("Vypis byl ulozen do souboru '%s'.\n", VYSTUPNI_SOUBOR);

    return 0;
}
