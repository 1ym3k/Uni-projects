public class Osoba {
    private String pesel;

    public Osoba(String pesel) {
        this.pesel = pesel;
    }

    @Override
    public String toString() {
        return "Osoba{" + pesel + '}';
    }
}
