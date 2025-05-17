public class Policja extends JednostkaRatownicza {
    public Policja(int id, Lokalizacja baza) {
        super(id, baza);
    }

    @Override
    public void obsluzZgloszenie(Zgloszenie zgloszenie) {
        System.out.println("Policja " + id + " obsługuje zgłoszenie " + zgloszenie.getId());
    }
}
