import java.util.Date;
import java.util.Random;

public class Symulacja {
    private static Random random = new Random();


    public static Wypadek generujWypadekZaleznyOdTypu() {
        double x = random.nextDouble()*20;
        double y = random.nextDouble()*20;
        Lokalizacja lok = new Lokalizacja(x, y);

        // Przykładowe typy
        String[] typy = {
                "Pożar",
                "Kolizja drogowa",
                "Zasłabnięcie",
                "Klęska żywiołowa",
                "Katastrofa budowlana",
                "Włamanie"
        };
        String typ = typy[random.nextInt(typy.length)];

        // Losowa liczba poszkodowanych [1..6]
        int poszk = 1 + random.nextInt(5);
        // Losowa liczba wymagających hospitalizacji (0..poszk)
        int hosp = random.nextInt(poszk+1);

        // Losowy czas pomocy [1..3]
        int czasPomocy = 1 + random.nextInt(3);

        // Domyślny priorytet = 1
        int priorytet = 1;
        boolean reqPolice = false;
        boolean reqAmbulance = false;
        boolean reqFire = false;

        switch (typ) {
            case "Pożar":
                // straż, pogotowie, priorytet 3
                reqFire = true;
                reqAmbulance = true;
                priorytet = 3;
                break;
            case "Włamanie":
                // policja, priorytet 1
                reqPolice = true;
                priorytet = 1;
                break;
            case "Kolizja drogowa":
                // policja + pogotowie, priorytet 2
                reqPolice = true;
                reqAmbulance = true;
                priorytet = 2;
                break;
            case "Zasłabnięcie":
                // tylko pogotowie, priorytet 1
                reqAmbulance = true;
                priorytet = 1;
                break;
            case "Klęska żywiołowa":
                // wszystko, priorytet 3
                reqPolice = true;
                reqAmbulance = true;
                reqFire = true;
                priorytet = 3;
                break;
            case "Katastrofa budowlana":
                // wszystko, priorytet 3
                reqPolice = true;
                reqAmbulance = true;
                reqFire = true;
                priorytet = 3;
                break;
        }

        // Tworzymy obiekt Wypadek
        return new Wypadek(
                new Date(),             // znacznik czasu
                lok,                    // lokalizacja
                typ,                    // typ
                poszk,                  // liczba poszkodowanych
                priorytet,             // priorytet
                czasPomocy,            // czas pomocy
                hosp,                   // ilu wymaga hospitalizacji
                reqPolice,
                reqAmbulance,
                reqFire
        );
    }

    public static void main(String[] args) {
        Dyspozytor dyspozytor = new Dyspozytor();

        // Dodajemy służby (3x Pogotowie, 3x Policja, 3x Straż)
        for (int i=1; i<=3; i++){
            dyspozytor.dodajJednostke(new Pogotowie(i, new Lokalizacja(0,0)));
        }
        for (int i=1; i<=3; i++){
            dyspozytor.dodajJednostke(new Policja(100+i, new Lokalizacja(5,5)));
        }
        for (int i=1; i<=3; i++){
            dyspozytor.dodajJednostke(new StrazPozarna(200+i, new Lokalizacja(10,10)));
        }

        // Dodajemy 2 szpitale
        Szpital sz1 = new Szpital("Szpital Centralny", new Lokalizacja(2,2), 10);
        Szpital sz2 = new Szpital("Szpital Północny", new Lokalizacja(12,12), 10);
        dyspozytor.dodajSzpital(sz1);
        dyspozytor.dodajSzpital(sz2);

        // 10 tur symulacji
        for (int t=1; t<=10; t++){
            dyspozytor.setCurrentTurn(t);
            System.out.println("\n=== Tura #" + t + " ===");

            // Generujemy wypadek (w zależności od typu - inne wymagania służb)
            Wypadek w = generujWypadekZaleznyOdTypu();
            System.out.println("Nowy wypadek: " + w);

            // Dodajemy do aktywnych
            dyspozytor.dodajWypadek(w);

            // Tworzymy zgłoszenie
            dyspozytor.generujZgloszenie(w);

            // Symulujemy turę
            dyspozytor.symulujTurę();

            // Wyświetlamy statystyki
            dyspozytor.wyswietlStatystyki();
        }
    }
}
