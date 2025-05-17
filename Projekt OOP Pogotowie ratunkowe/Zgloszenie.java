import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Zgloszenie {
    private static final AtomicInteger generatorId = new AtomicInteger(0);

    private int id;
    private boolean aktywny;
    private int startTurn;

    private int priorytet;
    private int liczbaTurPotrzebna;
    private int liczbaTurSpedzona;

    private int liczbaOsobPoszkodowanych;
    private int liczbaOsobWymagajacychHospitalizacji;

    private boolean requiresPolice;
    private boolean requiresAmbulance;
    private boolean requiresFire;

    private Wypadek wypadek;
    private List<JednostkaRatownicza> jednostki;

    public Zgloszenie(Wypadek wypadek, int startTurn) {
        this.id = generatorId.incrementAndGet();
        this.wypadek = wypadek;
        this.startTurn = startTurn;
        this.aktywny = false;

        this.priorytet = wypadek.getPriorytet();
        this.liczbaTurPotrzebna = wypadek.getCzasTrwaniaPomocy();
        this.liczbaTurSpedzona = 0;

        this.liczbaOsobPoszkodowanych = wypadek.getLiczbaOsobPoszkodowanych();
        this.liczbaOsobWymagajacychHospitalizacji = wypadek.getLiczbaOsobWymagajacychHospitalizacji();

        this.requiresPolice = wypadek.requiresPolice();
        this.requiresAmbulance = wypadek.requiresAmbulance();
        this.requiresFire = wypadek.requiresFire();

        this.jednostki = new ArrayList<>();
    }

    public int getId() {
        return id;
    }
    public boolean isAktywny() {
        return aktywny;
    }
    public void setAktywny(boolean akt) {
        this.aktywny = akt;
    }

    public int getPriorytet() {
        return priorytet;
    }
    public int getStartTurn() {
        return startTurn;
    }
    public int getLiczbaTurPotrzebna() {
        return liczbaTurPotrzebna;
    }
    public int getLiczbaTurSpedzona() {
        return liczbaTurSpedzona;
    }
    public void zwiekszTurSpedzona() {
        liczbaTurSpedzona++;
    }

    public int getLiczbaOsobPoszkodowanych() {
        return liczbaOsobPoszkodowanych;
    }
    public int getLiczbaOsobWymagajacychHospitalizacji() {
        return liczbaOsobWymagajacychHospitalizacji;
    }

    public Wypadek getWypadek() {
        return wypadek;
    }

    public boolean requiresPolice() {
        return requiresPolice;
    }
    public boolean requiresAmbulance() {
        return requiresAmbulance;
    }
    public boolean requiresFire() {
        return requiresFire;
    }

    public List<JednostkaRatownicza> getJednostki() {
        return jednostki;
    }
    public void dodajJednostke(JednostkaRatownicza j) {
        jednostki.add(j);
    }

    @Override
    public String toString() {
        return "Zgloszenie{" +
                "id=" + id +
                ", priorytet=" + priorytet +
                ", aktywny=" + aktywny +
                ", turPotrzebna=" + liczbaTurPotrzebna +
                ", turSpedzone=" + liczbaTurSpedzona +
                ", requires=[police=" + requiresPolice + ", amb=" + requiresAmbulance + ", fire=" + requiresFire + "]" +
                ", wypadekID=" + wypadek.getId() +
                '}';
    }
}
