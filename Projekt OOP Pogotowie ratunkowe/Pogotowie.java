public class Pogotowie extends JednostkaRatownicza {
    public Pogotowie(int id, Lokalizacja baza) {
        super(id, baza);
    }

    @Override
    public void obsluzZgloszenie(Zgloszenie zgloszenie) {
        System.out.println("Pogotowie " + id + " obsługuje zgłoszenie " + zgloszenie.getId());
    }
}
