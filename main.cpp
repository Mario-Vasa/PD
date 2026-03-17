#include <windows.h>
#include <stdio.h>

#define LUNGIME_MAX_NUME 256
#define LUNGIME_MAX_DATE 512

void PrintValoriCheie(HKEY hRadacina, const wchar_t* cale)
{
    HKEY hCheie;
    LONG rezultat = RegOpenKeyExW(hRadacina, cale, 0, KEY_READ, &hCheie);

    if (rezultat != ERROR_SUCCESS)
    {
        wprintf(L"[EROARE] Nu s-a putut deschide cheia: %s (cod: %ld)\n", cale, rezultat);
        return;
    }

    DWORD numarValori = 0;
    DWORD lungimeMaxNume = 0;
    DWORD lungimeMaxDate = 0;

    rezultat = RegQueryInfoKeyW(
        hCheie, NULL, NULL, NULL, NULL, NULL, NULL,
        &numarValori, &lungimeMaxNume, &lungimeMaxDate, NULL, NULL
    );

    if (rezultat != ERROR_SUCCESS)
    {
        wprintf(L"[EROARE] RegQueryInfoKeyW a esuat (cod: %ld)\n", rezultat);
        RegCloseKey(hCheie);
        return;
    }

    wprintf(L"=== Cheie: HKLM\\%s ===\n", cale);
    wprintf(L"Total valori gasite: %lu\n\n", numarValori);

    for (DWORD idx = 0; idx < numarValori; idx++)
    {
        wchar_t bufNume[LUNGIME_MAX_NUME];
        BYTE    bufDate[LUNGIME_MAX_DATE];
        DWORD   dimNume = LUNGIME_MAX_NUME;
        DWORD   dimDate = LUNGIME_MAX_DATE;
        DWORD   tipDate = 0;

        rezultat = RegEnumValueW(
            hCheie, idx,
            bufNume, &dimNume,
            NULL, &tipDate,
            bufDate, &dimDate
        );

        if (rezultat != ERROR_SUCCESS)
        {
            wprintf(L"[AVERTISMENT] Nu s-a putut citi valoarea %lu\n", idx);
            continue;
        }

        wprintf(L"[%lu] Nume   : %s\n", idx + 1, dimNume > 0 ? bufNume : L"(Implicit)");

        switch (tipDate)
        {
            case REG_SZ:
            case REG_EXPAND_SZ:
                wprintf(L"     Tip    : %s\n", tipDate == REG_SZ ? L"REG_SZ" : L"REG_EXPAND_SZ");
                wprintf(L"     Valoare: %s\n", (wchar_t*)bufDate);
                break;

            case REG_DWORD:
                wprintf(L"     Tip    : REG_DWORD\n");
                wprintf(L"     Valoare: %lu (0x%08X)\n", *(DWORD*)bufDate, *(DWORD*)bufDate);
                break;

            case REG_QWORD:
                wprintf(L"     Tip    : REG_QWORD\n");
                wprintf(L"     Valoare: %llu\n", *(ULONGLONG*)bufDate);
                break;

            case REG_BINARY:
                wprintf(L"     Tip    : REG_BINARY\n");
                wprintf(L"     Valoare: ");
                for (DWORD b = 0; b < dimDate && b < 16; b++)
                    wprintf(L"%02X ", bufDate[b]);
                if (dimDate > 16) wprintf(L"...");
                wprintf(L"\n");
                break;

            default:
                wprintf(L"     Tip    : necunoscut (%lu)\n", tipDate);
                break;
        }

        wprintf(L"\n");
    }

    RegCloseKey(hCheie);
}

int main()
{
    const wchar_t* caleSubcheie =
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";

    PrintValoriCheie(HKEY_LOCAL_MACHINE, caleSubcheie);

    return 0;
}
