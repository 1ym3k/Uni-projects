import java.util.ArrayList;
import java.util.List;

public class Szpital {
    private String nazwa;
    private Lokalizacja lokalizacja;
    private int liczbaMiejsc;
    private int dostepneMiejsca;
    private List<Osoba> pacjenci;

    public Szpital(String nazwa, Lokalizacja lokalizacja, int liczbaMiejsc) {
        this.nazwa = nazwa;
        this.lokalizacja = lokalizacja;
        this.liczbaMiejsc = liczbaMiejsc;
        this.dostepneMiejsca = liczbaMiejsc;
        this.pacjenci = new ArrayList<>();
    }

    public String getNazwa() {
        return nazwa;
    }
    public Lokalizacja getLokalizacja() {
        return lokalizacja;
    }
    public int getDostepneMiejsca() {
        return dostepneMiejsca;
    }

    public boolean przyjmijPacjenta(Osoba osoba) {
        if (dostepneMiejsca > 0) {
            pacjenci.add(osoba);
            dostepneMiejsca--;
            return true;
        }
        return false;
    }

    @Override
    public String toString() {
        return "Szpital{" +
                "nazwa='" + nazwa + '\'' +
                ", wolneMiejsca=" + dostepneMiejsca + "/" + liczbaMiejsc +
                '}';
    }
}
