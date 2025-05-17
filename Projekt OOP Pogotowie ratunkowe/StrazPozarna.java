public class StrazPozarna extends JednostkaRatownicza {
    public StrazPozarna(int id, Lokalizacja baza) {
        super(id, baza);
    }

    @Override
    public void obsluzZgloszenie(Zgloszenie zgloszenie) {
        System.out.println("Straż Pożarna " + id + " obsługuje zgłoszenie " + zgloszenie.getId());
    }
}
