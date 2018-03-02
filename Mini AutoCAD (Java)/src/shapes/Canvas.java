package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Canvas extends Shape implements DrawableShape {
    private int height;
    private int width;

    public Canvas() {
    }

    // Constructorul pentru Canvas obtine din input inaltimea, latimea, culoarea
    // si opacitatea
    public Canvas(final String[] input) {
        this.height = Integer.parseInt(input[INPUT1]);
        this.width = Integer.parseInt(input[INPUT2]);
        super.setFillColor(Integer.parseInt(input[INPUT3].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[INPUT4]));
    }

    public final int getHeight() {
        return height;
    }

    public final int getWidth() {
        return width;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
