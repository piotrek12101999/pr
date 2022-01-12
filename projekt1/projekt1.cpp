#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

// Wektor wartości true/false dlugosci n;
// Jeżeli indeks wektora jest liczbą pierwszą, wartość tego indeksu przyjmuje wartość true
// Jeżeli nie - false
vector<bool> simpleSieve(int n)
{
    vector<bool> primes(n + 1, true);
    primes[0] = false;
    primes[1] = false;
    for (int i = 0; i < n; i++)
    {
        if (primes[i])
        {
            for (int j = i * i; j <= n; j += i)
                primes[j] = false;
        }
    }
    return primes;
}

vector<int> primesNumberIntVec(int n)
{
    vector<int> primes;
    vector<bool> primesBool = simpleSieve(n);
    for (int i = 0; i < primesBool.size(); i++)
    {
        if (primesBool[i])
        {
            primes.push_back(i);
        }
    }

    return primes;
}

vector<bool> divMethod(int low, int high)
{
    int r = sqrt(high);

    // Inicjalizacja wektora zawierającego liczby pierwsze w przedziale [0 - pierwiastek(high)]
    vector<int> primeSqrt = primesNumberIntVec(r);
    // Inicjalizacja wektora zawierającego liczby pierwsze w przedziale [low, high] - zapisanie każdej liczby jako pierwsza
    vector<bool> primes(high - low + 1, true);

    // Sprawdzenie każdej liczby metodą dzielenia

    omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for nowait
        for (int i = 0; i < primes.size(); i++)
        {
            int checkingNumber = i + low;
            for (int j = 0; j < primeSqrt.size(); j++)
            {
                if (checkingNumber == primeSqrt[j])
                {
                    break;
                }
                if (checkingNumber % primeSqrt[j] == 0)
                {
                    primes[i] = false;
                    break;
                }
            }
        }
    }

    return primes;
}

vector<bool> sieveMethod(int low, int high)
{
    int r = sqrt(high);

    // Inicjalizacja wektora zawierającego liczby pierwsze w przedziale [0 - pierwiastek(high)]
    vector<bool> primesSqrt = simpleSieve(r);
    // Inicjalizacja wektora zawierającego liczby pierwsze w przedziale [low, high] - zapisanie każdej liczby jako pierwsza
    vector<bool> sieve(high - low + 1, true);
    // Inicjalizacja tablicy bazowych wielokrotności - zapisanie każdej liczby jako złożona (wartość -1)
    vector<int> base(r, -1);

    // Szukanie bazowej wielokrotności każdej primeSqrt dla danego zakresu i
    for (int i = 0; i < primesSqrt.size(); i++)
    {
        if (primesSqrt[i])
        {
            base[i] = (low / i);
        }
    }

    // dla każdej primeSqrt przechodzącej przez zakres i oznaczającej wszystkie jej wielokrotności jako złożone
    for (int i = 0; i < primesSqrt.size(); i++)
    {
        if (primesSqrt[i])
        {
            for (int j = i * base[i]; j <= high; j += i)
            {
                if (j == i || j < low)
                {
                    continue;
                }
                sieve[j - low] = false;
            }
        }
    }

    return sieve;
}

int main()
{
    // Nie pytaj o min/max zakres - użyj domyślnych wartości
    bool debugMode = true;
    bool runDiv = true;
    bool runSieve = true;

    // Deklaracja zmiennych
    int counter = 0, low = 1, high = 100000000;
    clock_t start, stop;

    if (!debugMode)
    {
        // Zapytaj uzytkownika o dolna granice
        cout << "Dolna granica: ";
        cin >> low; // Take input

        // Zapytaj uzytkownika o gorna granice
        cout << "\Gorna granica: ";
        cin >> high; // Take input
    }

    if (low < 2)
    {
        low = 2;
    }

    // metoda dzielenia
    if (runDiv)
    {
        start = clock();
        vector<bool> resultDiv = divMethod(low, high);
        stop = clock();

        // Wypisanie liczb pierwszych
        for (int i = 0; i < resultDiv.size(); i++)
        {
            if (resultDiv[i])
            {
                counter++;
            }
        }

        printf("Znaleziono %i liczb pierwszych metoda dzielenia\n", counter);
        printf("Czas przetwarzania wynosi %f sec\n", ((double)(stop - start) / 1000.0));
    }

    // metoda sita
    if (runSieve)
    {
        start = clock();
        vector<bool> resultSieve = sieveMethod(low, high);
        stop = clock();

        counter = 0;
        // Wypisanie liczb pierwszych
        for (int i = 0; i < resultSieve.size(); i++)
        {
            if (resultSieve[i])
            {
                counter++;
            }
        }

        printf("Znaleziono %i liczb pierwszych metoda sita\n", counter);
        printf("Czas przetwarzania wynosi %f sec\n", ((double)(stop - start) / 1000.0));
    }

    return 0;
}