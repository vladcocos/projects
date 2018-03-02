package shapes;

import image.Image;
import tools.DrawTool;
import tools.DrawableShape;

public class Triangle extends Shape implements DrawableShape {
    private int firstPointX;
    private int firstPointY;
    private int secondPointX;
    private int secondPointY;
    private int thirdPointX;
    private int thirdPointY;

    public Triangle() {
    }

    // Constructorul pentru Triangle obtine din input coordonatele celor 3
    // puncte si culoarea si opacitatea pentru contur si pentru
    // interior
    public Triangle(final String[] input) {
        this.firstPointX = Integer.parseInt(input[INPUT1]);
        this.firstPointY = Integer.parseInt(input[INPUT2]);
        this.secondPointX = Integer.parseInt(input[INPUT3]);
        this.secondPointY = Integer.parseInt(input[INPUT4]);
        this.thirdPointX = Integer.parseInt(input[INPUT5]);
        this.thirdPointY = Integer.parseInt(input[INPUT6]);
        super.setStrokeColor(Integer.parseInt(input[INPUT7].substring(1), HEX));
        super.setStrokeOpacity(Integer.parseInt(input[INPUT8]));
        super.setFillColor(Integer.parseInt(input[INPUT9].substring(1), HEX));
        super.setFillOpacity(Integer.parseInt(input[INPUT10]));
    }

    public final int getFirstPointX() {
        return firstPointX;
    }

    public final int getFirstPointY() {
        return firstPointY;
    }

    public final int getSecondPointX() {
        return secondPointX;
    }

    public final int getSecondPointY() {
        return secondPointY;
    }

    public final int getThirdPointX() {
        return thirdPointX;
    }

    public final int getThirdPointY() {
        return thirdPointY;
    }

    // Metoda "accept" ce permite "visitor-ului" sa deseneze forma in imagine
    @Override
    public final void getDrawn(final DrawTool drawTool, final Image image) {
        drawTool.draw(this, image);
    }
}
