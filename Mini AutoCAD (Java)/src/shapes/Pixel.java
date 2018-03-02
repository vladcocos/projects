package shapes;

public class Pixel {
    private int x;
    private int y;

    public Pixel() {
    }

    public Pixel(final int x, final int y) {
        this.x = x;
        this.y = y;
    }

    public final int getX() {
        return x;
    }

    public final void setX(final int x) {
        this.x = x;
    }

    public final int getY() {
        return y;
    }

    public final void setY(final int y) {
        this.y = y;
    }
}
