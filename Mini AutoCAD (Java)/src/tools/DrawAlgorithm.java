package tools;

import java.util.ArrayList;

import image.Image;
import shapes.Pixel;

public class DrawAlgorithm {
    // Variabile si constante necesare pentru algoritmul Bresenham
    private boolean interchanged = false;
    private int x;
    private int y;
    private int deltaX;
    private int deltaY;
    private int s1;
    private int s2;
    private int aux;
    private static final int D3 = 3;
    private static final int D4 = 4;
    private static final int D6 = 6;
    private static final int D10 = 10;

    public DrawAlgorithm() {
    }

    // Metoda aplica algoritmul Bresenham pentru a trasa o linie din punctul de
    // coordonate (startX, startY) in punctul de coordonate (finalX, finalY)
    public final void drawLine(final int startX, final int startY, final int finalX,
            final int finalY, final Image image, final int color) {
        interchanged = false;
        x = startX;
        y = startY;
        deltaX = Math.abs(finalX - startX);
        deltaY = Math.abs(finalY - startY);
        s1 = (int) Math.signum(finalX - startX);
        s2 = (int) Math.signum(finalY - startY);

        if (deltaY > deltaX) {
            aux = deltaX;
            deltaX = deltaY;
            deltaY = aux;
            interchanged = true;
        } else {
            interchanged = false;
        }

        int error = 2 * deltaY - deltaX;

        for (int i = 0; i <= deltaX; i++) {
            if (x >= 0 && y >= 0 && x < image.getImage().getWidth()
                    && y < image.getImage().getHeight()) {
                image.getImage().setRGB(x, y, color);
            }

            while (error > 0) {
                if (interchanged) {
                    x = x + s1;
                } else {
                    y = y + s2;
                }

                error = error - 2 * deltaX;
            }

            if (interchanged) {
                y = y + s2;
            } else {
                x = x + s1;
            }

            error = error + 2 * deltaY;
        }
    }

    // Metoda foloseste algoritmul Bresenham modificat pentru a trasa conturul
    // unui cerc centrat in punctul de coordonate (startX, startY) si raza
    // radius
    public final void drawCircle(final int startX, final int startY, final int radius,
            final Image image, final int color) {
        x = 0;
        y = radius;
        int d = D3 - 2 * radius;

        while (x < y) {
            drawCircleMethod(startX, startY, x, y, image, color);

            x++;
            if (d < 0) {
                d = d + D4 * x + D6;
            } else {
                y--;
                d = d + D4 * (x - y) + D10;
            }

            drawCircleMethod(startX, startY, x, y, image, color);
        }
    }

    // Metoda coloreaza pixelii cei mai apropiati de conturul cercului
    public final void drawCircleMethod(final int startX, final int startY, final int p, final int q,
            final Image image, final int color) {
        if (startX + p < image.getImage().getWidth() && startY + q < image.getImage().getHeight()) {
            image.getImage().setRGB(startX + p, startY + q, color);
        }

        if (startX - p >= 0 && startY + q < image.getImage().getHeight()) {
            image.getImage().setRGB(startX - p, startY + q, color);
        }

        if (startX + p < image.getImage().getWidth() && startY - q >= 0) {
            image.getImage().setRGB(startX + p, startY - q, color);
        }

        if (startX - p >= 0 && startY - q >= 0) {
            image.getImage().setRGB(startX - p, startY - q, color);
        }

        if (startX + q < image.getImage().getWidth() && startY + p < image.getImage().getHeight()) {
            image.getImage().setRGB(startX + q, startY + p, color);
        }

        if (startX - q >= 0 && startY + p < image.getImage().getHeight()) {
            image.getImage().setRGB(startX - q, startY + p, color);
        }

        if (startX + q < image.getImage().getWidth() && startY - p >= 0) {
            image.getImage().setRGB(startX + q, startY - p, color);
        }

        if (startX - q >= 0 && startY - p >= 0) {
            image.getImage().setRGB(startX - q, startY - p, color);
        }

    }

    // Metoda coloreaza o forma geometrica cu o anumita culoare; se porneste din
    // punctul de start startPixel (cu coordonate x si y). Intrucat o metoda
    // recursiva ar fi umplut rapid stiva interna, am creat un ArrayList ce se
    // comporta ca o coada (detaliat in README).
    public final void fill(final Pixel startPixel, final int fillColor, final int strokeColor,
            final Image image) {

        ArrayList<Pixel> queue = new ArrayList<Pixel>();

        queue.add(startPixel);

        // Cat timp coada nu este goala, parcurge coada, verifica daca fiecare
        // element este inauntrul figurii (nu depaseste conturul) si daca este,
        // coloreaza pixelul respectiv; se verifica daca vecinii sunt inauntrul
        // figurii si daca sunt, se adauga in coada.
        while (!queue.isEmpty()) {
            Pixel currentPixel = queue.remove(0);

            if (currentPixel.getX() >= 0 && currentPixel.getY() >= 0
                    && currentPixel.getX() < image.getImage().getWidth()
                    && currentPixel.getY() < image.getImage().getHeight()
                    && image.getImage().getRGB(currentPixel.getX(),
                            currentPixel.getY()) != fillColor
                    && image.getImage().getRGB(currentPixel.getX(),
                            currentPixel.getY()) != strokeColor) {
                image.getImage().setRGB(currentPixel.getX(), currentPixel.getY(), fillColor);

                if (currentPixel.getY() - 1 >= 0) {
                    Pixel leftPixel = new Pixel(currentPixel.getX(), currentPixel.getY() - 1);
                    queue.add(leftPixel);
                }

                if (currentPixel.getY() + 1 < image.getImage().getHeight()) {
                    Pixel rightPixel = new Pixel(currentPixel.getX(), currentPixel.getY() + 1);
                    queue.add(rightPixel);
                }

                if (currentPixel.getX() - 1 >= 0) {
                    Pixel upPixel = new Pixel(currentPixel.getX() - 1, currentPixel.getY());
                    queue.add(upPixel);
                }

                if (currentPixel.getX() + 1 < image.getImage().getWidth()) {
                    Pixel downPixel = new Pixel(currentPixel.getX() + 1, currentPixel.getY());
                    queue.add(downPixel);
                }
            }
        }
    }
}
