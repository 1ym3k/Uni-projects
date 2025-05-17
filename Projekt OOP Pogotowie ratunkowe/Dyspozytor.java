import java.util.ArrayList;
import java.util.List;

public class Dyspozytor {
    private List<Wypadek> wypadkiAktywne;
    private List<Wypadek> wypadkiHistoryczne;
    private List<Zgloszenie> zgloszeniaOczekujace;
    private List<Zgloszenie> zgloszeniaAktywne;
    private List<Zgloszenie> zgloszeniaZakonczone;

    private List<JednostkaRatownicza> jednostki;
    private List<Szpital> szpitale;

    private int currentTurn;

    public Dyspozytor() {
        this.wypadkiAktywne = new ArrayList<>();
        this.wypadkiHistoryczne = new ArrayList<>();
        this.zgloszeniaOczekujace = new ArrayList<>();
        this.zgloszeniaAktywne = new ArrayList<>();
        this.zgloszeniaZakonczone = new ArrayList<>();
        this.jednostki = new ArrayList<>();
        this.szpitale = new ArrayList<>();
        this.currentTurn = 0;
    }

    public void setCurrentTurn(int turn) {
        this.currentTurn = turn;
    }

    public void dodajWypadek(Wypadek w) {
        wypadkiAktywne.add(w);
    }

    public void dodajSzpital(Szpital sz) {
        szpitale.add(sz);
    }

    public void dodajJednostke(JednostkaRatownicza j) {
        jednostki.add(j);
    }

    public void generujZgloszenie(Wypadek w) {
        Zgloszenie z = new Zgloszenie(w, currentTurn);
        zgloszeniaOczekujace.add(z);
    }

    // Obsługa kolejki - przydział służb (wg priorytetu malejąco)
    private void obsluzKolejkeOczekujaca() {
        zgloszeniaOczekujace.sort((a,b)->Integer.compare(b.getPriorytet(), a.getPriorytet()));

        List<Zgloszenie> doAktywnych = new ArrayList<>();
        for (Zgloszenie z : zgloszeniaOczekujace) {
            boolean ok = probujPrzydzielicSluzby(z);
            if (ok) {
                z.setAktywny(true);
                doAktywnych.add(z);
                System.out.println("Służby przydzielone do zgłoszenia ID=" + z.getId()
                        + " (priorytet=" + z.getPriorytet() + ")");
            }
        }
        zgloszeniaOczekujace.removeAll(doAktywnych);
        zgloszeniaAktywne.addAll(doAktywnych);
    }

    private boolean probujPrzydzielicSluzby(Zgloszenie z) {
        JednostkaRatownicza pol = null;
        if (z.requiresPolice()) {
            pol = znajdzWolnaJednostke(Policja.class);
            if (pol == null) return false;
        }
        JednostkaRatownicza amb = null;
        if (z.requiresAmbulance()) {
            amb = znajdzWolnaJednostke(Pogotowie.class);
            if (amb == null) return false;
        }
        JednostkaRatownicza str = null;
        if (z.requiresFire()) {
            str = znajdzWolnaJednostke(StrazPozarna.class);
            if (str == null) return false;
        }

        if (pol != null) {
            pol.setStatus(StatusJednostki.ZADYSPONOWANY);
            z.dodajJednostke(pol);
        }
        if (amb != null) {
            amb.setStatus(StatusJednostki.ZADYSPONOWANY);
            z.dodajJednostke(amb);
        }
        if (str != null) {
            str.setStatus(StatusJednostki.ZADYSPONOWANY);
            z.dodajJednostke(str);
        }
        return true;
    }

    private JednostkaRatownicza znajdzWolnaJednostke(Class<?> klasa) {
        for (JednostkaRatownicza j : jednostki) {
            if (klasa.isInstance(j) && j.getStatus() == StatusJednostki.DOSTEPNY) {
                return j;
            }
        }
        return null;
    }

    public void symulujTurę() {
        // 1. Obsługa kolejki
        obsluzKolejkeOczekujaca();

        // 2. Zgłoszenia aktywne
        for (Zgloszenie z : new ArrayList<>(zgloszeniaAktywne)) {
            if (z.isAktywny()) {
                if (currentTurn > z.getStartTurn()) {
                    z.zwiekszTurSpedzona();
                }
                if (z.getLiczbaTurSpedzona() >= z.getLiczbaTurPotrzebna()) {
                    // Koniec akcji
                    z.setAktywny(false);
                    zgloszeniaAktywne.remove(z);
                    zgloszeniaZakonczone.add(z);

                    Wypadek w = z.getWypadek();
                    w.setAktywny(false);
                    wypadkiAktywne.remove(w);
                    wypadkiHistoryczne.add(w);

                    // Hospitalizacja
                    int doHosp = z.getLiczbaOsobWymagajacychHospitalizacji();
                    for (int i=0; i<doHosp; i++) {
                        Osoba pacjent = new Osoba("PESEL_" + z.getId() + "_" + i);
                        boolean ok = przypiszDoSzpitalaNajblizszego(pacjent, w.getLokalizacja());
                        if (!ok) {
                            System.out.println("Brak miejsc w szpitalach dla pacjenta: " + pacjent);
                        }
                    }

                    // Jednostki wracają do bazy
                    for (JednostkaRatownicza j : z.getJednostki()) {
                        j.setStatus(StatusJednostki.ZAJETY);
                    }
                }
            }
        }

        // 3. Ruch jednostek
        for (JednostkaRatownicza j : jednostki) {
            if (j.getStatus() == StatusJednostki.ZADYSPONOWANY) {
                // Jedzie do zgłoszenia
                Zgloszenie doc = znajdzZgloszenieDlaJednostki(j);
                if (doc != null) {
                    j.przemiescSie(doc.getWypadek().getLokalizacja(), 2.0);
                    double dist = j.getAktualnaLokalizacja().distanceTo(doc.getWypadek().getLokalizacja());
                    if (dist < 0.0001) {
                        j.setStatus(StatusJednostki.ZAJETY);
                        j.obsluzZgloszenie(doc);
                    }
                }
            } else if (j.getStatus() == StatusJednostki.ZAJETY) {
                // Wraca do bazy, jeśli zgłoszenie nieaktywne
                Zgloszenie doc = znajdzZgloszenieDlaJednostki(j);
                if (doc==null || !doc.isAktywny()) {
                    double distBazy = j.getAktualnaLokalizacja().distanceTo(j.getBaza());
                    if (distBazy>0.0001) {
                        j.przemiescSie(j.getBaza(), 2.0);
                    } else {
                        j.setStatus(StatusJednostki.DOSTEPNY);
                    }
                }
            }
        }
    }

    private Zgloszenie znajdzZgloszenieDlaJednostki(JednostkaRatownicza j) {
        for (Zgloszenie zg : zgloszeniaAktywne) {
            if (zg.getJednostki().contains(j)) {
                return zg;
            }
        }
        for (Zgloszenie zg : zgloszeniaZakonczone) {
            if (zg.getJednostki().contains(j)) {
                return zg;
            }
        }
        return null;
    }

    private boolean przypiszDoSzpitalaNajblizszego(Osoba pacjent, Lokalizacja source) {
        Szpital najlepszy = null;
        double minDist = Double.MAX_VALUE;

        for (Szpital s : szpitale) {
            if (s.getDostepneMiejsca() > 0) {
                double d = s.getLokalizacja().distanceTo(source);
                if (d < minDist) {
                    minDist = d;
                    najlepszy = s;
                }
            }
        }
        if (najlepszy != null) {
            boolean przyjety = najlepszy.przyjmijPacjenta(pacjent);
            if (przyjety) {
                System.out.println("Pacjent " + pacjent + " przyjęty do " + najlepszy.getNazwa()
                        + " (odl=" + String.format("%.2f", minDist) + ")");
                return true;
            }
        }
        return false;
    }

    public void wyswietlStatystyki() {
        System.out.println("\n=== STATYSTYKI (tura " + currentTurn + ") ===");
        System.out.println("Aktywne wypadki: " + wypadkiAktywne.size());
        System.out.println("Oczekujące zgłoszenia: " + zgloszeniaOczekujace.size());
        System.out.println("Aktywne zgłoszenia: " + zgloszeniaAktywne.size());

        System.out.println("\nJednostki:");
        for (JednostkaRatownicza j : jednostki) {
            System.out.println("  " + j);
        }

        System.out.println("\nSzpitale:");
        for (Szpital sz : szpitale) {
            System.out.println("  " + sz);
        }

        System.out.println("\n--- Kolejka oczekujących (po priorytecie) ---");
        zgloszeniaOczekujace.sort((a,b)->Integer.compare(b.getPriorytet(), a.getPriorytet()));
        for (Zgloszenie z : zgloszeniaOczekujace) {
            System.out.println("  " + z + " (WypadekID=" + z.getWypadek().getId() + ")");
        }

        System.out.println("\n--- Zgłoszenia aktywne ---");
        for (Zgloszenie z : zgloszeniaAktywne) {
            System.out.println("  " + z + " -> Jednostki=" + z.getJednostki());
        }

        System.out.println("-------------------------------------------");
    }
}
