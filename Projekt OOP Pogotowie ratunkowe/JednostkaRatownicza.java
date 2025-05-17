public abstract class JednostkaRatownicza {
    protected int id;
    protected Lokalizacja baza;
    protected Lokalizacja aktualnaLokalizacja;
    protected StatusJednostki status;

    public JednostkaRatownicza(int id, Lokalizacja baza) {
        this.id = id;
        this.baza = baza;
        this.aktualnaLokalizacja = baza;
        this.status = StatusJednostki.DOSTEPNY;
    }

    public int getId() {
        return id;
    }
    public Lokalizacja getBaza() {
        return baza;
    }
    public Lokalizacja getAktualnaLokalizacja() {
        return aktualnaLokalizacja;
    }
    public StatusJednostki getStatus() {
        return status;
    }
    public void setStatus(StatusJednostki status) {
        this.status = status;
    }

    /**
     * Uproszczone przemieszczanie się:
     * - Jeśli odległość <= maxDystansNaTurę, docieramy całkowicie.
     * - Inaczej przesuwamy się o proporcję drogi.
     */
    public void przemiescSie(Lokalizacja cel, double maxDystansNaTurę) {
        double odleglosc = aktualnaLokalizacja.distanceTo(cel);
        if (odleglosc <= maxDystansNaTurę) {
            // Możemy dotrzeć w tej turze w całości
            this.aktualnaLokalizacja = cel;
        } else {
            // Część drogi
            double ratio = maxDystansNaTurę / odleglosc;
            double newX = aktualnaLokalizacja.getX()
                    + ratio * (cel.getX() - aktualnaLokalizacja.getX());
            double newY = aktualnaLokalizacja.getY()
                    + ratio * (cel.getY() - aktualnaLokalizacja.getY());
            this.aktualnaLokalizacja = new Lokalizacja(newX, newY);
        }
    }

    public abstract void obsluzZgloszenie(Zgloszenie zgloszenie);

    @Override
    public String toString() {
        return getClass().getSimpleName()
                + " [id=" + id
                + ", lok=" + aktualnaLokalizacja
                + ", status=" + status + "]";
    }
}
