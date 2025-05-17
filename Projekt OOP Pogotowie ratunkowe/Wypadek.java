import java.util.Date;
import java.util.concurrent.atomic.AtomicInteger;

public class Wypadek {
    private static final AtomicInteger generatorId = new AtomicInteger(0);

    private int id;
    private Date znacznikCzasu;
    private Lokalizacja lokalizacja;
    private boolean aktywny;
    private String typWypadku;

    private int priorytet;
    private int czasTrwaniaPomocy;
    private int liczbaOsobPoszkodowanych;
    private int liczbaOsobWymagajacychHospitalizacji;

    private boolean requiresPolice;
    private boolean requiresAmbulance;
    private boolean requiresFire;

    public Wypadek(Date znacznikCzasu, Lokalizacja lokalizacja, String typWypadku,
                   int liczbaOsobPoszkodowanych,
                   int priorytet,
                   int czasTrwaniaPomocy,
                   int liczbaOsobWymagajacychHospitalizacji,
                   boolean requiresPolice,
                   boolean requiresAmbulance,
                   boolean requiresFire)
    {
        this.id = generatorId.incrementAndGet();
        this.znacznikCzasu = znacznikCzasu;
        this.lokalizacja = lokalizacja;
        this.typWypadku = typWypadku;
        this.aktywny = true;

        this.priorytet = priorytet;
        this.czasTrwaniaPomocy = czasTrwaniaPomocy;
        this.liczbaOsobPoszkodowanych = liczbaOsobPoszkodowanych;
        this.liczbaOsobWymagajacychHospitalizacji = liczbaOsobWymagajacychHospitalizacji;

        this.requiresPolice = requiresPolice;
        this.requiresAmbulance = requiresAmbulance;
        this.requiresFire = requiresFire;
    }

    public int getId() { return id; }
    public Date getZnacznikCzasu() { return znacznikCzasu; }
    public Lokalizacja getLokalizacja() { return lokalizacja; }
    public String getTypWypadku() { return typWypadku; }

    public int getPriorytet() { return priorytet; }
    public int getCzasTrwaniaPomocy() { return czasTrwaniaPomocy; }
    public int getLiczbaOsobPoszkodowanych() { return liczbaOsobPoszkodowanych; }
    public int getLiczbaOsobWymagajacychHospitalizacji() { return liczbaOsobWymagajacychHospitalizacji; }

    public boolean requiresPolice() { return requiresPolice; }
    public boolean requiresAmbulance() { return requiresAmbulance; }
    public boolean requiresFire() { return requiresFire; }

    public boolean isAktywny() { return aktywny; }
    public void setAktywny(boolean aktywny) {
        this.aktywny = aktywny;
    }

    @Override
    public String toString() {
        return "Wypadek{" +
                "id=" + id +
                ", typ=" + typWypadku +
                ", priorytet=" + priorytet +
                ", lok=" + lokalizacja +
                ", poszk=" + liczbaOsobPoszkodowanych +
                ", hosp=" + liczbaOsobWymagajacychHospitalizacji +
                ", czasPomocy=" + czasTrwaniaPomocy +
                ", requires=[police=" + requiresPolice + ", amb=" + requiresAmbulance + ", fire=" + requiresFire + "]" +
                '}';
    }
}
